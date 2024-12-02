/*
 * @Author: zbccc zbccc.p@gmail.com
 * @Date: 2024-11-01 23:01:01
 * @LastEditors: zbccc zbccc.p@gmail.com
 * @LastEditTime: 2024-11-04 15:24:26
 * @FilePath: /coding/Server/ChatServer/AsioIOServicePool.cpp
 */
#include "AsioIOServicePool.h"
#include "log.h"
#include <iostream>

AsioIOServicePool::~AsioIOServicePool()
{
    if(_stop==false)
        stop();
}

AsioIOServicePool::AsioIOServicePool(std::size_t size)
:_size(size),_index(0),_io_services(size)
{
    for(int i = 0 ;i < size;i++){
        _works.emplace_back(new Work(_io_services[i]));
        _threads.emplace_back(new std::thread([this,i](){
            
            consoleLog("io service thread ",std::this_thread::get_id()," start successfully");
            this->_io_services[i].run();
            consoleLog("io service thread ",std::this_thread::get_id()," exit");
        }));
    }
}

void AsioIOServicePool::stop()
{
    _stop = true;
    for(auto & it:_works){
        it.reset(); //删除占位work
    }
    consoleLog("等待未完成的异步请求");
    for(auto & thread:_threads){
        if(thread->joinable())
            thread->join();
    }
    consoleLog("异步服务停止");
}

AsioIOServicePool::IOService& AsioIOServicePool::getIOService()
{
    std::size_t index = _index;
    _index = (_index + 1 )%_size; 
    return _io_services[index];
}