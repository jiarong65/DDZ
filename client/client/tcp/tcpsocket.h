#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include <QObject>
#ifdef Q_OS_WIN
#include <winsock2.h>
#endif

class TcpSocket : public QObject
{
    Q_OBJECT
public:
    TcpSocket(QObject* parent=nullptr);
    TcpSocket(QByteArray ip,unsigned short port,QObject* parent=nullptr);
    bool connectToHost(QByteArray ip,unsigned short port);
    QByteArray recvMsg(int timeout=-1);//timeout阻塞时长
    bool sendMsg(QByteArray msg,int timeout=-1);
    void disConnect();
    ~TcpSocket();
private:
#ifdef Q_OS_WIN
    SOCKET m_socket;
#endif
#ifdef Q_OS_LINUX
    int m_socket;
#endif
    bool readTimeout(int timeout);
    bool writeTimeout(int timeout);
    int readn(char* buf,int count);
    int writen(const char* buf,int count);
};

#endif
