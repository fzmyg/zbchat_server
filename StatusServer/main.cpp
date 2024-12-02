/*
 * @Author: zbccc zbccc.p@gmail.com
 * @Date: 2024-11-01 18:00:53
 * @LastEditors: zbccc zbccc.p@gmail.com
 * @LastEditTime: 2024-11-02 23:54:26
 * @FilePath: /coding/Server/StatusServer/main.cpp
 */
#include "StatusServer.h"
#include <iostream>
#include <boost/asio.hpp>

int main(int argc,char**argv)
{
    try{
        StatusServer status_server; //启动状态服务
    }catch(std::exception&e){
        std::cerr<<e.what()<<std::endl;
    }
    return 0;
}