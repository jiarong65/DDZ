#include "Room.h"
#include "Log.h"
#include <random>
#include <optional>
#include <vector>
#include <utility>

Room::~Room()
{
    if(m_redis){
        delete m_redis;
    }
}

bool Room::initEnvironment()
{
    JsonParse json;
    auto info=json.getDatebaseInfo(JsonParse::DBType::Redis);
    std::string conn="tcp://"+info->ip+":"+std::to_string(info->port);
    m_redis=new sw::redis::Redis(conn);
    if(m_redis->ping()=="PONG"){
        Debug("成功连接redis服务器...");
        return true;
    }
    Debug("连接redis服务器失败...");
    return false;
}

void Room::clear()
{
    m_redis->flushdb();
}

void Room::saveRsaSecKey(std::string field, std::string value)
{
    m_redis->hset("RSA",field,value);
}

std::string Room::rsaSecKey(std::string field)
{
    auto value=m_redis->hget("RSA",field);
    if(value.has_value()){
        return value.value();
    }
    return std::string();
}

std::string Room::joinRoom(std::string userName)
{
    std::optional<std::string> room;
    if(m_redis->scard(TwoPlayer)>0){
        room=m_redis->srandmember(TwoPlayer);
    }else if(m_redis->scard(OnePlayer)>0){
        room=m_redis->srandmember(OnePlayer);
    }else{
        room=getNewRoomName();
    }
    Debug("room: %s", room.value().c_str());
    joinRoom(userName,room.value());
    return room.value();
}

std::string Room::getNewRoomName()
{
    // 创建随机设备对象
    std::random_device rd;
    // 创建随机数生成对象
    std::mt19937 gen(rd());
    // 创建随机数分布对象 -> 均匀分布
    std::uniform_int_distribution<int> dis(100000, 999999);
    int randNum = dis(gen);
    Debug("Generated new room name: %s", std::to_string(randNum).c_str());

    return std::to_string(randNum);
}

bool Room::joinRoom(std::string userName, std::string roomName)
{
    Debug("joinRoom roomname:%s",roomName.c_str());
    if(m_redis->zcard(roomName)>=3){
        return false;
    }
    if(!m_redis->exists(roomName)){
        m_redis->sadd(OnePlayer,roomName);
    }else if(m_redis->sismember(OnePlayer,roomName)){
        m_redis->smove(OnePlayer,TwoPlayer,roomName);
    }else if(m_redis->sismember(TwoPlayer,roomName)){
        m_redis->smove(TwoPlayer,ThreePlayer,roomName);
    }else{
        assert(false);
    }
    m_redis->zadd(roomName,userName,0);
    m_redis->hset("Players",userName,roomName);
    return true;
}

int Room::getPlayerCount(std::string roomName)
{
    return m_redis->zcard(roomName);
}

void Room::updatePlayerScore(std::string roomName, std::string userName, int score)
{
    m_redis->zadd(roomName,userName,score);
}

std::string Room::whereAmUser(std::string userName)
{
    auto roomName=m_redis->hget("Players",userName);
    if(roomName.has_value()){
        return roomName.value();
    }
    return std::string();
}

int Room::getPlayerScore(std::string roomName, std::string userName)
{
    auto score= m_redis->zscore(roomName,userName);
    if(score.has_value()){
        return score.value();
    }
    return 0;
}

std::string Room::playersOrder(std::string roomName)
{
    int index=0;
    std::string data;
    std::vector<std::pair<std::string,double>> output;
    m_redis->zrevrange(roomName,0,-1,std::back_inserter(output));
    for(auto& it : output){
        data+=it.first+"-"+std::to_string(++index)+"-"+std::to_string((int)it.second)+"#";
    }
    return data;
}

void Room::leaveRoom(std::string roomName, std::string userName)
{
    if(m_redis->sismember(ThreePlayer,roomName)){
        m_redis->smove(ThreePlayer,Invalid,roomName);
    }
    m_redis->zrem(roomName,userName);
    auto count=m_redis->zcard(roomName);
    if(count==0){
        m_redis->del(roomName);
        m_redis->srem(Invalid,roomName);
    }
}

bool Room::searchRoom(std::string roomName)
{
    //搜索二人间
    bool flag=m_redis->sismember(TwoPlayer,roomName);
    //搜索一人间
    if(!flag){
        flag=m_redis->sismember(OnePlayer,roomName);
    }
    return flag;
}
