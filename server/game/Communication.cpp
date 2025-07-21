#include "Communication.h"
#include <netinet/in.h>
#include "RsaCrypto.h"
#include "Hash.h"
#include <Log.h>
#include "TcpConnection.h"
#include "JsonParse.h"
#include <memory>
#include <random>

void Communication::setCallBack(sendCallBack func1, deleteCallBack func2)
{
    sendMessage=func1;
    disConnect=func2;
}

void Communication::parseRequest(Buffer *buf)
{
    //读数据头
    std::string data=buf->data(sizeof(int));
    int length=*(int*)data.data();
    //转换成小端
    length=ntohl(length);
    //读数据块
    data=buf->data(length);

    if(m_aes){
        data=m_aes->decrypt(data);
    }

    //反序列化
    Codec codec(data);
    std::shared_ptr<Message> ptr=codec.decodeMsg();
    Debug("%d",ptr->reqCode);
    Message resMsg;
    sendCallBack myFunc=sendMessage;
    switch(ptr->reqCode){
        case RequestCode::AesFenfa:
            handleAesFenfa(ptr.get(),resMsg);
            break;
        case RequestCode::UserLogin:
            handleLogin(ptr.get(),resMsg);
            break;
        case RequestCode::Register:
            handleRegister(ptr.get(),resMsg);
            break;
        case RequestCode::AutoRoom:
        case RequestCode::ManualRoom:
            handleAddRoom(ptr.get(),resMsg);
            myFunc=std::bind(&Communication::readyForPlay,this,resMsg.roomName,std::placeholders::_1);
            break;
        case RequestCode::GrabLord:
            Debug("grablord");
            resMsg.data1=ptr->data1;
            resMsg.resCode=RespondCode::OtherGrabLord;
            myFunc=std::bind(&Communication::notifyOtherPlayers,this,std::placeholders::_1,ptr->roomName,ptr->userName);
            Debug("roomName:%s userName:%s",ptr->roomName.c_str(),ptr->userName.c_str());
            break;
        case RequestCode::PlayAHand:
            resMsg.data1=ptr->data1;
            resMsg.data2=ptr->data2;
            resMsg.resCode=RespondCode::OtherPlayHand;
            myFunc=std::bind(&Communication::notifyOtherPlayers,this,std::placeholders::_1,ptr->roomName,ptr->userName);
            Debug("roomName:%s userName:%s",ptr->roomName.c_str(),ptr->userName.c_str());
            break;
        case RequestCode::GameOver:
            handleGameOver(ptr.get());
            myFunc=nullptr;
            break;
        case RequestCode::Continue:
            restartGame(ptr.get());
            myFunc=nullptr;
            break;
        case RequestCode::SearchRoom:
            handleSearchRoom(ptr.get(),resMsg);
            break;
        case RequestCode::LeaveRoom:
            handleLeaveRoom(ptr.get(),resMsg);
            myFunc=nullptr;
            break;
        case RequestCode::Goodbye:
            handleGoodBye(ptr.get());
            myFunc=nullptr;
            break;
        default:
            break;
    }

    if(myFunc != nullptr)
    {
        codec.reload(&resMsg);
        std::string msg = codec.encodeMsg();
        Debug("回复给客户端的数据: %s, size = %ld, status: %d,roomName: %s", msg.data(), msg.size(), resMsg.resCode,resMsg.roomName.c_str());
        myFunc(msg);
    }
}

void Communication::handleAesFenfa(Message* reqMsg,Message& resMsg)
{
    RsaCrypto rsa;
    rsa.parseStringToKey(m_redis->rsaSecKey("PrivateKey"),RsaCrypto::KeyType::PrivateKey);
    std::string aesKey=rsa.priKeyDecrypt(reqMsg->data1);

    //hash校验
    Hash hash(HashType::Sha224);
    hash.addData(aesKey);
    std::string res=hash.result();

    resMsg.resCode=RespondCode::AesVerifyOK;
    if(reqMsg->data2!=res){
        Debug("aes 密钥校验失败");
        resMsg.resCode=RespondCode::Failed;
        resMsg.data1="aes 密钥校验失败";
    }else{
        m_aes=new AesCrypto(AesCrypto::AES_CBC_256,aesKey);
        Debug("aes 密钥校验成功");
    }
}

