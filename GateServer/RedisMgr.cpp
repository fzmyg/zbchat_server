/*
 * @Author: zbccc zbccc.p@gmail.com
 * @Date: 2024-10-18 21:28:47
 * @LastEditors: zbccc zbccc.p@gmail.com
 * @LastEditTime: 2024-11-08 14:54:26
 * @FilePath: /coding/Server/gateServer/RedisMgr.cpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "RedisMgr.h"
#include "ConfigMgr.h"
#include <cstdlib>
#include <iostream>
#include <cassert>
#include <cstring>
RedisConnectionMgr::~RedisConnectionMgr()
{
    //close();
}
RedisConnectionMgr::RedisConnectionMgr()
:_pool(5)
{
}
/*bool RedisConnectionMgr::connect(const std::string &host, unsigned short port,const std::string& pass)
{
    this->_redis_connect = redisConnect(host.c_str(),port);
    if(_redis_connect==nullptr){
        return false;
    }
    return auth(pass);
}*/
RedisConnectionMgr::ErrorCode RedisConnectionMgr::set(const std::string &key, const std::string &val)
{
    std::shared_ptr<RedisConnection> redis = this->_pool.getRedisConnection();
    if(redis == nullptr){
        return RedisConnectionMgr::ErrorCode::STOP;
    }
    redisContext* redis_connect = redis->_conn;
    redisReply* redis_reply = (redisReply*)redisCommand(redis_connect,"set %s %s",key.c_str(),val.c_str());
    if(redis_reply==nullptr || redis_reply->type == REDIS_REPLY_ERROR){
        if(redis_reply)
            freeReplyObject(redis_reply);
        this->_pool.storeRedisConnection(redis);
        return RedisConnectionMgr::ErrorCode::NETWORK_ERR;
    }
    if((redis_reply->type == REDIS_REPLY_STATUS && (strcmp(redis_reply->str,"OK")==0 || strcmp(redis_reply->str,"ok")==0))==false){
        this->_pool.storeRedisConnection(redis);
        freeReplyObject(redis_reply);
        return RedisConnectionMgr::ErrorCode::FAILURE;
    }
    this->_pool.storeRedisConnection(redis);
    freeReplyObject(redis_reply);
    redis->_last_opt_time = std::chrono::steady_clock::now();
    return RedisConnectionMgr::ErrorCode::SUCCESS;
}
RedisConnectionMgr::ErrorCode RedisConnectionMgr::get(const std::string &key, std::string &val)
{
    std::shared_ptr<RedisConnection> redis = this->_pool.getRedisConnection();
    if(redis == nullptr){
        return RedisConnectionMgr::ErrorCode::STOP;
    }
    redisContext* redis_connect = redis->_conn;
    redisReply * _redis_reply = (redisReply*)redisCommand(redis_connect,"get %s",key.c_str());
    if(_redis_reply==nullptr){ // redis服务器错误
        this->_pool.storeRedisConnection(redis);
        return RedisConnectionMgr::ErrorCode::NETWORK_ERR;
    }
    if(_redis_reply->type == REDIS_REPLY_NIL){ //未查找到，验证码失效
        this->_pool.storeRedisConnection(redis);   
        freeReplyObject(_redis_reply);
        return RedisConnectionMgr::ErrorCode::FAILURE;
    }
    this->_pool.storeRedisConnection(redis);   
    val.clear();
    val = _redis_reply->str;
    redis->_last_opt_time = std::chrono::steady_clock::now();
    return RedisConnectionMgr::ErrorCode::SUCCESS;
}
RedisConnectionMgr::ErrorCode RedisConnectionMgr::auth(const std::string &pass)
{
    std::shared_ptr<RedisConnection> redis = this->_pool.getRedisConnection();
    if(redis == nullptr){
        return RedisConnectionMgr::ErrorCode::STOP;
    }
    redisContext* redis_connect = redis->_conn;
    redisReply*redis_reply = (redisReply*)redisCommand(redis_connect,"auth %s",pass.c_str());
    if(redis_reply==nullptr){
        this->_pool.storeRedisConnection(redis);
        return RedisConnectionMgr::ErrorCode::NETWORK_ERR;
    }
    if(redis_reply->type == REDIS_REPLY_ERROR || (strcmp(redis_reply->str,"OK")!=0 && strcmp(redis_reply->str,"ok")!=0)){
        freeReplyObject(redis_reply);
        this->_pool.storeRedisConnection(redis);
        return RedisConnectionMgr::ErrorCode::FAILURE;
    }
    freeReplyObject(redis_reply);
    this->_pool.storeRedisConnection(redis);
    redis->_last_opt_time = std::chrono::steady_clock::now();
    return RedisConnectionMgr::ErrorCode::SUCCESS;
}
RedisConnectionMgr::ErrorCode RedisConnectionMgr::lpush(const std::string &key, const std::string &val)
{
    std::shared_ptr<RedisConnection> redis = this->_pool.getRedisConnection();
    if(redis == nullptr){
        return RedisConnectionMgr::ErrorCode::STOP;
    }
    redisContext* redis_connect = redis->_conn;
    redisReply * redis_reply = (redisReply*)redisCommand(redis_connect,"lpush %s %s",key.c_str(),val.c_str());
    if(redis_reply==nullptr){
        this->_pool.storeRedisConnection(redis);
        return RedisConnectionMgr::ErrorCode::NETWORK_ERR;
    }

    if(redis_reply->type != REDIS_REPLY_INTEGER|| redis_reply->type == REDIS_REPLY_ERROR || redis_reply->integer < 0){
        freeReplyObject(redis_reply);
        this->_pool.storeRedisConnection(redis);
        return RedisConnectionMgr::ErrorCode::FAILURE;
    }

    freeReplyObject(redis_reply);
    this->_pool.storeRedisConnection(redis);
    redis->_last_opt_time = std::chrono::steady_clock::now();
    return RedisConnectionMgr::ErrorCode::SUCCESS;
}

