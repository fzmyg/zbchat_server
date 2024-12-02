/*
 * @Author: zbccc zbccc.p@gmail.com
 * @Date: 2024-10-21 10:24:27
 * @LastEditors: zbccc zbccc.p@gmail.com
 * @LastEditTime: 2024-11-26 10:33:39
 * @FilePath: /coding/Server/gateServer/MySQLMgr.h
 */
#pragma once
#include "Singleton.hpp"
#include "ConfigMgr.h"
#include "LoginError.h"
#include "Defer.hpp"
#include "Error.h"
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/exception.h>
#include <thread>
#include <condition_variable>
#include <queue>
#include <mutex>
#include <atomic>
#include <chrono>
#include <tuple>
#include <type_traits>
//定义了时间的连接信息
struct MysqlConnection{
    std::shared_ptr<sql::Connection> _conn;
    std::chrono::steady_clock::time_point _last_opt_time;
    MysqlConnection(sql::Connection*conn)
    :_conn(conn)
    {
        _last_opt_time = std::chrono::steady_clock::now();
    }
};
//
class MysqlPool{
public:
    MysqlPool()=default;
    MysqlPool(const std::string& url,const std::string& user,const std::string&pass,int size = 5)
    :_url(url)
    ,_user(user)
    ,_pass(pass)
    {
        for(int i = 0;i<size;i++){
            sql::Driver * driver = sql::mysql::get_driver_instance();
            std::shared_ptr<MysqlConnection> conn = std::make_shared<MysqlConnection>(driver->connect(url,user,pass));
            if(conn == nullptr){
                std::cout<<"创建"<<i+1<<"号Mysql链接失败"<<std::endl;
                i--;
                continue;
            }else{
                std::cout<<"创建"<<i+1<<"号Mysql链接成功"<<std::endl;
            }
            _conns.emplace_back(conn);
        }

        _check_thread = std::thread([this](){
            std::cerr<<"Mysql连接池检测线程启动 线程id:"<<std::this_thread::get_id()<<std::endl;
            while(this->_stop==false){
                checkConnection();
                std::this_thread::sleep_for(std::chrono::seconds(60));
            }
        });   
        _check_thread.detach();
    }
    void close(){
        _stop=true;
        this->_cv.notify_all();
    }
    ~MysqlPool()
    {
        std::unique_lock<std::mutex> lock(_mux);
        _conns.clear();
    }
    std::shared_ptr<MysqlConnection> getConnection();
    bool returnConnection(const std::shared_ptr<MysqlConnection>& ptr);
    void checkConnection();
private:
    std::deque<std::shared_ptr<MysqlConnection>> _conns;
    std::string _url;
    std::string _user;
    std::string _pass;
    std::mutex _mux;
    std::condition_variable _cv;
    std::atomic<bool> _stop;
    std::thread _check_thread;
};


