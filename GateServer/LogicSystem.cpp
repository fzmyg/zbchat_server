/*
 * @Author: zbccc zbccc.p@gmail.com
 * @Date: 2024-10-07 21:21:05
 * @LastEditors: zbccc zbccc.p@gmail.com
 * @LastEditTime: 2024-11-30 08:30:22
 * @FilePath: /coding/gateServer/LogicSystem.cpp
 */
#include "LogicSystem.h"
#include "HttpConnection.h"
#include "Error.h"
#include "message.grpc.pb.h"
#include "message.pb.h"
#include "VarifyGrpcClient.h"
#include "RedisMgr.h"
#include "MySQLMgr.h"
#include "ConfigMgr.h"
#include "LoginError.h"
#include "StatusGrpcClient.h"
#include <filesystem>
#include <boost/beast.hpp>
#include <iostream>
#include <cctype>
#include <sstream>
#include <iomanip>
#include <jsoncpp/json/json.h>

#define VERIFY_CODE_PREFIX "code_"

static const std::string redis_login_set = "login_set";

LogicSystem::LogicSystem()
{
    
    registerHttpGet("/get_text",[](std::shared_ptr<HttpConnection> session){
        boost::beast::ostream(session->_response.body())<<"receive get_test request\r\n";
        for(auto & it:session->_url_params){
            boost::beast::ostream(session->_response.body())<<"key:"<<it.first<<"= val:"<<it.second<<"\r\n";
        }
        return true;
    });

    //获取验证码
    registerHttpPost("/get_varifycode",[](std::shared_ptr<HttpConnection> session){
        std::cout<<"callback:/get_varifycode"<<std::endl;
        std::string req_body_str = boost::beast::buffers_to_string(session->_request.body().data());
        Json::Value src_val;
        Json::Value root;
        Json::Reader reader;
        reader.parse(req_body_str,src_val);
        //if(src_val.isMember("key")==false){
            //std::cerr<<"Faild to parse json file"<<std::endl;
            //root["error"]=ErrorCode::ERROR_JSON;
            //boost::beast::ostream(session->_response.body())<<root.toStyledString();
            //return false;
        //}
        std::string email = src_val["email"].asString();
        int key = src_val["key"].asInt();
        
        int ans = MySQLMgr::getInstance()->queryRegisteredUser(email);
        if(ans == -1 && key == 1)//找回密码但用户不存在
        {
            root["error"]=USER_NOT_EXISTED;
            root["email"]=email;
            root["code"]="null";
            return true;
        }
        if(ans == 0 && key ==0){//注册用户但用户存在
            root["error"]=USER_EXISTED;
            root["email"]=email;
            root["code"]="null";
            return true;
        }
        
        message::GetVarifyRsp rsp = VarifyGrpcClient::getInstance()->getVarifyCode(key,email); //向验证码服务器发送获取验证码请求
        if(rsp.error()==ERR_NETWORK){ 
            std::cerr<<"grpc请求错误"<<std::endl;
            return false; 
        }
        root["error"]=rsp.error();
        root["email"]=rsp.email();
        root["code"]=rsp.code();
        boost::beast::ostream(session->_response.body())<<root.toStyledString();
        return true;
    });

    //注册请求
    registerHttpPost("/user_register",[](std::shared_ptr<HttpConnection> session){
        std::cout<<"callback:/user_register"<<std::endl;
        std::string body_str = boost::beast::buffers_to_string(session->_request.body().data());
        Json::Reader reader;
        Json::Value root;
        reader.parse(body_str,root);
        std::string user = root["user"].asString();
        std::string password = root["password"].asString();
        std::string email = root["email"].asString();
        std::string req_verify_code = root["code"].asString();
        std::string true_verify_code;
        RedisConnectionMgr::ErrorCode status = RedisConnectionMgr::getInstance()->get(std::string(VERIFY_CODE_PREFIX)+email,true_verify_code);
        if(status == RedisConnectionMgr::ErrorCode::NETWORK_ERR||status == RedisConnectionMgr::ErrorCode::STOP){
            std::cerr<<"reids link error"<<std::endl;
            Json::Value error;
            error["error"] = REDIS_ERROR;
            boost::beast::ostream(session->_response.body()) << error.toStyledString();
            return true;
        }
        else if(status==RedisConnectionMgr::ErrorCode::FAILURE){
            std::cerr<<"verify code expired"<<std::endl;
            Json::Value error;
            error["error"] = VARIFY_EXPIRED;
            boost::beast::ostream(session->_response.body()) << error.toStyledString();
            return true;
        }
        if(req_verify_code != true_verify_code){
            Json::Value error;
            error["error"] = VARIFY_CODE_ERROR;
            boost::beast::ostream(session->_response.body()) << error.toStyledString();
            return true;
        }
        Json::Value error;
        /*链接mysql，注入注册数据*/
        int uid = MySQLMgr::getInstance()->regUser(user,email,password);
        if(uid==0 || uid==-1){
            error["error"] = ErrorCode::USER_EXISTED; //用户存在
            std::cerr<<"uid:"<<uid<<"注册失败"<<std::endl;
        }else{
            error["error"] = SUCCESS;
            std::cerr<<"uid:"<<uid<<"注册成功"<<std::endl;
        }
        error["uid"] = uid;
        boost::beast::ostream(session->_response.body()) << error.toStyledString();
        return true;
    });

    registerHttpPost("/user_login",[](std::shared_ptr<HttpConnection> session){
        std::string body_str = boost::beast::buffers_to_string(session->_request.body().data());
        Json::Reader reader;
        Json::Value request_json;
        Json::Value response_json;
        reader.parse(body_str,request_json);
        int stat = MySQLMgr::getInstance()->loginVerify(request_json["email"].asString(),xorEncryptPassword(request_json["password"].asString())); //查询mysql
        if(stat == MYSQL_CONNECT_ERROR){ //网络错误
            response_json["error"] = ErrorCode::ERR_NETWORK;
            response_json["uid"] = -1;
            goto rollback;
        }else if(stat == PASSWORD_NOT_MATCH){ //密码不匹配
            response_json["error"] = ErrorCode::PASSWORD_ERROR;
            response_json["uid"] = -1;
            goto  rollback;
        }else if(stat == USER_NOT_EXIST) { //用户不存在
            response_json["error"] = ErrorCode::USER_NOT_EXISTED;
            response_json["uid"] = -1;
            goto rollback;
        }else{  //密码匹配
            //检查用户是否登录
            std::string login_stat = "";
            RedisConnectionMgr::ErrorCode redis_stat = RedisConnectionMgr::getInstance()->sismember(redis_login_set,request_json["email"].asString());
            if(redis_stat == RedisConnectionMgr::ErrorCode::NETWORK_ERR || redis_stat == RedisConnectionMgr::ErrorCode::STOP){
                response_json["error"]=ErrorCode::ERR_NETWORK;
                goto rollback;
            }
            if(redis_stat == RedisConnectionMgr::ErrorCode::SUCCESS){ //用户已登录
                response_json["error"]=ErrorCode::USER_LOGINED;
                response_json["uid"] = stat;
                goto rollback;
            }else{ //用户未登录
                redis_stat = RedisConnectionMgr::getInstance()->sadd(redis_login_set,request_json["email"].asString());
                if(redis_stat!=RedisConnectionMgr::ErrorCode::SUCCESS){
                    response_json["error"]=ErrorCode::ERR_NETWORK;
                    goto rollback;
                }
                //rpc获取ChatServer信息
                message::GetChatServerRsp rep = StatusGrpcClient::getInstance()->getChatServer(stat);
                if(rep.error()!=ErrorCode::SUCCESS){
                    std::cerr<<"grpc with StatusServer failed"<<std::endl;
                    response_json["error"]=ErrorCode::ERR_NETWORK;
                    do{
                        redis_stat = RedisConnectionMgr::getInstance()->srem(redis_login_set,request_json["email"].asString()); 
                    }while(redis_stat != RedisConnectionMgr::ErrorCode::SUCCESS); //可能存在bug
                    goto rollback;
                }
                response_json["host"]=rep.host();
                response_json["port"]=rep.port();
                response_json["token"]=rep.token();
                response_json["error"]=rep.error();
                response_json["uid"] = stat;
                goto rollback;
            }
        }
rollback:
        boost::beast::ostream(session->_response.body()) << response_json.toStyledString();
        return true;
    });

    registerHttpPost("/modify_password",[](std::shared_ptr<HttpConnection> session){
        std::string body_str = boost::beast::buffers_to_string(session->_request.body().data());
        Json::Reader reader;
        Json::Value request_json;
        Json::Value response_json;
        reader.parse(body_str,request_json);
        std::string email = request_json["email"].asString();
        std::string verify_code = request_json["verify_code"].asString();
        std::string new_pass = request_json["new_password"].asString();
        new_pass = xorEncryptPassword(new_pass);
        std::string true_verifycode;
        //校验验证码
        if(RedisConnectionMgr::getInstance()->get(std::string(VERIFY_CODE_PREFIX)+email,true_verifycode)!=RedisConnectionMgr::ErrorCode::SUCCESS){ //网络问题或验证码失效
            response_json["error"]=VARIFY_EXPIRED;
            boost::beast::ostream(session->_response.body())<<response_json.toStyledString();
            return true;
        }
        if(verify_code != true_verifycode){ //验证码不对
            response_json["error"]=VARIFY_CODE_ERROR;
            boost::beast::ostream(session->_response.body())<<response_json.toStyledString();
            return true;
        }
        //校验验证码通过
        int stat =  MySQLMgr::getInstance()->modifyPassword(email,new_pass);
        if(stat == -1){
            response_json["error"]=ERR_NETWORK;
            boost::beast::ostream(session->_response.body())<<response_json.toStyledString();
            return true;
        }
        response_json["error"]=SUCCESS;
        boost::beast::ostream(session->_response.body())<<response_json.toStyledString();
        return true;
    });
    
}