void Communication::handleRegister(Message *reqMsg, Message &resMsg)
{
    Debug("开始注册");
    //查询数据库
    char sql[1024];
    sprintf(sql,"SELECT * FROM user WHERE name='%s'",reqMsg->userName.data());
    bool flag=m_mysql->query(sql);
    if(flag&&!m_mysql->next()){
        //将注册信息写入数据库
        m_mysql->transaction();
        sprintf(sql, "INSERT INTO `user`(name, passwd, phone, date) VALUES('%s', '%s', '%s', NOW());",
                reqMsg->userName.data(), reqMsg->data1.data(), reqMsg->data2.data());
        bool fl1 = m_mysql->update(sql);
        Debug("fl1 = %d, sql = %s", fl1, sql);
        sprintf(sql, "INSERT INTO `information`(name, score, status) VALUES('%s', 0, 0);",
                reqMsg->userName.data());
        bool fl2 = m_mysql->update(sql);
        Debug("fl2 = %d, sql = %s", fl2, sql);
        if(fl1&&fl2){
            m_mysql->commit();
            resMsg.resCode=RespondCode::RegisterOk;
        }else{
            m_mysql->rollback();
            resMsg.resCode=RespondCode::Failed;
            resMsg.data1="数据库插入数据失败!";
        }
    }else{
        resMsg.resCode=RespondCode::Failed;
        resMsg.data1="用户名已存在！";
    }
}

Communication::Communication()
{
    JsonParse json;
    std::shared_ptr<DBInfo> ptr=json.getDatebaseInfo(JsonParse::DBType::Mysql);
    m_mysql=new MySqlConn;
    m_redis=new Room;
    assert(m_redis->initEnvironment());
    // bool flag=m_mysql->connect("root","happy-ddz","123456","localhost");
    bool flag=m_mysql->connect(ptr->user,ptr->dbname,ptr->passwd,ptr->ip);
    assert(flag);
}

void Communication::handleLogin(Message *reqMsg, Message &resMsg)
{
    Debug("开始登录");
    char sql[1024];
    sprintf(sql,"SELECT * FROM `user` WHERE `name`='%s' AND passwd='%s' AND (select  count(*) from information where name = '%s' AND status=0);",
        reqMsg->userName.data(),reqMsg->data1.data(),reqMsg->userName.data());
    bool flag=m_mysql->query(sql);
    if(flag&&m_mysql->next()){
        m_mysql->transaction();
        sprintf(sql,"update `information` set status=1 WHERE `name`='%s'",
            reqMsg->userName.data());
        bool flag1=m_mysql->update(sql);
        if(flag1){
            m_mysql->commit();
            resMsg.resCode=RespondCode::LoginOK;
            Debug("用户登录成功了.....................................");
            return;
        }
        m_mysql->rollback();
    }
    resMsg.resCode=RespondCode::Failed;
    resMsg.data1="用户名或者密码错误, 或者当前用户已经成功登录了...";
}

Communication::~Communication()
{
    if(m_mysql){
        delete m_mysql;
    }
    if(m_redis){
        delete m_redis;
    }
    if(m_aes){
        delete m_aes;
    }
}

void Communication::handleAddRoom(Message *reqMsg, Message &resMsg)
{
    //登录后不是第一次加入房间
    std::string oldRoom=m_redis->whereAmUser(reqMsg->userName);
    bool flag=true;
    int score=m_redis->getPlayerScore(oldRoom,reqMsg->userName);
    if(oldRoom!=std::string()){
        m_redis->leaveRoom(oldRoom,reqMsg->userName);
        RoomList::getIntance()->removePlayer(oldRoom,reqMsg->userName);
    }
    std::string roomName;
    if(reqMsg->reqCode==RequestCode::AutoRoom){
        roomName=m_redis->joinRoom(reqMsg->userName);
    }else{
        roomName=reqMsg->roomName;
        flag=m_redis->joinRoom(reqMsg->userName,roomName);
    }
    //判断是否已经加入了某个房间
    if(flag){
        Debug("加入房间成功...");
        //1.加载分数 2.redis更新分数 3.mysql同步分数
        if(score==0){
            //登录后第一次加入房间
            //查询mysql   
            std::string sql="select score from information where name='"+reqMsg->userName+"';";
            bool fl=m_mysql->query(sql);
            assert(fl);
            m_mysql->next();
            score=std::stoi(m_mysql->value(0));
        }
        //更新分数
        m_redis->updatePlayerScore(roomName,reqMsg->userName,score);
        //处理房间通讯fd
        RoomList* roomList=RoomList::getIntance();
        roomList->addUser(roomName,reqMsg->userName,sendMessage);
        //给客户端回复数据
        resMsg.resCode=RespondCode::JoinRoomOK;
        resMsg.data1=std::to_string(m_redis->getPlayerCount(roomName));//房间人数
        resMsg.roomName=roomName;//roomname
        Debug("resMsg.roomName=roomName; %s",resMsg.roomName.c_str());
    }else{
        Debug("加入房间失败...");
        resMsg.resCode=RespondCode::Failed;
        resMsg.data1="加入房间失败，人数已满！";
    }
}

