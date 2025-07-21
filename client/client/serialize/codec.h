#ifndef CODEC_H
#define CODEC_H

#include <QByteArray>
#include<QSharedPointer>
#include "infomation.pb.h"

struct Message{
    QByteArray userName;
    QByteArray roomName;
    QByteArray data1;
    QByteArray data2;
    QByteArray data3;
    RequestCode reqCode;
    RespondCode resCode;
};

class Codec
{
public:
    //序列化
    Codec(Message* msg);
    //反序列化
    Codec(QByteArray msg);
    //数据编码
    QByteArray encodeMsg();
    //数据解码
    QSharedPointer<Message> decodeMsg();
    //重新加载数据
    void reload(QByteArray msg);
    void reload(Message* msg);
    ~Codec();
private:
    QByteArray m_msg;
    Infomation m_obj;
};

#endif // CODEC_H
