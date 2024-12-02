/*
 * @Author: zbccc zbccc.p@gmail.com
 * @Date: 2024-10-13 14:20:50
 * @LastEditors: zbccc zbccc.p@gmail.com
 * @LastEditTime: 2024-10-31 09:17:01
 * @FilePath: /coding/Server/VerifyServer/server.js
 */
const { _grpc_channelz_v1_ChannelConnectivityState_State } = require("@grpc/grpc-js/build/src/generated/grpc/channelz/v1/ChannelConnectivityState.js");
const email = require("./email.js");
const grpc = require("@grpc/grpc-js");
const const_mod = require("./const.js");
const message_proto = require("./proto.js");
const config_module = require("./config.js");
const redis = require("./redis.js");

function generateAlphaNumericCode(length) {
  const characters = '0123456789';
  let code = '';
  for (let i = 0; i < length; i++) {
    code += characters.charAt(Math.floor(Math.random() * characters.length));
  }
  return code;
}

async function GetVarifyCode(call,callback)
{
    let unique_id = generateAlphaNumericCode(6);
    console.log("email is ",call.request.email);
    try{
        let text_str;
        if(call.request.key == 0){
            text_str = "您的验证码为："+unique_id+",请三分钟内完成注册";
        }else if(call.request.key==1){
            text_str = "您正在请求更改密码，验证码为："+unique_id+",请三分钟内完成验证";
        }
        let mail_option = {
            from:config_module.email_user,
            to:call.request.email,
            subject:"[zbchat]:验证码",
            text:text_str
        };
        let send_res = await email.sendEmail(mail_option);//等待promise完成
        console.log(const_mod.code_prefix+call.request.email);
        redis.setRedisExpire(const_mod.code_prefix+call.request.email,unique_id,181); //在redis中设置验证码生存周期
        callback(null,{email:call.request.email,error:const_mod.Errors.Success,code:unique_id});//写入grpc回复消息
    }catch(error){
        console.log("catch error is ",error);
        callback(null,{email:call.request.email,error:const_mod.Errors.Exception});
    }
}

function main()
{
    var server = new grpc.Server();
    server.addService(message_proto.message_proto.VarifyService.service,{GetVarifyCode:GetVarifyCode});
    server.bindAsync("127.0.0.1:50051",grpc.ServerCredentials.createInsecure(),function(){ //绑定验证码服务器
        console.log("grpc server started");
    });
}

main();