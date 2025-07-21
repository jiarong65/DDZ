#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <QByteArray>
#include "communication.h"
#include "cards.h"

//饿汉模式
class DataManager
{
public:
    enum GameMode{Single,Network};
    enum RoomMode{Auto,Manual};
    DataManager(const DataManager&)=delete ;
    DataManager& operator=(const DataManager&)=delete ;
    DataManager(DataManager&&)=delete ;
    DataManager& operator=(DataManager&&)=delete;
    static DataManager* getInstance();
    void setUserName(QByteArray name);
    void setIp(QByteArray ip);
    void setPort(QByteArray port);
    void setCommun(Communication* commun);
    void setRoomName(QByteArray roomName);
    void setCards(Cards cs,Cards last);
    void setGameMode(GameMode mode);
    void setRoomMode(RoomMode mode);
    QByteArray getUserName();
    QByteArray getIp();
    QByteArray getPort();
    QByteArray getRoomName();
    GameMode getGameMode();
    RoomMode getRoomMode();
    bool isNetMode();
    bool isAutoMode();
    Communication* getCommun();
    Cards getCards();
    Cards getLastCards();
private:
    DataManager()=default;
    static DataManager* m_data;

    QByteArray m_userName;
    QByteArray m_ip="192.168.233.128";
    QByteArray m_port="10000";
    QByteArray m_roomName;
    Communication *m_commun;
    Cards m_cs,m_last;
    GameMode m_mode;
    RoomMode m_roomMode;
};

#endif // DATAMANAGER_H
