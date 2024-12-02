/*
 * @Author: zbccc zbccc.p@gmail.com
 * @Date: 2024-11-28 10:37:34
 * @LastEditors: zbccc zbccc.p@gmail.com
 * @LastEditTime: 2024-11-28 23:44:59
 * @FilePath: /coding/Server/ChatServer/global.h
 */
#pragma once
#include <boost/uuid.hpp>

extern std::string login_server;

extern std::string login_set;

extern std::string generateUUid();

extern std::string readFile(const std::string& path);