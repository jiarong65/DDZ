#include "gamemode.h"
#include "ui_gamemode.h"
#include "gamepanel.h"
#include <QCloseEvent>
#include <codec.h>
#include "datamanager.h"
#include "joinroom.h"

GameMode::GameMode(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GameMode)
{
    ui->setupUi(this);
    setFixedSize(578,450);
    ui->stackedWidget->setCurrentIndex(0);
    ui->information->setVisible(false);
    connect(ui->singleModeBtn,&QPushButton::clicked,this,[=](){
        GamePanel* panel=new GamePanel;
        connect(panel,&GamePanel::panelClose,this,&GameMode::show);
        panel->show();
        hide();
        DataManager::getInstance()->setGameMode(DataManager::Single);
    });

    connect(ui->netModeBtn,&QPushButton::clicked,this,[=](){
        ui->stackedWidget->setCurrentIndex(1);
        DataManager::getInstance()->setGameMode(DataManager::Network);
    });

    connect(ui->autoBtn,&QPushButton::clicked,this,[=](){
        Message msg;
        msg.userName=DataManager::getInstance()->getUserName();
        msg.reqCode=RequestCode::AutoRoom;
        DataManager::getInstance()->getCommun()->sendMessage(&msg);
        DataManager::getInstance()->setRoomMode(DataManager::Auto);
    });

    connect(ui->manualBtn,&QPushButton::clicked,this,[=](){
        JoinRoom room(DialogType::Manual);
        room.setWindowTitle("创建房间");
        room.exec();
    });
    connect(ui->searchBtn,&QPushButton::clicked,this,[=](){
        JoinRoom room(DialogType::Search);
        room.setWindowTitle("搜索房间");
        room.exec();
    });

    //取出通信类实例对象
    Communication* comm=DataManager::getInstance()->getCommun();
    connect(comm,&Communication::playerCount,this,[=](int count){
        showInfo(count);
        ui->information->setVisible(true);
    });

    connect(ui->stackedWidget,&QStackedWidget::currentChanged,this,[=](int index){
        if(index==0){
            ui->information->setVisible(false);
        }
    });

    connect(comm,&Communication::startGame,this,[=](QByteArray msg){
        this->hide();
        GamePanel* panel=new GamePanel;
        connect(panel,&GamePanel::panelClose,this,&GameMode::show);
        panel->show();
        panel->initGamePanel(msg);
        //断开信号，让GamePanel接收信号
        disconnect(comm,&Communication::startGame,this,nullptr);
    });

    connect(comm,&Communication::someBodyLeave,this,&GameMode::showInfo);
}

GameMode::~GameMode()
{
    delete ui;
}

void GameMode::showInfo(int count)
{
    QString tip=QString("当前<%1>房间玩家人数 %2 人，正在等待其它玩家进入，请稍后...")
                            .arg(DataManager::getInstance()->getRoomName().data())
                            .arg(count);
    ui->information->setText(tip);
}

void GameMode::closeEvent(QCloseEvent *ev)
{
    if(ui->stackedWidget->currentIndex()==1){
        ui->stackedWidget->setCurrentIndex(0);
        ev->ignore();
    }else{
        ev->accept();

        Message msg;
        msg.reqCode=RequestCode::Goodbye;
        msg.userName=DataManager::getInstance()->getUserName();
        msg.roomName=DataManager::getInstance()->getRoomName();
        DataManager::getInstance()->getCommun()->sendMessage(&msg);
        DataManager::getInstance()->getCommun()->stopLoop();
    }
}
