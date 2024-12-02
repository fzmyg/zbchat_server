/*
 * @Author: zbccc zbccc.p@gmail.com
 * @Date: 2024-10-10 21:32:25
 * @LastEditors: zbccc zbccc.p@gmail.com
 * @LastEditTime: 2024-10-13 15:39:56
 * @FilePath: /coding/VerifyServer/proto.js
 */
const path = require("path");
const grpc = require("@grpc/grpc-js");
const protoLoader = require("@grpc/proto-loader");

const PROTO_PATH = path.join(__dirname,"message.proto");
const pakage_definition = protoLoader.loadSync(PROTO_PATH,{keepCase:true/*保持大小写区分*/,longs:String/*防止浮点数溢出*/,enums:String/*枚举类型转换为string*/,defaults:true,oneofs:true/**/});
const proto_descriptor = grpc.loadPackageDefinition(pakage_definition);

const message_proto = proto_descriptor.message;

module.exports.message_proto = message_proto;