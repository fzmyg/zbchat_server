/*
 * @Author: zbccc zbccc.p@gmail.com
 * @Date: 2024-10-18 21:16:09
 * @LastEditors: zbccc zbccc.p@gmail.com
 * @LastEditTime: 2024-11-23 11:47:08
 * @FilePath: /coding/Server/gateServer/RedisMgr.h
 */
#pragma once
#include "Singleton.hpp"
#include "ConfigMgr.h"
#include <hiredis/hiredis.h>
#include <memory>
#include <condition_variable>
#include <mutex>
#include <atomic>
#include <queue>
#include <thread>
#include <chrono>
#include <iostream>
struct RedisConnection{
    redisContext* _conn;
    std::chrono::steady_clock::time_point _last_opt_time;
    RedisConnection(redisContext* conn)
    :_conn(conn){
        _last_opt_time = std::chrono::steady_clock::now();
    }
};

struct RedisUserInfo{
    const char* _host;
    unsigned short _port;
    std::string _password;
};

class RedisPool
{
private:
    std::size_t _size;
    std::deque<std::shared_ptr<RedisConnection>> _pool;
    std::condition_variable _cv;
    std::mutex _mux;
    std::atomic<bool> _stop;
    std::unique_ptr<std::thread> _check_thread;
    RedisUserInfo _user_info;
public:
    ~RedisPool(){
        if(!_stop){
            close();
        }
    }
    RedisPool(std::size_t size):_size(size),_stop(false){
        const char* redis_ip = (*ConfigMgr::getInstance())["RedisServer"]["Host"].c_str();
        unsigned short redis_port = atoi((*ConfigMgr::getInstance())["RedisServer"]["Port"].c_str());
        std::string pass = (*ConfigMgr::getInstance())["RedisServer"]["Password"];
        _user_info._host = redis_ip;
        _user_info._port = redis_port;
        _user_info._password = pass;
        std::size_t success_cnt = 0;
        for(int i = 0; i<size; i++){
            redisContext* redis_context = redisConnect(redis_ip,redis_port);
            if(redis_context == nullptr)
                continue;
            else{
                if(redis_context->err != 0){
                    redisFree(redis_context);
                    continue;
                }
            }
      
            redisReply* reply = (redisReply*)redisCommand(redis_context,"auth %s",pass.c_str());
            if(reply==nullptr){
                redisFree(redis_context);
                continue;
            }
            if(reply->type!=REDIS_REPLY_STATUS){
                redisFree(redis_context);
                freeReplyObject(reply);
                continue;
            }
            _pool.emplace_back(std::make_shared<RedisConnection>(redis_context));
            success_cnt ++;
        }
        _check_thread.reset(new std::thread([&]()mutable{
            std::cout<<"redis 检测线程成功启动 ，线程id:"<<std::this_thread::get_id()<<std::endl;
            while(_stop==false){
                checkConnection();
                std::this_thread::sleep_for(std::chrono::seconds(60));
            }
        }));
        _check_thread->detach();
    }

    std::shared_ptr<RedisConnection> getRedisConnection(){
        std::unique_lock<std::mutex> lock(_mux);
        _cv.wait(lock,[=](){
            if(_stop)
                return true;
             return !_pool.empty();
        });
        if(_stop){
            return nullptr;
        }
        auto ptr = _pool.front();
        _pool.pop_front();
        return ptr;
    }

    void storeRedisConnection(const std::shared_ptr<RedisConnection> &rc){
        if(_stop)
            return;
        this->_pool.push_back(rc);
        _cv.notify_one();
    }