void Communication::readyForPlay(std::string roomName,std::string data)
{
    RoomList* roomList=RoomList::getIntance();
    UserMap players= roomList->getPlayers(roomName);
    for(auto item : players){
        item.second(data);
        Debug("当前房间内人数: %ld==================", players.size());
    }
    //房间已满，开始游戏
    if(players.size()==3){
        startGame(roomName,players);
    }
}

void Communication::dealCards(UserMap players)
{
    Message msg;

    initCards();
    std::string &all=msg.data1;
    for(int i=0;i<51;i++){
        auto card=takeOneCrad();
        std::string sub=std::to_string(card.first)+"-"+std::to_string(card.second)+"#";
        all+=sub;
    }

    //三张底牌
    std::string &lastCard=msg.data2;
    for(const auto& item : m_cards){
        std::string sub=std::to_string(item.first)+"-"+std::to_string(item.second)+"#";
        lastCard+=sub;
    }
    msg.resCode=RespondCode::DealCards;
    Codec codec(&msg);
    std::string res=codec.encodeMsg();

    for(auto it : players){
        it.second(res);
    }
}

void Communication::initCards()
{
    //洗牌
    //i 花色 j 点数
    for(int i=1;i<=4;i++){
        for(int j=1;j<=13;j++){
            m_cards.insert({i,j});
        }
    }
    m_cards.insert({0,14});
    m_cards.insert({0,15});
}

std::pair<int, int> Communication::takeOneCrad()
{
    // 创建随机设备对象
    std::random_device rd;
    // 创建随机数生成对象
    std::mt19937 gen(rd());
    // 创建随机数分布对象 -> 均匀分布
    std::uniform_int_distribution<int> dis(0,m_cards.size()-1);
    int randNum = dis(gen);
    auto it=m_cards.begin();
    for(int i=0;i<randNum;i++,it++);
    m_cards.erase(it);

    return *it;
}

void Communication::startGame(std::string roomName, UserMap players)
{
    //发牌
    dealCards(players);
    //通知客户端
    Message msg;
    msg.resCode=RespondCode::StartGame;
    //data1 userName-次序-分数, 次序的规则: 谁分数高, 谁优先抢地主
    msg.data1=m_redis->playersOrder(roomName);
    Codec codec(&msg);
    for(const auto& it : players){
        it.second(codec.encodeMsg());
    }
}

void Communication::notifyOtherPlayers(std::string data, std::string roomName, std::string userName)
{
    RoomList* roomList=RoomList::getIntance();
    auto partners=roomList->getPartners(roomName,userName);
    for(const auto& item : partners){
        item.second(data);
    }
}

void Communication::handleGameOver(Message *reqMsg)
{
    //更新分数
    int score=std::stoi(reqMsg->data1);
    //redis
    m_redis->updatePlayerScore(reqMsg->roomName,reqMsg->userName,score);
    //mysql
    char sql[1024];
    sprintf(sql,"update information set score=%d where name='%s'",score,reqMsg->userName.data());
    m_mysql->update(sql);
}

void Communication::restartGame(Message *reqMsg)
{
    RoomList* roomList=RoomList::getIntance();
    //得到房间内玩家
    auto players=roomList->getPlayers(reqMsg->roomName);
    //判断房间人数
    if(players.size()==3){
        roomList->removeRoom(reqMsg->roomName);
    }
    //将玩家添加到单例对象中
    roomList->addUser(reqMsg->roomName,reqMsg->userName,sendMessage);

    players=roomList->getPlayers(reqMsg->roomName);
    if(players.size()==3){
        //发牌并开始游戏
        startGame(reqMsg->roomName,players);
    }
}

void Communication::handleSearchRoom(Message *reqMsg, Message &resMsg)
{
    bool flag=m_redis->searchRoom(reqMsg->roomName);
    resMsg.resCode=RespondCode::SearchRoomOK;
    resMsg.data1=flag==true?"true":"false";
}

void Communication::handleLeaveRoom(Message *reqMsg, Message &resMsg)
{
    m_redis->leaveRoom(reqMsg->roomName,reqMsg->userName);
    RoomList::getIntance()->removePlayer(reqMsg->roomName,reqMsg->userName);
    resMsg.resCode=RespondCode::OtherLeaveRoom;
    auto players=RoomList::getIntance()->getPlayers(reqMsg->roomName);
    resMsg.data1=std::to_string(players.size());
    for(const auto& item:players){
        Codec codec(&resMsg);
        item.second(codec.encodeMsg());
    }
}

void Communication::handleGoodBye(Message * reqMsg)
{
    //修改玩家登录状态
    char sql[10240]={0};
    sprintf(sql,"update information set status = 0 where name = '%s'",reqMsg->userName.data());
    m_mysql->update(sql);
    disConnect();
}
