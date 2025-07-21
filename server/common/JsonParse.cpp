#include "JsonParse.h"
#include <cassert>
#include <fstream>
#include "Log.h"

JsonParse::JsonParse(std::string fileName)
{
    std::ifstream ifs(fileName);
    //assert(!ifs.is_open());

    //  创建 JSON 解析器 和存储对象
    Json::Reader rd;  // 老版本的 JSON 解析器
    rd.parse(ifs,m_root);
    assert(m_root.isObject());
}

std::shared_ptr<DBInfo> JsonParse::getDatebaseInfo(DBType type)
{
    std::string DBName=type==DBType::Mysql?"mysql":"redis";
    Json::Value node=m_root[DBName];
    DBInfo* info=new DBInfo;
    info->ip=node["ip"].asString();
    info->port=node["port"].asInt();
   
    if(type==DBType::Mysql){
        info->dbname=node["dbname"].asString();
        info->user=node["user"].asString();
        info->passwd=node["password"].asString();
    }

    return std::shared_ptr<DBInfo>(info);
}

        // "ip":"localhost",
        // "port":3306,
        // "user":"root",
        // "password":"123456",
        // "dbname":"happy-ddz"