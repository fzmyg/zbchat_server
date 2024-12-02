/*
 * @Author: zbccc zbccc.p@gmail.com
 * @Date: 2024-10-10 16:45:02
 * @LastEditors: zbccc zbccc.p@gmail.com
 * @LastEditTime: 2024-10-10 19:30:27
 * @FilePath: /coding/gateServer/ConfigMgr.cpp
 */
#include "ConfigMgr.h"
#include <boost/filesystem.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

ConfigSection::ConfigSection()
{

}

ConfigSection::~ConfigSection()
{
    this->_config_list.clear();
}

ConfigSection::ConfigSection(const ConfigSection &con_sect)
{
    if(&con_sect == this){
        return;
    }
    this->_config_list = con_sect._config_list;
}

ConfigSection &ConfigSection::operator=(const ConfigSection &con_sect)
{
    if(&con_sect==this){
        return *this;
    }
    this->_config_list = con_sect._config_list;
    return *this;
}

std::string ConfigSection::operator[](const std::string key)
{
    auto it = this->_config_list.find(key);
    if(it==this->_config_list.end()){
        return "";
    }
    return it->second;
}

ConfigSection::ConfigSection(ConfigSection&&con_sect)
:_config_list(std::move(con_sect._config_list))
{
}

void ConfigSection::insert(const std::string &key,const std::string& val)
{
    this->_config_list.insert(std::pair<std::string,std::string>(key,val));
}

void ConfigMgr::readIniFile(std::string path)
{
    boost::property_tree::ptree pt;
    boost::property_tree::read_ini(path,pt);
    for(auto &it:pt){
        const boost::property_tree::ptree & sub_pt = it.second;
        ConfigSection sec;
        for(auto & sub_it:sub_pt){
            sec.insert(sub_it.first,sub_it.second.get_value<std::string>(sub_it.first));
        }
        this->_section_list.insert(std::pair<std::string,ConfigSection>(it.first,sec));
    }
}

ConfigMgr::ConfigMgr()
{
    boost::filesystem::path current_path = boost::filesystem::current_path();
    boost::filesystem::path config_path = current_path / "config.ini";
    readIniFile(config_path.string());
}

ConfigMgr::~ConfigMgr()
{
    this->_section_list.clear();
}

ConfigSection ConfigMgr::operator[](const std::string key)
{
    auto it = this->_section_list.find(key);
    if(it==this->_section_list.end()){
        return ConfigSection();
    }
    return it->second;
}   