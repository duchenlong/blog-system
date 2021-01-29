#include "table.hpp"

// 连接数据库
bool CreateMysql() {
    myMysql _mysql_tmpc;
    return _mysql_tmpc.MysqlInit();
}

// 释放数据库连接
bool DestoryMysql() {
    myMysql _mysql_tmpd;
    _mysql_tmpd.MysqlClose();
    return true;
}

/////////////////////////////////////////////////////
//////      博客标签
/////////////////////////////////////////////////////

//  插入一条数据
bool TagTable::Insert(const Json::Value& val) {
#define INSERT_TAG_DATA "insert into tag_blog values (null,'%s');"
    char sql[SQL_SIZE] = {'\0'};

    //Json::Value::acCString() 返回C风格的字符串
    sprintf(sql, INSERT_TAG_DATA, val["name"].asCString());

    //printf("%s\n",sql);
    //执行sql语句
    bool ret = _mysql->MysqlQuery(sql);
    return ret;
}

//  删除指定id一条数据
bool TagTable::Delete(int id) {
#define DELETE_TAG_DATA "delete from tag_blog where id=%d;"
    char sql[SQL_SIZE] = {'\0'};

    sprintf(sql, DELETE_TAG_DATA, id);

    bool ret = _mysql->MysqlQuery(sql);
    return ret;
}

//  更新指定id一条数据
bool TagTable::Update(int id,const Json::Value& val) {
#define UPDATE_TAG_DATA "update tag_blog set name='%s' where id=%d;"
    char sql[SQL_SIZE] = {'\0'};

    sprintf(sql,UPDATE_TAG_DATA, val["name"].asCString(),id);

    bool ret = _mysql->MysqlQuery(sql);
    return ret;
}

//  获得所有数据
bool TagTable::GetAll(Json::Value* val) {
    std::lock_guard<std::mutex> lock(_mutex);
#define GETALL_TAG_DATA "select * from tag_blog;"

    bool ret =  _mysql->MysqlQuery(GETALL_TAG_DATA);
    if(ret == false)
        return false;
    
    return Copy(val); 
}

//  获得指定Id的一条数据 
bool TagTable::GetOne(int id,Json::Value* val) {
    std::lock_guard<std::mutex> lock(_mutex);
#define GETONE_TAG_DATA "select * from tag_blog where id=%d;"
    char sql[SQL_SIZE] = {'\0'};
    sprintf(sql, GETONE_TAG_DATA, id);

    bool ret = _mysql->MysqlQuery(sql);
    if(ret == false)
        return false;
    return Copy(val,true);
}

// 进行数据的拷贝
bool TagTable::Copy(Json::Value* val, bool isOne) {
    //得到数据库查询结果集
    MYSQL_RES* res = _mysql->MysqlRes();    

    //获得结果集中有多少行数据
    int num_row = mysql_num_rows(res);
    if(num_row <= 0) {
        LOG() << "tag_blog is empty" << endl;
        mysql_free_result(res);//释放结果集
        return false;
    }

    if(isOne && num_row != 1) {
        LOG() << "tag_blog is not one" << endl;
        mysql_free_result(res);//释放结果集
        return false;
    }

    //开始append
    for(int i = 0; i < num_row; i++) {
        //获取结果集中当前行的数据,调用后 res 指向下一行数据首地址
        MYSQL_ROW row = mysql_fetch_row(res);
        Json::Value tag;
        tag["id"] = std::stoi(row[0]);
        tag["name"] = row[1];
        //添加该数据到参数中，在函数调用处打印
        val->append(tag);
    }

    //释放结果集
    mysql_free_result(res);
    return true;
}

/////////////////////////////////////////////////////
//////      博客用户
/////////////////////////////////////////////////////

//  插入一条数据
bool UserTable::Insert(const Json::Value& val) {
#define INSERT_USER_DATA "insert into user_blog values(null,'%s');"
    char sql[SQL_SIZE] = {'\0'};
    sprintf(sql,INSERT_USER_DATA, val["name"].asCString());

    bool ret = _mysql->MysqlQuery(sql);
    return ret;
}

//  删除指定id一条数据
bool UserTable::Delete(int id) {
#define DELETE_USER_DATA "delete from user_blog where id=%d;"
    char sql[SQL_SIZE] = {'\0'};
    sprintf(sql,DELETE_USER_DATA, id);

    bool ret = _mysql->MysqlQuery(sql);
    return ret;
}

//  更新指定id一条数据
bool UserTable::Update(int id,const Json::Value& val) {
#define UPDATE_USER_DATA "update user_blog set name='%s' where id=%d;"
    char sql[SQL_SIZE] = {'\0'};
    sprintf(sql,UPDATE_USER_DATA,val["name"].asCString(),id);
           
    bool ret = _mysql->MysqlQuery(sql);
    return ret;
}

//  获得所有数据
bool UserTable::GetAll(Json::Value* val) {
    std::lock_guard<std::mutex> lock(_mutex);
#define GETALL_USER_DATA "select * from user_blog;"
            
    int ret = _mysql->MysqlQuery(GETALL_USER_DATA);
    if(ret == false)
        return false;
    return Copy(val);
}

//  获得指定Id的一条数据 
bool UserTable::GetOne(int id,Json::Value* val) {
    std::lock_guard<std::mutex> lock(_mutex);
#define GETONE_USER_DATA "select * from user_blog where id=%d;"
            
    char sql[SQL_SIZE] = {'\0'};
    sprintf(sql, GETONE_USER_DATA, id);

    bool ret = _mysql->MysqlQuery(sql);
    if(ret == false)
        return false;
    return Copy(val,true);
}

