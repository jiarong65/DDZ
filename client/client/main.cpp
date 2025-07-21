#include "gamepanel.h"
//#include "loading.h"
#include "login.h"

#include <QApplication>
#include <QResource>
#include<QDebug>
#include <QFile>
#include "aescrypto.h"
#include "rsacrypto.h"

void test()
{
    //aes
    AesCrypto aes(AesCrypto::AES_CBC_128,"1234567887654321");
    QByteArray text=aes.encrypt("本章中我们介绍了对称密码，以及DES、三重DES、AES和其他一些密码算法。");
    text=aes.decrypt(text);
    qDebug()<<text.data();

    //rsa
    RsaCrypto rsa;
    rsa.generateRsaKey(RsaCrypto::KeyLength::BITS_2k);

    RsaCrypto rsa1("public.pem",RsaCrypto::PublicKey);
    text="两情若是长久时，又岂在朝朝暮暮";
    qDebug()<<"明文："<<text.data();
    text=rsa1.pubKeyEncrypt(text);
    qDebug()<<"密文："<<text.data();

    QByteArray temp;
    RsaCrypto rsa2("private.pem",RsaCrypto::PrivateKey);
    temp=rsa2.priKeyDecrypt(text);
    qDebug()<<temp.data();

    temp=rsa2.sign(text);
    bool flag=rsa1.verify(temp,text);
    qDebug()<<flag;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qRegisterMetaType<Cards>("Cards&");
    qRegisterMetaType<Cards>("Cards");
    //QResource::registerResource("./resource.rcc");

    //加载资源文件 qss文件必须是utf8编码
    QFile file(":/conf/style.qss");
    file.open(QFile::ReadOnly);
    QByteArray all=file.readAll();
    a.setStyleSheet(all);
    file.close();

    login w;
    int ret=w.exec();
    if(ret==QDialog::Accepted){
        return a.exec();
    }
    //test();
}