//注册httpget函数
void LogicSystem::registerHttpGet(std::string url,HttpHandler handler)
{
    this->_get_handlers.insert(std::pair<std::string,HttpHandler>(url,handler));
}

void LogicSystem::registerHttpPost(std::string url,HttpHandler handler)
{
    this->_post_handlers.insert(std::pair<std::string,HttpHandler>(url,handler));
}

//外部调用接口调用http get 函数
bool LogicSystem::handleHttpGet(std::string path,std::shared_ptr<HttpConnection> session)
{
    auto it = this->_get_handlers.find(path);
    if(it==_get_handlers.end()){
        std::cerr<<"can not find: " <<path<<" no such routed path!"<<std::endl;
        return false;
    }
    return it->second(session); //调用处理函数
}

//外部接口调用http post函数
bool LogicSystem::handleHttpPost(std::string path,std::shared_ptr<HttpConnection> session)
{
    std::map<std::string,HttpHandler>::iterator it = _post_handlers.find(path);
    if(it==_post_handlers.end()){
        std::cerr<<"can not find: " <<path<<" no such routed path!"<<std::endl;
        return false;
    }
    return it->second(session); //调用处理函数
}

// 判断字符是否为字符 或 数字
static bool isalnum(unsigned char ch)
{
    return  isalpha(ch) || isdigit(ch);
}

