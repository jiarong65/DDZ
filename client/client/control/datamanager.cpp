#include "datamanager.h"

DataManager* DataManager::m_data=new DataManager;

DataManager *DataManager::getInstance()
{
    return m_data;
}

void DataManager::setUserName(QByteArray name)
{
    m_userName=name;
}

void DataManager::setIp(QByteArray ip)
{
    m_ip=ip;
}

void DataManager::setPort(QByteArray port)
{
    m_port=port;
}

void DataManager::setCommun(Communication *commun)
{
    m_commun=commun;
}

void DataManager::setRoomName(QByteArray roomName)
{
    m_roomName=roomName;
}

void DataManager::setCards(Cards cs, Cards last)
{
    m_cs=cs;
    m_last=last;
}

void DataManager::setGameMode(DataManager::GameMode mode)
{
    m_mode=mode;
}

void DataManager::setRoomMode(DataManager::RoomMode mode)
{
    m_roomMode=mode;
}

QByteArray DataManager::getUserName()
{
    return m_userName;
}

QByteArray DataManager::getIp()
{
    return m_ip;
}

QByteArray DataManager::getPort()
{
    return m_port;
}

QByteArray DataManager::getRoomName()
{
    return m_roomName;
}

DataManager::GameMode DataManager::getGameMode()
{
    return m_mode;
}

DataManager::RoomMode DataManager::getRoomMode()
{
    return m_roomMode;
}

bool DataManager::isNetMode()
{
    return m_mode==DataManager::Network?true:false;
}

bool DataManager::isAutoMode()
{
    return m_roomMode==DataManager::Auto?true:false;
}

Communication *DataManager::getCommun()
{
    return m_commun;
}

Cards DataManager::getCards()
{
    return m_cs;
}

Cards DataManager::getLastCards()
{
    return m_last;
}
