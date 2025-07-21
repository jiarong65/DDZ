#pragma once
#include <string>
#include <sw/redis++/redis++.h>
#include "JsonParse.h"

class Room{
public:
    Room()=default;
    ~Room();
    //初始化 连接redis
    bool initEnvironment();
    //清空数据
    void clear();
    //保存Rsa密钥对
    void saveRsaSecKey(std::string field,std::string value);
    //读密钥
    std::string rsaSecKey(std::string field);
    //加入房间
    std::string joinRoom(std::string userName);
    bool joinRoom(std::string userName,std::string roomName);
    //随机生成房间的名字
    std::string getNewRoomName();
    //获取房间人数
    int getPlayerCount(std::string roomName);
    //存储玩家分数
    void updatePlayerScore(std::string roomName,std::string userName,int score);
    //username->roomname
    std::string whereAmUser(std::string userName);
    //查询玩家分数
    int getPlayerScore(std::string roomName,std::string userName);
    //获取抢地主次序
    std::string playersOrder(std::string roomName);
    //离开房间
    void leaveRoom(std::string roomName,std::string userName);
    //搜索房间
    bool searchRoom(std::string roomName);
private:
    sw::redis::Redis* m_redis=nullptr;
    const std::string OnePlayer="OnePlayer";
    const std::string TwoPlayer="TwoPlayer";
    const std::string ThreePlayer="ThreePlayer";
    const std::string Invalid = "Invalid";
};