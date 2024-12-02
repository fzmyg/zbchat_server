#include "TransferKits.h"
std::string xorEncryptPassword(const std::string &str)
{
    char len = static_cast<char>(str.size());
    std::string ret (str);
    for(int i = 0;i<len;i++)
    {
        ret[i] = ret[i] ^ len;
    }
    return ret;
}