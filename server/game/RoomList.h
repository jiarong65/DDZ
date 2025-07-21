#pragma once
#include <string>
#include <mutex>
#include <functional>
#include <map>
#include <utility>

using callback=std::function<void(std::string)>;
using UserMap=std::map<std::string,callback>;//用户名，通信函数
//懒汉单例
class RoomList{
public:
    static RoomList* getIntance();
    void addUser(std::string roomName,std::string userName,callback sendMessage);
    //获取房间成员信息
    UserMap getPlayers(std::string roomName);
    UserMap getPartners(std::string roomName,std::string userName);
    //删除房间中指定玩家
    void removePlayer(std::string roomName,std::string userName);
    //清空房间内所有玩家
    void removeRoom(std::string roomName);

    RoomList(const RoomList&)=delete;
    RoomList& operator=(const RoomList&)=delete;
    RoomList(const RoomList&&)=delete;
    RoomList& operator=(const RoomList&&)=delete;
private:
    RoomList()=default;
    std::map<std::string,UserMap> m_roomMap;
    std::mutex m_mutex;
};