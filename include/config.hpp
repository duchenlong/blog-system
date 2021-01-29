#pragma once

#include "head.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include <list>

using namespace std;

struct node {
    string _key;
    string _value;
    node(const string& key,const string& value)
        :_key(key),_value(value)
        {}
};

// 存储配置文件的数据结构链表
static list<node*>      _info;

// 链表中节点的数量
static size_t           _size = 0;

// 读取配置文件中的信息    
bool read(const string fileName = "my.conf");

// 得到字符串类型的 value         
const string GetString(const string key); 
// 得到C 风格类型的字符串
const char* GetCString(const string key);

// 得到int 类型的数据，默认值为 del      
const int GetInt(const string key,const int del);
