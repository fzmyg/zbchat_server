/*
 * @Author: zbccc zbccc.p@gmail.com
 * @Date: 2024-11-26 10:02:43
 * @LastEditors: zbccc zbccc.p@gmail.com
 * @LastEditTime: 2024-11-26 10:03:55
 * @FilePath: /coding/Server/ChatServer/Defer.hpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#pragma once
#include <functional>

class Defer{
public:
    Defer(std::function<void()> func):_func(func){};
    ~Defer(){_func();}
private:
    std::function<void()> _func;
};