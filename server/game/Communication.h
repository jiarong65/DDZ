#pragma once
#include "codec.h"
#include <string>
#include "Buffer.h"
#include "AesCrypto.h"
#include "MySqlConn.h"
#include "Room.h"
#include <map>
#include "RoomList.h"

class Communication{
public:
    Communication();
    ~Communication();
    //设置回调
    using sendCallBack=function<void(std::string)>;
    using deleteCallBack=function<void()>;
    void setCallBack(sendCallBack func1,deleteCallBack func2);
    //解析客户端发来的数据
    void parseRequest(Buffer *buf);
    void handleAesFenfa(Message* reqMsg,Message& resMsg);
    void handleRegister(Message* reqMsg,Message& resMsg);
    void handleLogin(Message* reqMsg,Message& resMsg);
    void handleAddRoom(Message* reqMsg,Message& resMsg);
    void handleGameOver(Message* reqMsg);
    void handleSearchRoom(Message* reqMsg,Message& resMsg);
    void handleLeaveRoom(Message* reqMsg,Message& resMsg);
    void handleGoodBye(Message* reqMsg);
    //数据转发
    void notifyOtherPlayers(std::string data,std::string roomName,std::string userName);
    //准备开始游戏
    void readyForPlay(std::string roomName,std::string data);
    //开始游戏
    void startGame(std::string roomName,UserMap players);
    //重新开始游戏
    void restartGame(Message* reqMsg);
    //发牌
    void dealCards(UserMap players);
    //洗牌
    void initCards();
    //随机取出一张牌
    std::pair<int,int> takeOneCrad();
private:
    sendCallBack sendMessage;
    deleteCallBack disConnect;
    AesCrypto* m_aes=nullptr;
    MySqlConn* m_mysql=nullptr;
    Room* m_redis=nullptr;
    std::multimap<int,int> m_cards;
};