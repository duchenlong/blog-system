#pragma once

#include "table.hpp"
#include "httplib.h"

#include <unistd.h>
#include <signal.h>
#include <sys/time.h>

//用户表，标签表，博客表的是三个类
static table* table_user;
static table* table_tag;
static table* table_blog;

void signalHandler(int signo) {    
    DestoryMysql();
    LOG() << "mysql fd reboot " << endl;
    CreateMysql();
}    
      
bool setCreate() {    
    signal(SIGALRM,signalHandler);    
    struct itimerval new_val,old_val;    
    new_val.it_value.tv_usec = 1;    
    new_val.it_value.tv_sec = 0;    
    new_val.it_interval.tv_usec = 0;    
    new_val.it_interval.tv_sec = 3600;    
    setitimer(ITIMER_REAL,&new_val,&old_val);    
        
    return true;    
}   

bool TableInit() {
    LOG() << "ing conncet mysql and init table" << endl;
    // 连接数据库
    //if(!CreateMysql()) return false;
    if(!setCreate()) return false;
    
    table_tag  = new TagTable();
    table_user = new UserTable();
    table_blog = new BlogTable();

    LOG() << "mysql and table init sucess" << endl;
    return true;
}

//博客用户表
//http请求 与 http响应
void InsertUser(const httplib::Request& req,httplib::Response& rsp)
{
    LOG() << "正在插入用户" << endl;
    //获取json字符串
    std::string json_user = req.body;
    //json字符串解析为Json::Value对象
    Json::Reader reader;
    Json::Value root;
    bool ret = reader.parse(json_user,root);
    if(ret == false)
    {
        LOG() << "insert user info parse json client error" << endl;
        rsp.status = 400;// 400 Bad Request 
        return ;
    }

    //调用table_user 中Inser接口插入数据
    ret = table_user->Insert(root);
    if(ret == false)
    {
        LOG() << "insert user info db server error" << endl;
        rsp.status = 500;//服务器内部错误
        return ;
    }

    //填充rsp响应头部信息
    rsp.status = 200;
}

//删除指定用户 参数为用户id
void DeleteUser(const httplib::Request& req,httplib::Response& rsp)
{
    LOG() << "正在删除用户" << endl;
    int user_id = std::stoi(req.matches[1]);
    
    bool ret = table_user->Delete(user_id);
    if(ret == false)
    {
        LOG() << "delete user from db server error" << endl;
        rsp.status = 500;
        return ;
    }
    rsp.status = 200;
}

//更新用户，参数为用户id
void PutUser(const httplib::Request& req,httplib::Response& rsp)
{
    LOG() << "正在更新用户信息" << endl;
    std::string json_user = req.body;
    int user_id = std::stoi(req.matches[1]);

    Json::Reader reader;
    Json::Value root;
    bool ret = reader.parse(json_user,root);
    if(ret == false)
    {
        LOG() << "update user from parse client error" << endl;
        rsp.status = 400;
        return ;
    }

    ret = table_user->Update(user_id,root);
    if(ret == false)
    {
        LOG() << "update user from db server error" << endl;
        rsp.status = 500;
        return ;
    }
    rsp.status = 200;
}

//获得所有用户信息
void GetAllUser(const httplib::Request& req,httplib::Response& rsp)
{
    LOG() << "正在获得所有用户信息" << endl;

    Json::Value root;
    bool ret = table_user->GetAll(&root);
    if(ret == false)
    {
        LOG() << "get all user info from db server error" << endl;
        rsp.status = 500;
        return ;
    }

    Json::FastWriter writer;
    std::string body = writer.write(root);
    // rsp.body = body;
    // void set_content(const char *s, size_t n, const char *content_type);
    rsp.set_content(&body[0],body.size(),"application/json");
    rsp.status = 200;
}

void GetOneUser(const httplib::Request& req,httplib::Response& rsp)
{
    LOG() << "正在读取指定用户信息" << endl;
    int user_id = std::stoi(req.matches[1]);

    Json::Value root;
    bool ret = table_user->GetOne(user_id,&root);
    if(ret == false)
    {
        LOG() << "get oneuser info from db server error" << endl;
        rsp.status = 500;
        return ;
    }
    
    Json::FastWriter writer;
    // void set_content(std::string s, const char *content_type);
    rsp.set_content(writer.write(root),"application/json");
    rsp.status = 200;
}

//博客标签表
void InsertTag(const httplib::Request& req,httplib::Response& rsp)
{
    LOG() << "正在插入标签" << endl;
    //获取json字符串
    std::string json_tag = req.body;
    //json字符串解析为Json::Value对象
    Json::Reader reader;
    Json::Value root;
    bool ret = reader.parse(json_tag,root);
    if(ret == false)
    {
        LOG() << "insert tag info parse json client error" << endl;
        rsp.status = 400;// 400 Bad Request 
        return ;
    }

    //调用table_tag 中Inser接口插入数据
    ret = table_tag->Insert(root);
    if(ret == false)
    {
        LOG() << "insert tag info db server error" << endl;
        rsp.status = 500;//服务器内部错误
        return ;
    }

    //填充rsp响应头部信息
    rsp.status = 200;
}

void DeleteTag(const httplib::Request& req,httplib::Response& rsp)
{
    LOG() << "正在删除标签" << endl;
    int tag_id = std::stoi(req.matches[1]);
    
    bool ret = table_tag->Delete(tag_id);
    if(ret == false)
    {
        LOG() << "delete tag from db server error" << endl;
        rsp.status = 500;
        return ;
    }
    rsp.status = 200;
}

