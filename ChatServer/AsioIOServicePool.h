/*
 * @Author: zbccc zbccc.p@gmail.com
 * @Date: 2024-11-01 23:00:45
 * @LastEditors: zbccc zbccc.p@gmail.com
 * @LastEditTime: 2024-11-03 16:12:02
 * @FilePath: /coding/Server/ChatServer/AsioIOServicePool.h
 */
#pragma once
#include "Singleton.hpp"
#include <boost/asio.hpp>
#include <memory>
#include <vector>
#include <atomic>
class AsioIOServicePool
:public Singleton<AsioIOServicePool>
{
    friend Singleton<AsioIOServicePool>;
public:
    using IOService = boost::asio::io_context;
    using Work = boost::asio::io_context::work;
    using ThreadPtr = std::unique_ptr<std::thread>;
    using WorkPtr = std::unique_ptr<Work>;
    AsioIOServicePool(const AsioIOServicePool&service)=delete;
    AsioIOServicePool& operator=(const AsioIOServicePool& service)=delete;
    ~AsioIOServicePool();
    IOService& getIOService();
    void stop();
private:
    AsioIOServicePool(std::size_t size = std::thread::hardware_concurrency());
    std::vector<IOService> _io_services;
    std::vector<WorkPtr> _works;
    std::vector<ThreadPtr> _threads;
    std::size_t _index;
    std::size_t _size;
    std::atomic<bool> _stop;
};