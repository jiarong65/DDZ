#include "communication.h"
#include "datamanager.h"
#include <QDebug>
#include "rsacrypto.h"
#include <QDateTime>
#include <QCryptographicHash>
#include <cards.h>
#include <QDataStream>
#include "taskqueue.h"

Communication::Communication(Message& msg,QObject *parent) : QObject(parent),m_msg(msg)
{
    setAutoDelete(true);
}

Communication::~Communication()
{
    if(m_aes){
        delete m_aes;
    }
}

void Communication::sendMessage(Message *msg,bool encrypto)
{
    qDebug()<<"用户名"<<msg->userName;
    //序列化
    Codec codec(msg);
    QByteArray data=codec.encodeMsg();
    //加密
    if(encrypto){
        data=m_aes->encrypt(data);
    }
    bool flag=m_socket->sendMsg(data);
    qDebug()<<"发送数据 "<<flag;
    qDebug()<<"data1"<<msg->data1<<","<<msg->reqCode;
}

void Communication::parseRecvMessage()
{
    //接收数据 (先接收rsa公钥，再发送aes密钥）
    QByteArray data=m_socket->recvMsg(2);

    if(data.isEmpty())return ;
    //反序列化
    Codec codec(data);
    QSharedPointer<Message> ptr=codec.decodeMsg();
    qDebug()<<ptr->resCode<<","<<ptr->data1<<","<<ptr->data2;
    switch(ptr->resCode){
    case RespondCode::LoginOK:
        emit loginOk();
        qDebug()<<"登录成功";
        break;
    case RespondCode::RegisterOk:
        emit registerOk();
        qDebug()<<"注册成功";
        break;
    case RespondCode::RsaFenfa:
        handleRsaFenfa(ptr.get());
        break;
    case RespondCode::AesVerifyOK:
        m_aes=new AesCrypto(AesCrypto::AES_CBC_256,m_aesKey);
        qDebug()<<"req:"<<m_msg.reqCode;
        sendMessage(&m_msg);
        qDebug()<<"aes 密钥分发成功";
        break;
    case RespondCode::Failed:
        emit failedMsg(ptr->data1);
        break;
    case RespondCode::JoinRoomOK:
        DataManager::getInstance()->setRoomName(ptr->roomName);
        qDebug()<<"roomName:"<<ptr->roomName;
        emit playerCount(ptr->data1.toInt());
        break;
    case RespondCode::DealCards:
        parseCards(ptr->data1,ptr->data2);
        break;
    case RespondCode::StartGame:
        emit startGame(ptr->data1);
        break;
    case RespondCode::OtherGrabLord:
    {
        Task t;
        t.bet=ptr->data1.toInt();
        TaskQueue::getInstance()->add(t);
        break;
    }
    case RespondCode::OtherPlayHand:
    {
        Task t;
        QDataStream stream(ptr->data2);
        for(int i=0;i<ptr->data1.toInt();i++){
            Card c;
            stream>>c;
            t.cs.add(c);
        }
        TaskQueue::getInstance()->add(t);
        break;
    }
    case RespondCode::SearchRoomOK:
    {
        bool flag=ptr->data1=="true"?true:false;
        emit roomExist(flag);
        break;
    }
    case RespondCode::OtherLeaveRoom:
    {
        int count=ptr->data1.toInt();
        emit someBodyLeave(count);
        break;
    }
    default:
        break;
    }
}

void Communication::handleRsaFenfa(Message* msg)
{
    RsaCrypto rsa;
    rsa.parseStringToKey(msg->data1,RsaCrypto::PublicKey);
    //校验签名
    bool flag=rsa.verify(msg->data2,msg->data1);
    assert(flag);

    m_aesKey=generateAesKey(KeyLen::L32);

    Message resMsg;
    resMsg.reqCode=RequestCode::AesFenfa;
    resMsg.data1=rsa.pubKeyEncrypt(m_aesKey);
    resMsg.data2=QCryptographicHash::hash(m_aesKey,QCryptographicHash::Sha224).toHex();
    qDebug()<<"sendMessage(&resMsg);";
    //qDebug()<<"data1"<<msg->data1<<","<<msg->reqCode;
    sendMessage(&resMsg,false);
}

QByteArray Communication::generateAesKey(Communication::KeyLen len)
{
    QByteArray time=QDateTime::currentDateTime().toString("yyyy.mm.dd-hh:mm:ss.zzz").toUtf8();
    QCryptographicHash hash(QCryptographicHash::Sha384);
    hash.addData(time);
    time=hash.result();
    time=time.left(len);
    return time;
}

void Communication::parseCards(QByteArray data1, QByteArray data2)
{
    auto func=bind([=](QByteArray data){
        auto lst=data.left(data.length()-1).split('#');
        Cards cs;
        for(const auto& it : lst){
            auto sub=it.split('-');
            Card card(static_cast<Card::CardPoint>(sub.last().toInt()),
                      static_cast<Card::CardSuit>(sub.first().toInt()));
            cs.add(card);
        }
        return cs;
    },std::placeholders::_1);
    auto cards=func(data1);
    auto last=func(data2);

    //存储数据
    DataManager::getInstance()->setCards(cards,last);
}

void Communication::run()
{
    m_socket=new TcpSocket;
    DataManager* obj=DataManager::getInstance();
    bool flag=m_socket->connectToHost(obj->getIp(),obj->getPort().toInt());
    if(!flag){
        emit connectFailed();
        delete m_socket;
        return;
    }

    //处理通信
    while(!m_stop){
        parseRecvMessage();
    }
    m_socket->disConnect();
    delete m_socket;
}
