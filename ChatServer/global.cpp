/*
 * @Author: zbccc zbccc.p@gmail.com
 * @Date: 2024-11-28 10:37:45
 * @LastEditors: zbccc zbccc.p@gmail.com
 * @LastEditTime: 2024-11-28 23:44:51
 * @FilePath: /coding/Server/ChatServer/global.cpp
 */
#include "global.h"
#include <fstream>

std::string login_server = "login_server_";

std::string login_set = "login_set";

static boost::uuids::random_generator random_generator;

std::string generateUUid()
{
    boost::uuids::uuid uuid = random_generator();
    return boost::uuids::to_string(uuid);
}

std::string readFile(const std::string& path)
{
    std::ifstream ifs(path,std::ios::in);
    if(!ifs){
        return "";
    }
    return std::string(std::istreambuf_iterator<char>(ifs),std::istreambuf_iterator<char>());
}