// 进行数据的拷贝
bool UserTable::Copy(Json::Value* val, bool isOne){
    MYSQL_RES* res = _mysql->MysqlRes();
    int num_row = mysql_num_rows(res);
    if(num_row <= 0) {
        LOG() << "user_blog is empty" << endl;
        mysql_free_result(res);
        return false;
    }
    if(isOne && num_row != 1) {
        LOG() << "user_blog count error" << endl;
        mysql_free_result(res);
        return false;
    }

    for(int i = 0; i < num_row; i++) {
        MYSQL_ROW row = mysql_fetch_row(res);
        Json::Value user;
        user["id"] = std::stoi(row[0]);
        user["name"] = row[1];

        val->append(user);
    }

    mysql_free_result(res);
    return true;
}



/////////////////////////////////////////////////////
//////      博客
/////////////////////////////////////////////////////

//  插入一条数据
bool BlogTable::Insert(const Json::Value& val) {
#define INSERT_BLOG_DATA "insert into blog values (null,%d,%d,'%s','%s',now());"
    char sql[SQL_SIZE * 100] = {'\0'};

    //Json::Value::acCString() 返回C风格的字符串
    //id tag_id user_id title content ctime
    sprintf(sql, INSERT_BLOG_DATA, val["tag_id"].asInt(),val["user_id"].asInt(),\
                    val["title"].asCString(),val["content"].asCString());
    
    //printf("%s\n",sql);
    //执行sql语句
    bool ret = _mysql->MysqlQuery(sql);
    return ret;
}

//  删除指定id一条数据
bool BlogTable::Delete(int id) {
#define DELETE_BLOG_DATA "delete from blog where id=%d;"
    char sql[SQL_SIZE] = {'\0'};
    sprintf(sql,DELETE_BLOG_DATA, id);

    bool ret = _mysql->MysqlQuery(sql);
    return ret;
}

//  更新指定id一条数据
bool BlogTable::Update(int id,const Json::Value& val) {
#define UPDATE_BLOG_DATA "update blog set tag_id=%d,user_id=%d,title='%s',\
            content='%s',ctime=now() where id=%d;"
    char sql[SQL_SIZE * 100] = {'\0'};

    sprintf(sql,UPDATE_BLOG_DATA, val["tag_id"].asInt(),val["user_id"].asInt(),\
                    val["title"].asCString(),val["content"].asCString(),id);
    bool ret = _mysql->MysqlQuery(sql);
    return ret;
}

//  获得所有数据
bool BlogTable::GetAll(Json::Value* val) {
    std::lock_guard<std::mutex> lock(_mutex);
#define GETALL_BLOG_DATA "select id,tag_id,user_id,title,ctime  from blog;"
    int ret = _mysql->MysqlQuery(GETALL_BLOG_DATA);
    if(ret == false)
        return false;
    return Copy(val);
}

//  获得指定标签id的博客
bool BlogTable::GetTag(int id,Json::Value* val) {
    std::lock_guard<std::mutex> lock(_mutex);
#define GETTAG_BLOG_DATA "select id,tag_id,user_id,title,ctime from blog where tag_id=%d;"
    char sql[SQL_SIZE] = {'\0'};
    sprintf(sql,GETTAG_BLOG_DATA,id);

    int ret = _mysql->MysqlQuery(sql);
    if(ret == false)
        return false;
    return Copy(val);
}

//  获得指定用户id的博客
bool BlogTable::GetUser(int id,Json::Value* val) {
    std::lock_guard<std::mutex> lock(_mutex);
#define GETUSER_BLOG_DATA "select id,tag_id,user_id,title,ctime from blog where user_id=%d;"
    char sql[SQL_SIZE] = {'\0'};
    sprintf(sql,GETUSER_BLOG_DATA,id);

    int ret = _mysql->MysqlQuery(sql);
    if(ret == false)
        return false;
    return Copy(val);
}

//  获得指定Id的一条数据 
bool BlogTable::GetOne(int id,Json::Value* val) {
    std::lock_guard<std::mutex> lock(_mutex);
#define GETONE_BLOG_DATA "select * from blog where id=%d;"
    char sql[SQL_SIZE] = {'\0'};
    sprintf(sql,GETONE_BLOG_DATA,id);

    int ret = _mysql->MysqlQuery(sql);
    if(ret == false)
        return false;
    return Copy(val,true);
}

// 进行数据的拷贝
bool BlogTable::Copy(Json::Value* val, bool isOne) {
    MYSQL_RES* res = _mysql->MysqlRes();

    int num_row = mysql_num_rows(res);
    if(num_row <= 0) {
        LOG() << "blog is empty" << endl;
        mysql_free_result(res);
        return false;
    }

    if(isOne) {
        if(num_row != 1) {
            LOG() << "blog count error" << endl;
            mysql_free_result(res);
            return false;
        }

        for(int i = 0; i < num_row; i++) {
            MYSQL_ROW row = mysql_fetch_row(res);
            Json::Value blog;
        
            blog["id"] = std::stoi(row[0]);
            blog["tag_id"] = std::stoi(row[1]);
            blog["user_id"] = std::stoi(row[2]);
            blog["title"] = row[3];
            blog["content"] = row[4];
            blog["ctime"] = row[5];

            val->append(blog);
        }
        mysql_free_result(res);
        return true;
    }

    for(int i = 0; i < num_row; i++) {
        MYSQL_ROW row = mysql_fetch_row(res);
        Json::Value blog;
        
        blog["id"] = std::stoi(row[0]);
        blog["tag_id"] = std::stoi(row[1]);
        blog["user_id"] = std::stoi(row[2]);
        blog["title"] = row[3];
        blog["ctime"] = row[4];

        val->append(blog);
    }
    mysql_free_result(res);
    return true;
}
