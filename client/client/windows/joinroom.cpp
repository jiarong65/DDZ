#include "joinroom.h"
#include "ui_joinroom.h"
#include "datamanager.h"
#include "mybutton.h"

JoinRoom::JoinRoom(DialogType type,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::JoinRoom)
{
    ui->setupUi(this);
    ui->joinRoom->setFixedSize(100, 45);
    ui->joinRoom->setImage(":/images/addRoom-normal.png", ":/images/addRoom-hover.png",
                               ":/images/addRoom-pressed.png", ":/images/addRoom-disable.png");
    ui->joinRoom->setBtnDisable(true);
    const QString titles[]={
        "请输入要创建的房间的名字：",
        "请输入要搜索的房间的名字："
    };
    int index=static_cast<int>(type);
    ui->describe->setText(titles[index]);
    ui->stackedWidget->setCurrentIndex(index);
    setFixedSize(300,180);

    connect(ui->createRoom,&QPushButton::clicked,this,&JoinRoom::joinRoom);
    connect(ui->joinRoom,&QPushButton::clicked,this,&JoinRoom::joinRoom);
    connect(ui->searchRoom,&QPushButton::clicked,this,&JoinRoom::searchRoom);

    Communication* comm=DataManager::getInstance()->getCommun();
    connect(comm,&Communication::roomExist,this,[=](bool flag){
        if(flag){
            ui->joinRoom->setBtnDisable(false);
        }else{
            ui->joinRoom->setBtnDisable(true);
        }
    });
}

JoinRoom::~JoinRoom()
{
    delete ui;
}

void JoinRoom::joinRoom()
{
    encodeMessage(RequestCode::ManualRoom);
    DataManager::getInstance()->setRoomMode(DataManager::Manual);
    accept();
}

void JoinRoom::searchRoom()
{
    encodeMessage(RequestCode::SearchRoom);
}

void JoinRoom::encodeMessage(RequestCode reqCode)
{
    Message msg;
    msg.reqCode=reqCode;
    msg.userName=DataManager::getInstance()->getUserName();
    msg.roomName=ui->roomName->text().toUtf8();
    DataManager::getInstance()->getCommun()->sendMessage(&msg);
}
