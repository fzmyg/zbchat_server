/*
 * @Author: zbccc zbccc.p@gmail.com
 * @Date: 2024-11-04 15:03:02
 * @LastEditors: zbccc zbccc.p@gmail.com
 * @LastEditTime: 2024-11-04 15:30:08
 * @FilePath: /coding/Server/ChatServer/debug.h
 */
#pragma once
#include <mutex>
#include <iostream>
extern std::mutex screen_mux;
template<typename... Args>
void consoleLog(const Args&... args)
{
    std::unique_lock<std::mutex> lock(screen_mux);
    (std::cout<<...<<args)<<std::endl;;
}