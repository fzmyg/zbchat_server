/*
 * @Author: zbccc zbccc.p@gmail.com
 * @Date: 2024-10-17 19:17:07
 * @LastEditors: zbccc zbccc.p@gmail.com
 * @LastEditTime: 2024-11-03 16:28:10
 * @FilePath: /coding/Server/gateServer/IOContextPool.cpp
 */
#include "IOContextPool.h"
#include <iostream>
#include <thread>
IOContextPool::~IOContextPool()
{
    if(_stop==false)
        stop();
}

IOContextPool::IOContextPool(std::size_t size)
:_services(size),_works(size),_next_io_service(0)
{
    for(int i = 0 ;i<size; i++){
        _works.emplace_back( new Work(_services[i]));
        _thread_pool.emplace_back([this,i](){
            std::cout<<"io service thread "<<std::this_thread::get_id()<<"start successfully"<<std::endl;
            this->_services[i].run();
        });
    }
}

void IOContextPool::stop()
{
    _stop = true;
    for(auto & work:_works){
        //work->get_io_context().stop();
        work.reset();
    }
    std::cerr<<"等待未完成的异步请求"<<std::endl;
    for(auto & thread:_thread_pool){
        thread.join();
    }
    std::cerr<<"异步服务停止"<<std::endl;
}

IOContextPool::IOService& IOContextPool::getIOService()
{
    std::size_t index = _next_io_service;
    _next_io_service = (_next_io_service++)%_services.size();
    return _services[index];
}