/*
 * @Author: zbccc zbccc.p@gmail.com
 * @Date: 2024-10-11 20:01:29
 * @LastEditors: zbccc zbccc.p@gmail.com
 * @LastEditTime: 2024-10-20 16:55:40
 * @FilePath: /coding/Server/VerifyServer/const.js
 */
let code_prefix = "code_";

const Errors = {
    Success : 0,
    Redis : 1,
    Exception : 2
};

module.exports.Errors = Errors;
module.exports.code_prefix = code_prefix;