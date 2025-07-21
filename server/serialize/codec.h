#ifndef CODEC_H
#define CODEC_H

#include <string>
#include <memory>
#include "infomation.pb.h"

struct Message{
    std::string userName;
    std::string roomName;
    std::string data1;
    std::string data2;
    std::string data3;
    RequestCode reqCode;
    RespondCode resCode;
};

class Codec
{
public:
    //序列化
    Codec(Message* msg);
    //反序列化
    Codec(std::string msg);
    //数据编码
    std::string encodeMsg();
    //数据解码
    std::shared_ptr<Message> decodeMsg();
    //重新加载数据
    void reload(std::string msg);
    void reload(Message* msg);
    ~Codec();
private:
    std::string m_msg;
    Infomation m_obj;
};

#endif // CODEC_H