    void close(){
        _stop = true;
        _cv.notify_all();
        std::unique_lock<std::mutex> lock(_mux);
        while(_pool.empty()==false){
            redisFree(_pool.front()->_conn);
            _pool.pop_front();
        }
    }
private:
    void checkConnection()
    {
        int size = _pool.size();
        std::chrono::steady_clock::duration cur_dur = std::chrono::steady_clock::now().time_since_epoch();
        long long int time_stamp = std::chrono::duration_cast<std::chrono::seconds>(cur_dur).count();
        for(int i = 0;i < size; i++){
            std::shared_ptr<RedisConnection> conn = nullptr;
            {
                std::unique_lock<std::mutex> lock(_mux);
                conn = this->_pool.front();
                _pool.pop_front();
            }
            long long int last_opt_time = std::chrono::duration_cast<std::chrono::seconds>(conn->_last_opt_time.time_since_epoch()).count();
            if(last_opt_time - time_stamp > 300) //五分钟内连接未操作需检测链接
            {
                redisContext* redis_connect = conn->_conn;
                redisReply* reply = (redisReply*)redisCommand(redis_connect,"ping");
                if(reply==nullptr) //网络出错
                {
                    redisFree(redis_connect);
                    redisContext* new_context = nullptr;
                    bool reconnect_tag = false;
                    do{
                        new_context = redisConnect(_user_info._host,_user_info._port);
                        if(new_context == nullptr) continue;
                        if(new_context->err != 0) {redisFree(new_context);continue;}
                        redisReply* reply = (redisReply*)redisCommand(new_context,"auth %s",_user_info._password.c_str());
                        if(reply==nullptr){
                            redisFree(new_context);
                            continue;
                        }
                        if(reply->type!=REDIS_REPLY_STATUS){
                            redisFree(new_context);
                            freeReplyObject(reply);
                            continue;
                        }
                        //重连成功
                        {
                            std::unique_lock<std::mutex> lock(_mux);
                            _pool.emplace_back(std::make_shared<RedisConnection>(new_context));
                        }
                        reconnect_tag =true;
                    }while(reconnect_tag == false);
                }
            }else{
                _pool.push_back(conn);
            }
        }
    }
};

class RedisConnectionMgr
:public Singleton<RedisConnectionMgr>
{
    friend class Singleton<RedisConnectionMgr>;
protected:
    RedisConnectionMgr();
public:
    ~RedisConnectionMgr();
    
    enum class ErrorCode{
        SUCCESS = 0,
        NETWORK_ERR = -1,
        FAILURE = 1,
        STOP = 2
    };

    //bool connect(const std::string& host,unsigned short port,const std::string& pass);
    //void close();
    // 0正确 -1网络错误
    ErrorCode set(const std::string & key,const std::string&val);
    // 0正确 -1网络错误 1未查找到
    ErrorCode get(const std::string& key,std::string &val);
    // 0正确 -1网络错误
    ErrorCode auth(const std::string &pass);
    // 0正确 -1网络错误
    ErrorCode lpush(const std::string&key,const std::string&val);
    // 0正确 -1网络错误 1列表为空
    ErrorCode lpop(const std::string&key,std::string&val);
    // 0正确 -1网络错误
    ErrorCode rpush(const std::string &key,const std::string&val);
    // 0正确 -1网络错误 1列表为空
    ErrorCode rpop(const std::string & key,std::string&val);
    // 0正确 -1网络错误 
    ErrorCode hset(const std::string&key,const std::string&hkey,const std::string& val);
    // 0正确 -1网络错误 1不存在
    ErrorCode hget(const std::string&key,const std::string&hkey,std::string&ret_val);
    // 0正确 -1网络错误 1不存在
    ErrorCode hdel(const std::string &key,const std::string& hkey);
    // 0正确 -1网络错误 1不存在相应键
    ErrorCode del(const std::string& str);
    // 0存在 -1网络错误 1不存在
    ErrorCode existsKey(const std::string&key);
    // 0添加成功 -1网络错误 1添加失败（该val已在set中存在）
    ErrorCode sadd(const std::string &set_name,const std::string&val);
    // 0在集合中 -1网络错误 1不在集合中
    ErrorCode sismember(const std::string&set_name,const std::string & val);
    // 0删除成功 -1网络错误 1原本没有
    ErrorCode srem(const std::string&set_name,const std::string & val);

private:
    RedisPool _pool;
};