RedisConnectionMgr::ErrorCode RedisConnectionMgr::lpop(const std::string &key, std::string &val)
{
    std::shared_ptr<RedisConnection> redis = this->_pool.getRedisConnection();
    if(redis == nullptr){
        return RedisConnectionMgr::ErrorCode::STOP;
    }
    redisContext* redis_connect = redis->_conn;
    redisReply *redis_reply = (redisReply*)redisCommand(redis_connect,"lpop %s",key.c_str());
    if(redis_reply==nullptr){
        this->_pool.storeRedisConnection(redis);
        return RedisConnectionMgr::ErrorCode::NETWORK_ERR;
    }
    if(redis_reply->type != REDIS_REPLY_NIL || redis_reply->type == REDIS_REPLY_ERROR){
        
        freeReplyObject(redis_reply);
        this->_pool.storeRedisConnection(redis);
        return RedisConnectionMgr::ErrorCode::FAILURE;
    }
    val = redis_reply->str;
    freeReplyObject(redis_reply);
    this->_pool.storeRedisConnection(redis);
    redis->_last_opt_time = std::chrono::steady_clock::now();
    return RedisConnectionMgr::ErrorCode::SUCCESS;
}

RedisConnectionMgr::ErrorCode RedisConnectionMgr::rpush(const std::string &key, const std::string &val)
{
    std::shared_ptr<RedisConnection> redis = this->_pool.getRedisConnection();
    if(redis == nullptr){
        return RedisConnectionMgr::ErrorCode::STOP;
    }
    redisContext* redis_connect = redis->_conn;
    redisReply*redis_reply = (redisReply*)redisCommand(redis_connect,"rpush %s %s",key.c_str(),val.c_str());
    if(redis_reply==nullptr){
        this->_pool.storeRedisConnection(redis);
        return RedisConnectionMgr::ErrorCode::NETWORK_ERR;
    }
    if(redis_reply->type != REDIS_REPLY_INTEGER || redis_reply->type == REDIS_REPLY_ERROR ||redis_reply->integer < 0){
        
        freeReplyObject(redis_reply);
        redis_reply = nullptr;
        this->_pool.storeRedisConnection(redis);
        return RedisConnectionMgr::ErrorCode::FAILURE;
    }
    freeReplyObject(redis_reply);
    this->_pool.storeRedisConnection(redis);
    redis->_last_opt_time = std::chrono::steady_clock::now();
    return RedisConnectionMgr::ErrorCode::SUCCESS;
}

