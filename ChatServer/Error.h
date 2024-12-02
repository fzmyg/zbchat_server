/*
 * @Author: zbccc zbccc.p@gmail.com
 * @Date: 2024-11-04 19:05:18
 * @LastEditors: zbccc zbccc.p@gmail.com
 * @LastEditTime: 2024-11-29 13:31:59
 * @FilePath: /coding/Server/ChatServer/Error.h
 */
#pragma once
enum  ErrorCode{
    SUCCESS = 0,    //成功
    ERROR_JSON = 1, //JSON解析错误
    ERR_NETWORK = 2,  //网络错误
    RPC_FAILED = 2,   //rpc调用错误
    REDIS_ERROR = 2,  //redis错误
    VARIFY_EXPIRED = 3, //验证码过期
    VARIFY_CODE_ERROR = 4, //验证码错误
    USER_EXISTED = 5, //用户存在
    USER_NOT_EXISTED = 6,
    PASSWORD_ERROR = 7, //密码不匹配
    EMAIL_NOT_MATCH = 8, //邮箱不匹配
    USER_LOGINED = 9,    //用户已登录
    USER_NOT_LOGINED = 10, //用户未登录
    TOKEN_NOT_MATCH = 11,
    TOKEN_NOT_FIND = 12,
    PIXMAP_NOT_FIND = 13,
    REPEAT_APPLY = 14
};