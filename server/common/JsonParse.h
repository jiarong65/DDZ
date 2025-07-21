#pragma once
#include <string>
#include <json/json.h>
#include <memory>

struct DBInfo{
    std::string ip;
    unsigned short port;
    std::string user;
    std::string passwd;
    std::string dbname;
};

class JsonParse{
public:
    enum DBType{Mysql,Redis};
    JsonParse(std::string fileName="../config/config.json");
    std::shared_ptr<DBInfo> getDatebaseInfo(DBType type);
private:
    Json::Value m_root;     // 用于存储解析结果的 JSON 根节点
};