RedisConnectionMgr::ErrorCode RedisConnectionMgr::rpop(const std::string &key, std::string &val)
{
    std::shared_ptr<RedisConnection> redis = this->_pool.getRedisConnection();
    if(redis == nullptr){
        return RedisConnectionMgr::ErrorCode::STOP;
    }
    redisContext* redis_connect = redis->_conn;
    redisReply *redis_reply = (redisReply*)redisCommand(redis_connect,"rpop %s",key.c_str());
    if(redis_reply==nullptr){
        this->_pool.storeRedisConnection(redis);
        return RedisConnectionMgr::ErrorCode::NETWORK_ERR;
    }
    if(redis_reply->type != REDIS_REPLY_NIL || redis_reply->type == REDIS_REPLY_ERROR){
        if(redis_reply){
            freeReplyObject(redis_reply);
        }
        this->_pool.storeRedisConnection(redis);
        return RedisConnectionMgr::ErrorCode::FAILURE;
    }
    val = redis_reply->str;
    freeReplyObject(redis_reply);
    this->_pool.storeRedisConnection(redis);
    redis->_last_opt_time = std::chrono::steady_clock::now();
    return RedisConnectionMgr::ErrorCode::SUCCESS;
}

RedisConnectionMgr::ErrorCode RedisConnectionMgr::hset(const std::string &key, const std::string &hkey, const std::string &val)
{
    std::shared_ptr<RedisConnection> redis = this->_pool.getRedisConnection();
    if(redis == nullptr){
        return RedisConnectionMgr::ErrorCode::STOP;
    }
    redisContext* redis_connect = redis->_conn;
    redisReply *redis_reply = (redisReply*)redisCommand(redis_connect,"hset %s %s %s",key.c_str(),hkey.c_str(),val.c_str());
    if(redis_reply==nullptr){
        this->_pool.storeRedisConnection(redis);
        return RedisConnectionMgr::ErrorCode::NETWORK_ERR;
    }
    if(redis_reply->type == REDIS_REPLY_ERROR || redis_reply->type != REDIS_REPLY_INTEGER){
        
        freeReplyObject(redis_reply);
        this->_pool.storeRedisConnection(redis);
        return RedisConnectionMgr::ErrorCode::FAILURE;
    }
    freeReplyObject(redis_reply);
    this->_pool.storeRedisConnection(redis);
    redis->_last_opt_time = std::chrono::steady_clock::now();
    return RedisConnectionMgr::ErrorCode::SUCCESS;
}

RedisConnectionMgr::ErrorCode RedisConnectionMgr::del(const std::string &str)
{
    std::shared_ptr<RedisConnection> redis = this->_pool.getRedisConnection();
    if(redis == nullptr){
        return RedisConnectionMgr::ErrorCode::STOP;
    }
    redisContext* redis_connect = redis->_conn;
    redisReply*redis_reply = (redisReply*)redisCommand(redis_connect,"del %s",str.c_str());
    if(redis_reply==nullptr){
        this->_pool.storeRedisConnection(redis);
        return RedisConnectionMgr::ErrorCode::NETWORK_ERR;
    }
    if(redis_reply->type == REDIS_REPLY_ERROR || redis_reply->type == REDIS_REPLY_NIL){
        
        freeReplyObject(redis_reply);
        this->_pool.storeRedisConnection(redis);
        return RedisConnectionMgr::ErrorCode::FAILURE;
    }
    freeReplyObject(redis_reply);
    this->_pool.storeRedisConnection(redis);
    redis->_last_opt_time = std::chrono::steady_clock::now();
    return RedisConnectionMgr::ErrorCode::SUCCESS;
}

RedisConnectionMgr::ErrorCode RedisConnectionMgr::existsKey(const std::string &key)
{
    std::shared_ptr<RedisConnection> redis = this->_pool.getRedisConnection();
    if(redis == nullptr){
        return RedisConnectionMgr::ErrorCode::STOP;
    }
    redisContext* redis_connect = redis->_conn;
    redisReply* redis_reply = (redisReply*)redisCommand(redis_connect,"exists %s",key.c_str());
    if(redis_reply==nullptr){
        this->_pool.storeRedisConnection(redis);
        return RedisConnectionMgr::ErrorCode::NETWORK_ERR;
    }
    if(redis_reply->type == REDIS_REPLY_ERROR || redis_reply->type != REDIS_REPLY_STATUS){
        freeReplyObject(redis_reply);
        this->_pool.storeRedisConnection(redis);
        return RedisConnectionMgr::ErrorCode::FAILURE;
    }
    freeReplyObject(redis_reply);
    this->_pool.storeRedisConnection(redis);
    redis->_last_opt_time = std::chrono::steady_clock::now();
    return RedisConnectionMgr::ErrorCode::SUCCESS;
}


