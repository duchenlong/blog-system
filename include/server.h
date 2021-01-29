#pragma once 

#include "serverFunction.h"

bool HttpLink()
{
    LOG() << "httplib set httpserver" << endl;
    httplib::Server server;
    
    //设置url 中资源的相对路径
    server.set_base_dir(GetCString("wwwroot"));
    //使用 正则匹配 

    //user 用户表的操作
    //新增用户信息
    server.Post(R"(/user)",InsertUser);
    //删除用户信息，所传参数为用户 id 
    server.Delete(R"(/user/(\d+))",DeleteUser);
    //更新用户信息, 所传参数为用户 id
    server.Put(R"(/user/(\d+))",PutUser);
    //获取所有用户信息
    server.Get(R"(/user)",GetAllUser);
    //获取指定用户信息, 所传参数用户 id
    server.Get(R"(/user/(\d+))",GetOneUser);

    //tag 标签表的操作
    //新增标签信息
    server.Post(R"(/tag)",InsertTag);
    //删除标签信息，所传参数为标签 id 
    server.Delete(R"(/tag/(\d+))",DeleteTag);
    //更新标签信息, 所传参数为标签 id
    server.Put(R"(/tag/(\d+))",PutTag);
    //获取所有标签信息
    server.Get(R"(/tag)",GetAllTag);
    //获取指定标签信息, 所传参数标签 id
    server.Get(R"(/tag/(\d+))",GetOneTag);
    
    //blog 博客表的操作
    //新增博客信息
    server.Post(R"(/blog)",InsertBlog);
    //删除博客信息，所传参数为博客 id 
    server.Delete(R"(/blog/(\d+))",DeleteBlog);
    //更新博客信息, 所传参数为博客 id
    server.Put(R"(/blog/(\d+))",PutBlog);
    //获取所有博客信息  
    // /blog            所有博客  
    // /blog?tag_id=    查询指定标签的所有博客
    // /blog?user_id=   查询指定用户的所有博客
    server.Get(R"(/blog)",GetAllBlog);
    //获取指定博客信息, 所传参数博客 id
    server.Get(R"(/blog/(\d+))",GetOneBlog);
    LOG() << "ing Listen port" << endl;

    server.listen(GetCString("listenIp"),GetInt("listenPort",LISTEN_PORT));
    return true;
}
