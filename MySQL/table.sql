
create database if not exists blog_system;

use blog_system;

create table if not exists tag_blog
(
    id int primary key auto_increment comment '博客标签id',
    name varchar(32) unique comment '标签名称'
)charset=utf8;

create table if not exists user_blog
(
    id int primary key auto_increment comment '博客用户id',
    name varchar(32) comment '用户名称',
    unique key(name)
)charset=utf8;

create table if not exists blog
(
    id int primary key auto_increment comment '博客id',
    tag_id int comment '所属标签id',
    user_id int comment '所属用户id',
    title varchar(255) comment '博客题目',
    content text comment '博客正文',
    ctime datetime comment '博客最后修改日期',
    foreign key(tag_id) references tag_blog(id),
    foreign key(user_id) references user_blog(id)
)charset=utf8;
