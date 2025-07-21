#include "codec.h"

Codec::Codec(Message *msg)
{
    reload(msg);
}

Codec::Codec(std::string msg)
{
    reload(msg);
}

std::string Codec::encodeMsg()
{
    m_obj.SerializeToString(&m_msg);
    return m_msg;
}

std::shared_ptr<Message> Codec::decodeMsg()
{
    std::string data=m_msg;
    m_obj.ParseFromString(data);
    Message* msg=new Message;
    msg->userName=m_obj.username();
    msg->roomName=m_obj.roomname();
    msg->data1=m_obj.data1();
    msg->data2=m_obj.data2();
    msg->data3=m_obj.data3();
    msg->reqCode=m_obj.reqcode();
    msg->resCode=m_obj.rescode();

    std::shared_ptr<Message> ptr(msg,[this](Message* pt){
        delete pt;
    });
    return ptr;
}

void Codec::reload(std::string msg)
{
    m_msg=msg;
}

void Codec::reload(Message *msg)
{
    m_obj.set_username(msg->userName);
    m_obj.set_roomname(msg->roomName);
    m_obj.set_data1(msg->data1);
    m_obj.set_data2(msg->data2);
    m_obj.set_data3(msg->data3);
    m_obj.set_reqcode(msg->reqCode);
    m_obj.set_rescode(msg->resCode);
}

Codec::~Codec(){

}