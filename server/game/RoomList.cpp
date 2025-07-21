#include "RoomList.h"
#include "Log.h"

RoomList* RoomList::getIntance()
{
    static RoomList obj;
    return &obj;
}

void RoomList::addUser(std::string roomName, std::string userName, callback sendMessage)
{
    Debug("addUser => roomName: [%s], userName: [%s]", roomName.c_str(), userName.c_str());

    std::lock_guard<std::mutex> lock(m_mutex);
    if(m_roomMap.find(roomName)!=m_roomMap.end()){
        auto& value=m_roomMap[roomName];
        value.insert(make_pair(userName,sendMessage));
    }else{
        UserMap value={make_pair(userName,sendMessage)};
        m_roomMap.insert(make_pair(roomName,value));
    }
}

UserMap RoomList::getPlayers(std::string roomName)
{   
    Debug("getPlayers => roomName: [%s]", roomName.c_str());

    std::lock_guard<std::mutex> lock(m_mutex);
    auto item=m_roomMap.find(roomName);
    if(m_roomMap.end()==item){
        Debug("room为空");
        return UserMap();
    }
    return item->second;
}

UserMap RoomList::getPartners(std::string roomName,std::string userName)
{
    auto players=getPlayers(roomName);
    if(players.size()>1){
        auto item=players.find(userName);
        if(item!=players.end()){
            players.erase(item);
            return players;
        }
    }
    Debug("getPartners 为空");
    return UserMap();
}

void RoomList::removePlayer(std::string roomName, std::string userName)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    auto item=m_roomMap.find(roomName);
    if(item!=m_roomMap.end()){
        auto players=item->second;
        auto it=players.find(userName);
        if(it!=players.end()){
            if(players.size()>1){
                m_roomMap[roomName].erase(it);
            }else if(players.size()==1){
                m_roomMap.erase(item);
            }
        }
    }
}

void RoomList::removeRoom(std::string roomName)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    auto item=m_roomMap.find(roomName);
    if(item!=m_roomMap.end()){
        m_roomMap.erase(item);
    }
}