class MysqlDao{
public:
    MysqlDao()=default;
    MysqlDao(const std::string& url,const std::string& user,const std::string&pass,int size)
    :_pool(new MysqlPool(url,user,pass,size)){
        
    }
    ~MysqlDao(){
        _pool->close();
    }
    template<typename... Vals>
    bool insert(const std::string& database,const std::string& table,const Vals&... vals)
    {
        std::tuple<Vals...> tup ( vals... );
        std::tuple<Vals...> args(vals...);
        std::shared_ptr<MysqlConnection> conn = _pool->getConnection();
        if(conn==nullptr){
            return false;
        }
        Defer return_conn([this,conn](){ this->_pool->returnConnection(conn);});
        conn->_conn->setSchema(database);
        int params_size = sizeof...(vals);
        std::string prepare_command = "insert into " + table+" values (";
        for(int i = 0;i < params_size;i++){
            prepare_command += "? ";
        }
        prepare_command += ");";
        std::unique_ptr<sql::PreparedStatement> prepare_statement(conn->_conn->prepareStatement(prepare_command));
        for(int i = 0;i < params_size;i++){
            auto val = std::get<i>(tup);
            if constexpr ( std::is_integral<decltype(val)>() ){
                prepare_statement->setInt(i+1,val);
            }else if constexpr (std::is_floating_point<decltype(val)>()){
                prepare_statement->setDouble(i+1,val);
            }else if constexpr (std::is_same<decltype(val),std::string>()){
                prepare_statement->setString(i+1,val);
            }
        }
        prepare_statement->execute();
        //_pool->returnConnection(conn);
        return true;
    }
    void reset(const std::string& url,const std::string& user,const std::string&pass,int size){
        _pool.reset(new MysqlPool(url,user,pass,size));
    }
    int regUser(const std::string name,const std::string&email,const std::string&pass)
    {
        std::shared_ptr<ConfigMgr> config = ConfigMgr::getInstance();
        std::string data_base = (*config)["MySQLServer"]["Schema"];
        std::shared_ptr<MysqlConnection> conn = _pool->getConnection();
        if(conn==nullptr){
            return -1;
        }
        Defer return_conn([this,conn](){ this->_pool->returnConnection(conn);});
        try{
        conn->_conn->setSchema(data_base);
        std::unique_ptr<sql::PreparedStatement> pre_stat (conn->_conn->prepareStatement("call reg_user(?,?,?,@result)"));
        pre_stat->setString(1,name);
        pre_stat->setString(2,email);
        pre_stat->setString(3,pass);
        pre_stat->executeUpdate();
        std::unique_ptr<sql::Statement> stat (conn->_conn->createStatement());
        std::unique_ptr<sql::ResultSet> res(stat->executeQuery("select @result;"));
        if(res->next()){
            int ret = res->getInt(1);   
            //_pool->returnConnection(conn);
            return ret;
        }
        }catch(sql::SQLException& e){
            //_pool->returnConnection(conn);
            std::cerr<<"In function regUser :"<<e.what()<<std::endl;
            return -1;
        }
        return -1;
    }
    // -3 连接出错 -1用户不存在 -2密码错误 0
    int loginVerify(const std::string & email,const std::string & pass)
    {
        int uid = -1;
        std::string password;
        std::shared_ptr<ConfigMgr> config = ConfigMgr::getInstance();
        std::string data_base = (*config)["MySQLServer"]["Schema"];
        std::shared_ptr<MysqlConnection> conn = _pool->getConnection();
        if(conn==nullptr){
            return MYSQL_CONNECT_ERROR;
        }
        Defer return_conn([this,conn](){ this->_pool->returnConnection(conn);});
        try{
            conn->_conn->setSchema(data_base);
            std::unique_ptr<sql::PreparedStatement> pre_stat (conn->_conn->prepareStatement("select uid,pass from user where email = ?"));
            pre_stat->setString(1,email);
            std::unique_ptr<sql::ResultSet> ress (pre_stat->executeQuery());
            if(ress->next() == false){
                //_pool->returnConnection(conn);
                return USER_NOT_EXIST; //用户不存在
            }  
            uid = ress->getInt(1);
            password = ress->getString(2);
            if(pass != password){ //密码不对
                //_pool->returnConnection(conn);
                return PASSWORD_NOT_MATCH;
            }else{ //密码正确
                //_pool->returnConnection(conn);
                return uid; //返回用户id
            }
        }catch(sql::SQLException&e){
            std::cerr<<"MysqlDao::loginVerify error"<<e.what();
            return 0;
        }
        return 0;
    }
    int queryRegisteredUser(const std::string & email){
        std::shared_ptr<MysqlConnection> conn = _pool->getConnection();
        if(conn==nullptr){
            return MYSQL_CONNECT_ERROR;
        }
        Defer return_conn([this,conn](){ this->_pool->returnConnection(conn);});
        std::shared_ptr<ConfigMgr> config = ConfigMgr::getInstance();
        std::string data_base = (*config)["MySQLServer"]["Schema"];   
        conn->_conn->setSchema(data_base);
        std::unique_ptr<sql::PreparedStatement> stat ( conn->_conn->prepareStatement("select 1 into @result from user where email = ?;"));
        stat->setString(1,email);
        std::unique_ptr<sql::Statement> stat2(conn->_conn->createStatement());

        std::unique_ptr<sql::ResultSet>res(stat2->executeQuery("select @result"));
        if(res->next()==true){ //查询到有用户
            return 0;
        }
        return -1; //查询到无用户
    }
    int modifyPassword(const std::string& email,const std::string & new_password)
    {
        std::shared_ptr<MysqlConnection> conn = _pool->getConnection();
        if(conn==nullptr){
            return MYSQL_CONNECT_ERROR;
        }
        Defer return_conn([this,conn](){ this->_pool->returnConnection(conn);});
        std::shared_ptr<ConfigMgr> config = ConfigMgr::getInstance();
        std::string data_base = (*config)["MySQLServer"]["Schema"];   
        conn->_conn->setSchema(data_base);
        std::unique_ptr<sql::PreparedStatement> stat ( conn->_conn->prepareStatement("call modifyPassword(?,?,@result);"));
        stat->setString(1,email);
        stat->setString(2,new_password);
        stat->execute();
        std::unique_ptr<sql::Statement> stat2(conn->_conn->createStatement());
        std::unique_ptr<sql::ResultSet>res(stat2->executeQuery("select @result"));
        if(res->next()){
            return res->getInt(1);
        }
        return -1;
    }

