/*
 * @Author: zbccc zbccc.p@gmail.com
 * @Date: 2024-10-07 21:12:57
 * @LastEditors: zbccc zbccc.p@gmail.com
 * @LastEditTime: 2024-11-30 09:49:27
 * @FilePath: /coding/gateServer/Singleton.hpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#pragma once

#include <memory>
#include <mutex>
template<typename T>
class Singleton{
public:
    template<typename... Args>
    static std::shared_ptr<T> getInstance(Args&&... args){
        std::call_once(_of,[&]()mutable{
            _instance.reset(new T(std::forward<Args>(args)...));
        });
        return _instance;
    }

protected:
    static std::shared_ptr<T> _instance;
private:
    static std::once_flag _of;
};

template<typename T>
std::shared_ptr<T> Singleton<T>::_instance = nullptr;

template<typename T>
std::once_flag Singleton<T>::_of;