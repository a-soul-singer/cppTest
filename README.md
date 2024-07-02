# 一、项目概要

项目名称: xx车辆维修管理系统

开发人员：8人 【前端3人，后端3人，实习生1人，PM1人】

项目架构：C/S架构

技术栈：C++11、Qt5.12、socket、多线程、json、数据库

开发工具：vscode、mobax、qt creator

开发环境：windows/linux

主要模块：

# 二、相关接口

## 2.1 前后端数据结构

**TLV协议： head + body**

```c++
T: 类型【用于标识请求类型和响应的类型】
L: 长度【用于标识整个数据包的字节大小】
V: 具体的数据包
```

```c++
enum Type {
    LOGIN_REQ, // request
    LOGIN_RES, // response
    USERS_REQ,
    USERS_RES,
};

struct Head
{
    int type;
    int length;
};

struct Data
{
    Head head;
    char body[0]; // body主要采用是json字符串形式
};
```

## 2.2 登录接口

```json
登录请求:
	head：
		type：0
		length：head + body
	body:
		{"username": "admin", "password": "admin123"};
登录响应:
	head：
		type：1
		length：head + body
	body:
		{	
			"data": "", 
			"code": "200/500", 
			"message": ""
		};
```

# 三、后端数据库

库名称：car_manage_system

## 3.1 用户表【t_user】

| 字段名称 | 类型    | 描述               |
| -------- | ------- | ------------------ |
| id       | int     | 主键，自增，用户ID |
| username | varchar | 用户名             |
| password | varchar | 用户密码           |