    /*int updateUserInfo(int uid,std::string nick_name,int age,SEX sex,std::string icon_pixmap,std::string location,std::string desc_msg)
    {
        std::shared_ptr<MysqlConnection> conn = _pool->getConnection();
        if(conn==nullptr){
            return MYSQL_CONNECT_ERROR;
        }
        Defer return_conn([this,conn](){ this->_pool->returnConnection(conn);});
        std::shared_ptr<ConfigMgr> config = ConfigMgr::getInstance();
        std::string data_base = (*config)["MySQLServer"]["Schema"];   
        message::AddIconRsp rsp = ResourceGrpcClient::getInstance()->addIcon(uid,icon_pixmap);
        if(rsp.error()!=ErrorCode::SUCCESS){
            return -1;
        }
        std::string icon_url = rsp.url();
        conn->_conn->setSchema(data_base);
        std::unique_ptr<sql::PreparedStatement> stat ( conn->_conn->prepareStatement("call updateUserInfo(?,?,?,?,?,?,?,@result);"));
        stat->setInt(1,uid);
        stat->setString(2,nick_name);
        stat->setInt(3,age);
        stat->setInt(4,sex);
        stat->setString(5,icon_url);
        stat->setString(6,location);
        stat->setString(7,desc_msg);
        stat->execute();
        std::unique_ptr<sql::Statement> stat2(conn->_conn->createStatement());
        std::unique_ptr<sql::ResultSet>res(stat2->executeQuery("select @result"));
        if(res->next()){
            return res->getInt(1);
        }
        return -1;
    }*/
protected:
    std::unique_ptr<MysqlPool> _pool;
};


class MySQLMgr
:public Singleton<MySQLMgr>
{
friend class Singleton<MySQLMgr>;

protected:
    MySQLMgr();
public:    
    ~MySQLMgr(){}
    
    int regUser(const std::string& name,const std::string & email,const std::string&pass);

    int loginVerify(const std::string& email,const std::string& pass);

    int queryRegisteredUser(const std::string & email);

    int modifyPassword(const std::string& email,const std::string & new_password);

    //int updateUserInfo(int uid,std::string nick_name,int age,SEX sex,std::string icon_url,std::string location,std::string desc_msg);
private:
    MysqlDao _dao;
};