// 将数字转换为16进制字符
static std::string toHexString(unsigned char ch) {
    std::ostringstream oss;
    oss << std::uppercase << std::hex << std::setw(2) << std::setfill('0') << (int)ch;
    return oss.str();
}

//将字符编码为url格式
std::string encodeUrl(const std::string & str)
{
    std::string temp_str = "";
    std::size_t str_len = str.size();
    for(std::size_t i = 0;i<str_len;i++){
        if(isalnum(str[i]) || str[i]=='-' || str[i] == '_' || str[i]=='.' || str[i]=='~'){ //若为字母，数字,'-','_','.','~'等特殊字符 不需要编码
            temp_str+=str[i];
        }else if(str[i]==' '){//为' ' 添加 '+'
            temp_str += '+';
        }else{ //特殊字符将每个字节的值编为字符串格式
            temp_str += '%';
            temp_str += toHexString(str[i]); 
        }
    }
    return temp_str;
}

//将16进制字符转换为10进制数
static unsigned char fromHexChar(unsigned char ch){
    if(ch>='0'&&ch<='9'){
        return ch-'0';
    }else{
        return ch-'A'+10;
    }
}

//解码url字符串
std::string decodeUrl(const std::string & str)
{
    std::string ans;
    int size = str.size();
    for(int i = 0;i<size;i++){
        if(isalnum((unsigned char)str[i])){ 
            ans += str[i];
        }else if(str[i]=='+'){
            ans += " ";
        }else{
            assert(i+2<size);
            unsigned char ch = (fromHexChar(str[i+1])<<4) | (fromHexChar(str[i+2]));
            ans += ch;
            if(i+3==size) break;
        }
    }
    return ans;
}

std::string xorEncryptPassword(const std::string &str)
{
    char len = static_cast<char>(str.size());
    std::string ret (str);
    for(int i = 0;i<len;i++)
    {
        ret[i] = ret[i] ^ len;
    }
    return ret;
}