/*
 * @Author: zbccc zbccc.p@gmail.com
 * @Date: 2024-11-04 15:05:14
 * @LastEditors: zbccc zbccc.p@gmail.com
 * @LastEditTime: 2024-11-04 15:06:45
 * @FilePath: /coding/Server/ChatServer/log.cpp
 */
#include "log.h"
#include <iostream>
std::mutex screen_mux;
void consoleLog(const std::string &str)
{
    std::unique_lock<std::mutex> lock(screen_mux);
    std::cerr<<str<<std::endl;
}