void PutTag(const httplib::Request& req,httplib::Response& rsp)
{
    LOG() << "正在更新标签信息" << endl;
    std::string json_tag = req.body;
    int tag_id = std::stoi(req.matches[1]);

    Json::Reader reader;
    Json::Value root;
    bool ret = reader.parse(json_tag,root);
    if(ret == false)
    {
        LOG() << "update tag from parse client error\n" << endl;
        rsp.status = 400;
        return ;
    }

    ret = table_tag->Update(tag_id,root);
    if(ret == false)
    {
        LOG() << "update tag from db server error" << endl;
        rsp.status = 500;
        return ;
    }
    rsp.status = 200;

}

void GetAllTag(const httplib::Request& req,httplib::Response& rsp)
{
    LOG() << "正在获得所有标签信息" << endl;

    Json::Value root;
    bool ret = table_tag->GetAll(&root);
    if(ret == false)
    {
        LOG() << "get all tag info from db server error" << endl;
        rsp.status = 500;
        return ;
    }

    Json::FastWriter writer;
    std::string body = writer.write(root);
    // rsp.body = body;
    // void set_content(const char *s, size_t n, const char *content_type);
    rsp.set_content(&body[0],body.size(),"application/json");
    rsp.status = 200;

}

void GetOneTag(const httplib::Request& req,httplib::Response& rsp)
{
    LOG() << "正在读取指定标签信息" << endl;
    int tag_id = std::stoi(req.matches[1]);

    Json::Value root;
    bool ret = table_tag->GetOne(tag_id,&root);
    if(ret == false)
    {
        LOG() << "get onetag info from db server error" << endl;
        rsp.status = 500;
        return ;
    }
    
    Json::FastWriter writer;
    // void set_content(std::string s, const char *content_type);
    rsp.set_content(writer.write(root),"application/json");
    rsp.status = 200;

}

//博客表
void InsertBlog(const httplib::Request& req,httplib::Response& rsp)
{
    LOG() << "正在插入博客信息" << endl;
    //获取json字符串
    std::string json_blog = req.body;
    //json字符串解析为Json::Value对象
    Json::Reader reader;
    Json::Value root;
    bool ret = reader.parse(json_blog,root);
    if(ret == false)
    {
        LOG() << "insert blog info parse json client error" << endl;
        rsp.status = 400;// 400 Bad Request 
        return ;
    }

    //调用table_blog 中Inser接口插入数据
    ret = table_blog->Insert(root);
    if(ret == false)
    {
        LOG() << "insert blog info db server error" << endl;
        rsp.status = 500;//服务器内部错误
        return ;
    }

    //填充rsp响应头部信息
    rsp.status = 200;
}

void DeleteBlog(const httplib::Request& req,httplib::Response& rsp)
{
    LOG() << "正在删除博客" << endl;
    int blog_id = std::stoi(req.matches[1]);
    
    bool ret = table_blog->Delete(blog_id);
    if(ret == false)
    {
        LOG() << "delete blog from db server error" << endl;
        rsp.status = 500;
        return ;
    }
    rsp.status = 200;
}

void PutBlog(const httplib::Request& req,httplib::Response& rsp)
{
    LOG() << "正在更新博客信息" << endl;
    std::string json_blog = req.body;
    int blog_id = std::stoi(req.matches[1]);

    Json::Reader reader;
    Json::Value root;
    bool ret = reader.parse(json_blog,root);
    if(ret == false)
    {
        LOG() << "update blog from parse client error" << endl;
        rsp.status = 400;
        return ;
    }

    ret = table_blog->Update(blog_id,root);
    if(ret == false)
    {
        LOG() << "update blog from db server error" << endl;
        rsp.status = 500;
        return ;
    }
    rsp.status = 200;
}

/*  /blog           获取所有博客信息
 *  /blog?user_id   获取指定用户的所有博客信息
 *  /blog?tag_id    获取指定标签的所有博客信息
 */
void GetAllBlog(const httplib::Request& req,httplib::Response& rsp)
{
    LOG() << "正在获得所有博客信息" << endl;

    Json::Value root;
    if(req.has_param("user_id") == true)
    {
        int user_id = std::stoi(req.get_param_value("user_id"));
        bool ret = table_blog->GetUser(user_id,&root);
        if(ret == false)
        {
            LOG() << "get user allblogs info from db server error" << endl;
            rsp.status = 500;
            return ;
        }
    }
    else if(req.has_param("tag_id") == true)
    {
        int tag_id = std::stoi(req.get_param_value("tag_id"));
        bool ret = table_blog->GetTag(tag_id,&root);
        if(ret == false)
        {
            LOG() << "get tag allblogs info from db server error" << endl;
            rsp.status = 500;
            return ;
        }
    }
    else 
    {
        bool ret = table_blog->GetAll(&root);
        if(ret == false)
        {
            LOG() << "get all blog info from db server error" << endl;
            rsp.status = 500;
            return ;
        }
    }

    Json::FastWriter writer;
    std::string body = writer.write(root);
    // rsp.body = body;
    // void set_content(const char *s, size_t n, const char *content_type);
    rsp.set_content(&body[0],body.size(),"application/json");
    rsp.status = 200;
}

void GetOneBlog(const httplib::Request& req,httplib::Response& rsp)
{
    LOG() << "正在读取指定博客信息" << endl;
    int blog_id = std::stoi(req.matches[1]);

    Json::Value root;
    bool ret = table_blog->GetOne(blog_id,&root);
    if(ret == false)
    {
        LOG() << "get oneblog info from db server error" << endl;
        rsp.status = 500;
        return ;
    }
    
    Json::FastWriter writer;
    // void set_content(std::string s, const char *content_type);
    rsp.set_content(writer.write(root),"application/json");
    rsp.status = 200;
}

