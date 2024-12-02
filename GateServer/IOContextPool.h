/*
 * @Author: zbccc zbccc.p@gmail.com
 * @Date: 2024-10-17 19:16:58
 * @LastEditors: zbccc zbccc.p@gmail.com
 * @LastEditTime: 2024-10-18 10:50:03
 * @FilePath: /coding/Server/gateServer/IOContextPool.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#pragma once
#include "Singleton.hpp"
#include <boost/asio.hpp>
#include <memory>
#include <vector>
class IOContextPool
:public Singleton<IOContextPool>
{
    friend Singleton<IOContextPool>;
protected:
    IOContextPool(std::size_t size = 2);
    IOContextPool(const IOContextPool&)=delete;
    void operator = (const IOContextPool&) = delete;
public:
    using IOService = boost::asio::io_context;
    using Work = boost::asio::io_context::work;
    using WorkPtr = std::unique_ptr<Work>;
    IOService & getIOService();
    void stop();
    ~IOContextPool();
private:
    std::vector<IOService> _services;
    std::vector<WorkPtr> _works;
    std::vector<std::thread> _thread_pool;
    std::size_t _next_io_service;
    bool _stop = false;
};