RedisConnectionMgr::ErrorCode RedisConnectionMgr::sadd(const std::string &set_name,const std::string&val)
{
    std::shared_ptr<RedisConnection> redis = this->_pool.getRedisConnection();
    if(redis == nullptr){
        return RedisConnectionMgr::ErrorCode::STOP;
    }
    redisContext* redis_connect = redis->_conn;
    redisReply* redis_reply = (redisReply*)redisCommand(redis_connect,"sadd %s %s",set_name.c_str(),val.c_str());
    if(redis_reply==nullptr){
        this->_pool.storeRedisConnection(redis);
        return RedisConnectionMgr::ErrorCode::NETWORK_ERR;
    }
    if(redis_reply->type == REDIS_REPLY_ERROR || redis_reply->type != REDIS_REPLY_INTEGER || redis_reply->integer == 0){
        freeReplyObject(redis_reply);
        this->_pool.storeRedisConnection(redis);
        return RedisConnectionMgr::ErrorCode::FAILURE;
    }
    freeReplyObject(redis_reply);
    this->_pool.storeRedisConnection(redis);
    redis->_last_opt_time = std::chrono::steady_clock::now();
    return RedisConnectionMgr::ErrorCode::SUCCESS;
}
RedisConnectionMgr::ErrorCode RedisConnectionMgr::sismember(const std::string&set_name,const std::string & val)
{
    std::shared_ptr<RedisConnection> redis = this->_pool.getRedisConnection();
    if(redis == nullptr){
        return RedisConnectionMgr::ErrorCode::STOP;
    }
    redisContext* redis_connect = redis->_conn;
    redisReply* redis_reply = (redisReply*)redisCommand(redis_connect,"sismember %s %s",set_name.c_str(),val.c_str());
    if(redis_reply==nullptr){
        this->_pool.storeRedisConnection(redis);
        return RedisConnectionMgr::ErrorCode::NETWORK_ERR;
    }
    if(redis_reply->type == REDIS_REPLY_ERROR || redis_reply->type != REDIS_REPLY_INTEGER || redis_reply->integer == 0){
        freeReplyObject(redis_reply);
        this->_pool.storeRedisConnection(redis);
        return RedisConnectionMgr::ErrorCode::FAILURE;
    }
    freeReplyObject(redis_reply);
    this->_pool.storeRedisConnection(redis);
    redis->_last_opt_time = std::chrono::steady_clock::now();
    return RedisConnectionMgr::ErrorCode::SUCCESS;
}


RedisConnectionMgr::ErrorCode RedisConnectionMgr::srem(const std::string&set_name,const std::string & val)
{
    std::shared_ptr<RedisConnection> redis = this->_pool.getRedisConnection();
    if(redis == nullptr){
        return RedisConnectionMgr::ErrorCode::STOP;
    }
    redisContext* redis_connect = redis->_conn;
    redisReply* redis_reply = (redisReply*)redisCommand(redis_connect,"srem %s %s",set_name.c_str(),val.c_str());
    if(redis_reply==nullptr){
        this->_pool.storeRedisConnection(redis);
        return RedisConnectionMgr::ErrorCode::NETWORK_ERR;
    }
    if(redis_reply->type == REDIS_REPLY_ERROR || redis_reply->type != REDIS_REPLY_INTEGER || redis_reply->integer == 0){
        freeReplyObject(redis_reply);
        this->_pool.storeRedisConnection(redis);
        return RedisConnectionMgr::ErrorCode::FAILURE;
    }
    freeReplyObject(redis_reply);
    this->_pool.storeRedisConnection(redis);
    redis->_last_opt_time = std::chrono::steady_clock::now();
    return RedisConnectionMgr::ErrorCode::SUCCESS;
}