/*
 * @Author: zbccc zbccc.p@gmail.com
 * @Date: 2024-10-13 10:09:38
 * @LastEditors: zbccc zbccc.p@gmail.com
 * @LastEditTime: 2024-10-17 12:35:46
 * @FilePath: /coding/Server/VerifyServer/email.js
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
const config = require("./config.js");

const proto = require("./proto.js");

const nodemailer = require("nodemailer");

/**
 *  创建发送邮件的代理
 */

let transport = nodemailer.createTransport({
    host:"smtp.163.com", //qq smtp服务器地址
    port:465,           //ssl端口
    secure:true,        //安全模式
    auth:{
        user: config.email_user, //
        pass: config.email_pass
    }
});

var sendEmail = function(mail_option){
    return new Promise(function(resolve,reject){ //类似于c++future
        transport.sendMail(mail_option,function(error,info){
            if(error){
                console.log("邮件已成功发送");
                console.log(error);
                reject(error); //返回错误信息 由catch接收
            }else{
                console.log("邮件已成功发送"+info.response); 
                resolve(info.response); //返回正确信息，由then接收
            }
        });
    });
};

module.exports.sendEmail = sendEmail;