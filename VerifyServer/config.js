/*
 * @Author: zbccc zbccc.p@gmail.com
 * @Date: 2024-10-11 20:04:07
 * @LastEditors: zbccc zbccc.p@gmail.com
 * @LastEditTime: 2024-10-11 20:18:42
 * @FilePath: /coding/Server/VerifyServer/config.js
 */
const fs = require("fs");

let config_json_data = JSON.parse(fs.readFileSync("config.json","utf-8"));
let email_user = config_json_data.email.user;
let email_pass = config_json_data.email.pass;
let mysql_host = config_json_data.mysql.host;
let mysql_port = config_json_data.mysql.port;
let mysql_pass = config_json_data.mysql.data;
let redis_host = config_json_data.redis.host;
let redis_port = config_json_data.redis.port;
let redis_pass = config_json_data.redis.pass;
let code_prefix = "code_";

module.exports = {
    email_user,email_pass,mysql_host,mysql_port,mysql_pass,redis_host,redis_port,redis_pass,code_prefix
};