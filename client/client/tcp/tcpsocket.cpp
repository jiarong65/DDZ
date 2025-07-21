#include "tcpsocket.h"
#include <QDebug>

TcpSocket::TcpSocket(QObject *parent)
{
#ifdef Q_OS_WIN
    WSADATA data;
    WSAStartup(MAKEWORD(2,2),&data);
#endif
}

TcpSocket::TcpSocket(QByteArray ip, unsigned short port, QObject *parent):TcpSocket(parent)
{
    connectToHost(ip,port);
}

bool TcpSocket::connectToHost(QByteArray ip, unsigned short port)
{
    assert(port>0&&port<65536);
    m_socket=socket(AF_INET,SOCK_STREAM,0);
    assert(m_socket);

    struct sockaddr_in addr;
    addr.sin_family=AF_INET;
    addr.sin_port=htons(port);
    addr.sin_addr.s_addr=inet_addr(ip.data());
    int ret=::connect(m_socket,reinterpret_cast<struct sockaddr*>(&addr),sizeof(addr));
    bool flag=ret==0?true:false;
    return flag;
}

QByteArray TcpSocket::recvMsg(int timeout)
{
    bool flag=readTimeout(timeout);
    QByteArray arry;
    if(flag){
        int headLen=0;
        int ret=readn((char*)&headLen,sizeof(int));
        if(ret==0){
            qDebug()<<"recv size 0 "<<headLen;
            return QByteArray();
        }
        qDebug()<<"大端长度"<<headLen;
        headLen=ntohl(headLen);
        qDebug()<<"小端长度"<<headLen;

        char* data=new char[headLen];
        assert(data);
        ret=readn(data,headLen);

        flag=ret==headLen?true:false;
        if(flag){
            //数据正常
            arry=QByteArray(data,headLen);
        }else{
            //数据异常
            arry=QByteArray();
        }

        delete[] data;
    }

    return arry;
}

bool TcpSocket::sendMsg(QByteArray msg, int timeout)
{
    bool flag=writeTimeout(timeout);
    if(flag){
        int headLen=htonl(msg.size());
        int length=msg.size()+sizeof(int);
        qDebug()<<length;

        char* data=new char[length];
        assert(data);
        memcpy(data,&headLen,sizeof(int));
        memcpy(data+sizeof(int),(char*)msg.data(),msg.size());
        qDebug()<<"writen";
        int ret=writen(data,length);
        qDebug()<<"writen down";
        flag=((ret==length)?true:false);

        delete[] data;
    }

    return flag;
}

void TcpSocket::disConnect()
{
    if(m_socket){
#ifdef Q_OS_WIN
        closesocket(m_socket);
#endif
#ifdef Q_QS_LInux
        close(m_socket);
#endif
    }
}

TcpSocket::~TcpSocket()
{
#ifdef Q_OS_WIN
    WSACleanup();
#endif
}

bool TcpSocket::readTimeout(int timeout)
{
    if(timeout==-1){
        return true;
    }

    int ndfs=0;
#ifdef Q_OS_Linux
    ndfs=m_socket+1;
#endif
    fd_set rdSet;
    FD_ZERO(&rdSet);
    FD_SET(m_socket,&rdSet);
    struct timeval tmout;
    tmout.tv_sec=timeout;
    tmout.tv_usec=0;
    int ret=select(ndfs,&rdSet,NULL,NULL,&tmout);
    bool flag=ret>0?true:false;
    return flag;
}

bool TcpSocket::writeTimeout(int timeout)
{
    if(timeout==-1){
        return true;
    }

    int ndfs=0;
#ifdef Q_OS_Linux
    ndfs=m_socket+1;
#endif
    fd_set wtSet;
    FD_ZERO(&wtSet);
    FD_SET(m_socket,&wtSet);
    struct timeval tmout;
    tmout.tv_sec=timeout;
    tmout.tv_usec=0;
    int ret=select(ndfs,NULL,&wtSet,NULL,&tmout);
    bool flag=ret>0?true:false;
    return flag;
}

int TcpSocket::readn(char *buf, int count)
{
    int last=count; //剩余字节数
    int size=0;     //每次读出字节数
    char* pt=buf;
    while(last>0){
        if((size=recv(m_socket,pt,last,0))<0){
            perror("recv err");
            return -1;
        }
        else if(size==0){
            break;
        }else{
            last-=size;
            pt+=size;
        }
    }
    return count-last;
}

int TcpSocket::writen(const char *buf, int count)
{
    int last=count; //剩余字节数
    int size=0;     //每次读出字节数
    const char* pt=buf;
    while(last>0){
        if((size=send(m_socket,pt,last,0))<0){
            perror("send err");
            return -1;
        }
        else if(size==0){
            continue;
        }else{
            last-=size;
            pt+=size;
        }
    }
    return count-last;
}
