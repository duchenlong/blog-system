#pragma once

#include "mysqlApi.hpp"
#include "head.hpp"

#include <jsoncpp/json/json.h>
#include <pthread.h>

#include <algorithm>
#include <mutex>

// sql语句的大小
#define SQL_SIZE 1024

// 互斥锁，查找拷贝数据时使用
static mutex _mutex;

// 连接数据库
bool CreateMysql();

// 释放数据库连接
bool DestoryMysql(); 

// 数据库实体的接口
class table {
    public:
        table() {
            _mysql = new myMysql();
        }
        virtual ~table() {
            delete _mysql;
            _mysql = nullptr;
        }
        //  插入一条数据
        virtual bool Insert(const Json::Value& val) =0;
        //  删除指定id一条数据
	    virtual bool Delete(int id) =0;
        //  更新指定id一条数据
	    virtual bool Update(int id,const Json::Value& val) =0;
        //  获得所有数据
	    virtual bool GetAll(Json::Value* val) =0;
        //  获得指定Id的一条数据 
	    virtual bool GetOne(int id,Json::Value* val) =0;
        //  获得指定标签id的博客
        virtual bool GetTag(int id,Json::Value* val) = 0;
        //  获得指定用户id的博客
        virtual bool GetUser(int id,Json::Value* val) = 0;
    protected:
        // 进行数据的拷贝
        virtual bool Copy(Json::Value* val, bool isOne = false) = 0;
        myMysql* _mysql;
};

/////////////////////////////////////////////////////
//////      博客标签
/////////////////////////////////////////////////////
class TagTable : public table{
    public:
        //  插入一条数据
        virtual bool Insert(const Json::Value& val);
        //  删除指定id一条数据
	    virtual bool Delete(int id);
        //  更新指定id一条数据
	    virtual bool Update(int id,const Json::Value& val);
        //  获得所有数据
	    virtual bool GetAll(Json::Value* val);
        //  获得指定Id的一条数据 
	    virtual bool GetOne(int id,Json::Value* val);
    private:
        //  获得指定标签id的博客
        bool GetTag(int id,Json::Value* val) {}
        //  获得指定用户id的博客
        bool GetUser(int id,Json::Value* val) {}
    protected:
        // 进行数据的拷贝
        virtual bool Copy(Json::Value* val, bool isOne = false);
};

/////////////////////////////////////////////////////
//////      博客用户
/////////////////////////////////////////////////////
class UserTable : public table {
    public:
        //  插入一条数据
        virtual bool Insert(const Json::Value& val);
        //  删除指定id一条数据
	    virtual bool Delete(int id);
        //  更新指定id一条数据
	    virtual bool Update(int id,const Json::Value& val);
        //  获得所有数据
	    virtual bool GetAll(Json::Value* val);
        //  获得指定Id的一条数据 
	    virtual bool GetOne(int id,Json::Value* val);
    private:
        //  获得指定标签id的博客
        bool GetTag(int id,Json::Value* val) {}
        //  获得指定用户id的博客
        bool GetUser(int id,Json::Value* val) {}
    protected:
        // 进行数据的拷贝
        virtual bool Copy(Json::Value* val, bool isOne = false);
};


/////////////////////////////////////////////////////
//////      博客
/////////////////////////////////////////////////////
class BlogTable : public table {
    public:
        //  插入一条数据
        virtual bool Insert(const Json::Value& val);
        //  删除指定id一条数据
	    virtual bool Delete(int id);
        //  更新指定id一条数据
	    virtual bool Update(int id,const Json::Value& val);
        //  获得所有数据
	    virtual bool GetAll(Json::Value* val);
        //  获得指定Id的一条数据 
	    virtual bool GetOne(int id,Json::Value* val);
        //  获得指定标签id的博客
        virtual bool GetTag(int id,Json::Value* val);
        //  获得指定用户id的博客
        virtual bool GetUser(int id,Json::Value* val);
    protected:
        // 进行数据的拷贝
        virtual bool Copy(Json::Value* val, bool isOne = false);
};