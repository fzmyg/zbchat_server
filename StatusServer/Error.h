/*
 * @Author: zbccc zbccc.p@gmail.com
 * @Date: 2024-10-09 09:11:49
 * @LastEditors: zbccc zbccc.p@gmail.com
 * @LastEditTime: 2024-11-05 20:05:21
 * @FilePath: /coding/gateServer/error.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#pragma once

enum  ErrorCode{
    SUCCESS = 0,    //成功
    ERROR_JSON = 1, //JSON解析错误
    ERR_NETWORK = 2,  //rpc调用错误
    RPC_FAILED = 2,   //rpc调用错误
    REDIS_ERROR = 2,  //redis错误
    CONNECT_REFUSE = 2, //拒绝连接
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
};