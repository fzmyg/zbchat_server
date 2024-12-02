/*
 * @Author: zbccc zbccc.p@gmail.com
 * @Date: 2024-11-01 18:05:53
 * @LastEditors: zbccc zbccc.p@gmail.com
 * @LastEditTime: 2024-11-01 18:14:14
 * @FilePath: /coding/Server/StatusServer/ConfigMgr.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#pragma once
#include "Singleton.hpp"
#include <map>
#include <string>
class ConfigSection{
public:
    ~ConfigSection();
    ConfigSection();
    ConfigSection(const ConfigSection&con_sect);
    ConfigSection(ConfigSection&&con_sect);
    ConfigSection& operator=(const ConfigSection&con_sect);
    std::string operator[](const std::string key);
    void insert(const std::string &key,const std::string& val);

private:
    std::map<std::string,std::string> _config_list;
};

class ConfigMgr
:public Singleton<ConfigMgr>{
    friend class Singleton<ConfigMgr>;
public:
    ~ConfigMgr();
    ConfigSection operator[](const std::string key);
    void readIniFile(std::string path);
private: 
    explicit ConfigMgr();
    ConfigMgr(const ConfigMgr&)=delete;
    ConfigMgr& operator = (const ConfigMgr&)=delete;
private:
    std::map<std::string,ConfigSection> _section_list;
};