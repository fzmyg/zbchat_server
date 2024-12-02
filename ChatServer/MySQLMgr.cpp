/*
 * @Author: zbccc zbccc.p@gmail.com
 * @Date: 2024-10-24 08:51:53
 * @LastEditors: zbccc zbccc.p@gmail.com
 * @LastEditTime: 2024-11-27 20:10:38
 * @FilePath: /coding/Server/gateServer/MysqlMgr.cpp
 */
#include "MySQLMgr.h"
#include "ConfigMgr.h"
//获取连接
std::shared_ptr<MysqlConnection> MysqlPool::getConnection()
{
    std::unique_lock<std::mutex> lock(_mux);
    _cv.wait(lock, [&]()
             {
            if(_stop)
                return true;
            return this->_conns.empty()==false; });
    if (_stop)
        return std::shared_ptr<MysqlConnection>(nullptr);
    auto ret = _conns.front();
    _conns.pop_front();
    return ret;
}

//返回连接
bool MysqlPool::returnConnection(const std::shared_ptr<MysqlConnection> &ptr)
{
    ptr->_last_opt_time=std::chrono::steady_clock::now();
    if (_stop)
        return false;
    {
        std::unique_lock<std::mutex> lock(_mux);
        _conns.push_back(ptr);
        _cv.notify_one();
    }
    return true;
}

void MysqlPool::checkConnection()
{
    std::unique_lock<std::mutex> lock(_mux);
    std::chrono::steady_clock::duration cur_dur = std::chrono::steady_clock::now().time_since_epoch();
    long long int time_stamp = std::chrono::duration_cast<std::chrono::seconds>(cur_dur).count();
    int size = _conns.size();
    for(int i = 0;i < size; i++){
        std::shared_ptr<MysqlConnection> con_ptr = _conns.front();
        _conns.pop_front();
        long long int last_opt_time = std::chrono::duration_cast<std::chrono::seconds>(con_ptr->_last_opt_time.time_since_epoch()).count();
        if(time_stamp - last_opt_time > 600){ //距离上次操作时间大于5s,需重新检测连接是否存在
            try{
                std::unique_ptr<sql::Statement> stat (con_ptr->_conn->createStatement());
                sql::SQLString command = "SELECT 1";
                stat->executeQuery(command); //检测是否断开连接
                con_ptr->_last_opt_time=std::chrono::steady_clock::now();
            }catch(sql::SQLException&e){ //断开连接
                sql::Driver* driver = sql::mysql::get_driver_instance();
                std::shared_ptr<MysqlConnection> conn = std::make_shared<MysqlConnection>(driver->connect(_url,_user,_pass));
                this->_conns.push_back(conn);
                continue;
            }
        }
        _conns.push_back(con_ptr);
    }
}


MySQLMgr::MySQLMgr()
{
    std::shared_ptr<ConfigMgr> config = ConfigMgr::getInstance();
    std::string ip = (*config)["MySQLServer"]["Host"];
    std::string port = (*config)["MySQLServer"]["Port"];
    std::string user = (*config)["MySQLServer"]["User"];
    std::string pass = (*config)["MySQLServer"]["Password"];
    _dao.reset(ip+":"+port,user,pass,5);
}

int MySQLMgr::regUser(const std::string& name,const std::string & email,const std::string&pass)
{
    return _dao.regUser(name,email,pass);
}

int MySQLMgr::loginVerify(const std::string& email,const std::string& pass)
{
    return _dao.loginVerify(email,pass); 
}

int MySQLMgr::queryRegisteredUser(const std::string & email)
{
    return _dao.queryRegisteredUser(email);
}

int MySQLMgr::modifyPassword(const std::string& email,const std::string & new_password)
{
    int stat = _dao.modifyPassword(email,new_password);
    if(stat != 0){
        return -1;
    }else{
        return 0;
    }
}

int MySQLMgr::updateUserInfo(int uid,std::string nick_name,int age,SEX sex,std::string icon_pixmap,std::string location,std::string desc_msg)
{
    return _dao.updateUserInfo(uid,nick_name,age,sex,icon_pixmap,location,desc_msg);
}

int MySQLMgr::searchUser(std::string name,Json::Value& ret)
{
    return _dao.searchUser(name,ret);
}

int MySQLMgr::searchUser(int uid,Json::Value& ret)
{
    return _dao.searchUser(uid,ret);
}

int MySQLMgr::addFriendApply(int from_uid,int to_uid,std::string message)
{
    return _dao.addFriendApply(from_uid,to_uid,message);
}

int MySQLMgr::addFriendReply(int from_uid,int to_uid)
{
    return _dao.addFriendReply(from_uid,to_uid);
}

int MySQLMgr::updateApplyInfo(int from_uid,int to_uid,AddFriendStatus stat)
{
    return _dao.updateApplyInfo(from_uid,to_uid,stat);
}

int MySQLMgr::getFriendApplySource(int uid,int offset,int count,ApplyStatusToUser stat,std::vector<std::shared_ptr<ApplyData>>& apply_info)
{
    return _dao.getFriendApplySource(uid,offset,count,stat,std::ref(apply_info));
}