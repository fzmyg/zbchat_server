/*
 * @Author: zbccc zbccc.p@gmail.com
 * @Date: 2024-10-19 22:12:45
 * @LastEditors: zbccc zbccc.p@gmail.com
 * @LastEditTime: 2024-10-20 09:15:08
 * @FilePath: /coding/Server/VerifyServer/redis.js
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
const config = require("./config.js");

const redis = require("ioredis");

const redis_client = new redis({host:config.redis_host,port:config.redis_port,password:config.redis_pass});

redis_client.on("error",function(err){
    console.log("Redis connect error");
    redis_client.quit();
});
/*  */
async function getRedis(key){
    try{
        const result = await redis_client.get(key);
        if(result===null){
            console.log("result:<"+result+">,This key cannot be find...");
            return null;
        }
        console.log("result:<"+result+">, Get key successfully");
        return result;
    }catch(error){
        console.log("get_redis error:"+result);
        return null;
    }
}
/*  */
async function queryRedis(key){
    try{
        const result = await redis_client.exists(key);
        if(result===0){
            console.log("result:<"+result+">,This key is null...");
            return null;
        }
        console.log("result:<"+result+">, With the value");
        return result;
    }catch(err){
        console.log("queryRedis error:",error);
        return null;
    }
}
/*  */
async function setRedisExpire(key,val,time)
{
    try{
        await redis_client.set(key,val);
        await redis_client.expire(key,time);
        return true;
    }catch(err){
        console.log("setRedisExpire Error\n",err);
        return false;
    }
}

function close()
{
    redis_client.close();
}

module.exports={
    getRedis:getRedis,
    queryRedis:queryRedis,
    close:close,
    setRedisExpire:setRedisExpire
};
