#include "login.h"
#include "ui_login.h"
#include "datamanager.h"
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QThreadPool>
#include <gamepanel.h>
#include <QCryptographicHash>
#include "communication.h"
#include <QMessageBox>
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>
#include "gamemode.h"

login::login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::login)
{
    ui->setupUi(this);
    setFixedSize(750,447);
    ui->stackedWidget->setCurrentIndex(0);

    connect(ui->homeBtn,&QPushButton::clicked,this,[=](){
        ui->stackedWidget->setCurrentIndex(0);
    });
    connect(ui->netSetBtn,&QPushButton::clicked,this,[=](){
        ui->stackedWidget->setCurrentIndex(2);
    });
    connect(ui->regUser,&QPushButton::clicked,this,[=](){
        ui->stackedWidget->setCurrentIndex(1);
    });

    //数据校验
    //用户名
    QRegularExpression expreg("^[a-zA-Z0-9_]{3,16}$");
    QRegularExpressionValidator *val1=new QRegularExpressionValidator(expreg,this);
    ui->userName->setValidator(val1);
    ui->regUserName->setValidator(val1);

    //密码 长度4-12  至少一个大写字母，一个小写字母，数字和特殊字符
    expreg.setPattern("^(?=.*[A-Z])(?=.*[a-z])(?=.*\\d)(?=.*[@!$#*?&])[A-Za-z\\d@!$#*?&]{4,12}$");
    QRegularExpressionValidator *val2=new QRegularExpressionValidator(expreg,this);
    ui->password->setValidator(val2);
    ui->regPassword->setValidator(val2);

    //手机号
    expreg.setPattern("^1[3456789]\\d{9}$");
    QRegularExpressionValidator *val3=new QRegularExpressionValidator(expreg,this);
    ui->phone->setValidator(val3);

    //IP
    expreg.setPattern("^(((\\d)|([1-9]\\d)|(1\\d{2})|(2[0-4]\\d)|(25[0-5]))\\.){3}((\\d)|([1-9]\\d)|(1\\d{2})|(2[0-4]\\d)|(25[0-5]))$");
    QRegularExpressionValidator *val4=new QRegularExpressionValidator(expreg,this);
    ui->ipAddr->setValidator(val4);

    //端口
    expreg.setPattern("^([0-9]|[1-9][0-9]{1,3}|[1-5][0-9]{4}|6[0-4][0-9]{3}|65[0-4][0-9]{2}|655[0-2][0-9]|6553[0-5])$");
    QRegularExpressionValidator *val5=new QRegularExpressionValidator(expreg,this);
    ui->port->setValidator(val5);

    //按钮点击事件
    connect(ui->loginBtn,&QPushButton::clicked,this,&login::onLogin);
    connect(ui->registerBtn,&QPushButton::clicked,this,&login::onRegister);
    connect(ui->netOkBtn,&QPushButton::clicked,this,&login::onNetOk);

    //设置线程池最大线程数
    QThreadPool::globalInstance()->setMaxThreadCount(8);

    loadUserInfo();
}

bool login::verityData(QLineEdit* edit){
    if(!edit->hasAcceptableInput()){
        //标红
        edit->setStyleSheet("border: 2px solid red;");
        return false;
    }else{
        edit->setStyleSheet("none");
    }
    return true;
}

void login::onLogin(){
    qDebug()<<"登录";
    bool f1=verityData(ui->userName);
    bool f2=verityData(ui->password);
    if(f1&&f2){
//        GamePanel* panel=new GamePanel;
//        panel->show();
        Message msg;
        msg.userName=ui->userName->text().toUtf8();
        msg.reqCode=RequestCode::UserLogin;

        QByteArray pw=ui->password->text().toUtf8();
        pw=QCryptographicHash::hash(pw,QCryptographicHash::Sha224).toHex();
        msg.data1=pw;

        //连接服务器
        startConnect(&msg);
    }
}

void login::onRegister(){
    qDebug()<<"注册";
    bool f1=verityData(ui->regUserName);
    bool f2=verityData(ui->regPassword);
    bool f3=verityData(ui->phone);
    if(f1&&f2&&f3){
        Message msg;
        msg.userName=ui->regUserName->text().toUtf8();
        qDebug()<<"userName"<<msg.userName;
        msg.reqCode=RequestCode::Register;

        QByteArray pw=ui->regPassword->text().toUtf8();
        pw=QCryptographicHash::hash(pw,QCryptographicHash::Sha224).toHex();
        msg.data1=pw;
        msg.data2=ui->phone->text().toUtf8();

        //连接服务器
        startConnect(&msg);
    }
}

void login::onNetOk(){
    bool f1=verityData(ui->ipAddr);
    bool f2=verityData(ui->port);
    if(f1&&f2){
        DataManager* instance=DataManager::getInstance();
        instance->setIp(ui->ipAddr->text().toUtf8());//toUtf8()将QString转化为QByteArray
        instance->setPort(ui->port->text().toUtf8());
    }
}

void login::startConnect(Message* msg)
{
    if(!m_isConnect){
        Communication *task=new Communication(*msg);
        connect(task,&Communication::connectFailed,this,[=](){
            QMessageBox::critical(this,"连接服务器","连接服务器失败");
            m_isConnect=false;
        });
        connect(task,&Communication::loginOk,this,[=](){
            //将用户名保存到单例对象
            DataManager::getInstance()->setUserName(ui->userName->text().toUtf8());
            //记住密码
            saveUserInfo();
            //显示游戏窗口 单机、网络
            GameMode* mode=new GameMode;
            mode->show();
            accept();
        });
        connect(task,&Communication::registerOk,this,[=](){
            ui->stackedWidget->setCurrentIndex(0);
        });
        connect(task,&Communication::failedMsg,this,[=](QByteArray msg){
            QMessageBox::critical(this,"error",msg);
        });
        m_isConnect=true;
        QThreadPool::globalInstance()->start(task);
        qDebug()<<"连接服务器startConnect";
        DataManager::getInstance()->setCommun(task);
    }else{
        DataManager::getInstance()->getCommun()->sendMessage(msg);
    }
}

void login::saveUserInfo()
{
    if(ui->savePwd->isChecked()){
        QJsonObject obj;
        obj.insert("user",ui->userName->text());
        obj.insert("passwd",ui->password->text());
        QJsonDocument doc(obj);
        QByteArray json=doc.toJson();
        AesCrypto aes(AesCrypto::AES_CBC_128,Key.left(16));
        json=aes.encrypt(json);
        QFile file("passwd.bin");
        file.open(QFile::WriteOnly);
        file.write(json);
        file.close();
    }else{
        QFile file("passwd.bin");
        file.remove();
    }
}

void login::loadUserInfo()
{

    QFile file("passwd.bin");
    bool flag=file.open(QFile::ReadOnly);
    if(flag){
        ui->savePwd->setChecked(true);
        QByteArray json=file.readAll();
        AesCrypto aes(AesCrypto::AES_CBC_128,Key.left(16));
        json=aes.decrypt(json);
        QJsonDocument doc=QJsonDocument::fromJson(json);
        QJsonObject obj=doc.object();
        QString name=obj.value("user").toString();
        QString passwd=obj.value("passwd").toString();
        ui->userName->setText(name);
        ui->password->setText(passwd);
    }else{
        ui->savePwd->setChecked(false);
    }
}

login::~login()
{
    delete ui;
}
