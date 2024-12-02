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