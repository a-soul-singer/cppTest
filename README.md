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
		{"action": "login", "username": "admin", "password": "admin123"};
登录响应:
	head：
		type：1
		length：head + body
	body:
		{	
			"data": {"action": "login"}, 
			"code": "200/500", 
			"message": ""
		};
==================================================================================================	
注册请求:
	head：
		type：0
		length：head + body
	body:
		{"action": "register", "username": "admin", "password": "admin123"};
注册响应:
	head：
		type：1
		length：head + body
	body:
		{	
			"data": {"action": "register"}, 
			"code": "200/500", 
			"message": ""
		};
```

## 2.3 操作日志接口

```
登录请求:
	head：
		type：4
		length：head + body
	body:
		{
			"username": "admin", 
			"each_page_cnt": 20, // 每一页展示的个数
			"curr_page": 20, // 当前页
		};
登录响应:
	head：
		type：5
		length：head + body
	body:
		{	
			"data": {
				"logs": [[]],
				"curr_page": 20,
				"total_page": 25,
			}, 
			"code": "200/500", 
			"message": ""
		};
```

**limit_cnt = each_page_cnt;**

**offset_cnt = (curr_page - 1) * each_page_cnt;**

**select * from t_opt_log where name = 'username' limit   limit_cnt  offset offset_cnt ;**

## 2.4 用户管理的接口

```json
查询请求:
	head：
		type：2
		length：head + body
	body:
		{
			"username": "admin", 
			"action": "query/delete/update/add",
		};
查询响应:
	head：
		type：3
		length：head + body
	body:
		{	
			"data": {
				"action": "query",
				"users": [[]]
			}, 
			"code": "200/500", 
			"message": ""
		};
==================================================================================================		
删除请求:
	head：
		type：2
		length：head + body
	body:
		{
			"username": "admin", 
			"action": "delete",
			"userIds": []
		};
删除响应:
	head：
		type：3
		length：head + body
	body:
		{	
			"data": {
				"action": "delete"
			}, 
			"code": "200/500", 
			"message": ""
		};
==================================================================================================	
修改请求:
	head：
		type：2
		length：head + body
	body:
		{
			"username": "admin", 
			"action": "update",
			"userInfos": [
				[],
				[]
			]
		};
修改响应:
	head：
		type：3
		length：head + body
	body:
		{	
			"data": {
				"action": "update"
			}, 
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

## 3.2 操作日志表 【t_opt_log】

| 字段名称   | 类型     | 描述               |
| ---------- | -------- | ------------------ |
| id         | int      | 主键，自增，用户ID |
| type       | varchar  | 操作类型           |
| action     | varchar  | 操作动作           |
| user       | varchar  | 操作者             |
| start_time | datetime | 开始时间           |
| end_time   | datetime | 结束时间           |
| result     | varchar  | 操作详情           |

**eg:  1 -  登录 | 登录 | admin | 2024-07-09 18:29:53 | 2024-07-09 18:29:53 | 登录成功**

# 四、问题

## 4.1 服务端和客户端通信时，由于数据量大，造成缓冲区溢出问题

```
问题现象：
	服务端发送数据，到达客户端，客户端未能完全接收
日志现象：
	日志方式进行打印：服务端实际发送大小和客户端接收大小不一致
	并且，客户端接收大小 小于 服务端实际发送大小
怀疑:
	TCP传输过程中出现了丢包问题
策略：
	首先使用tcpdump命令进行抓包
	然后将抓到的包使用wireshark进行分析
查询资料：
	在wireshark出现
	tcp window full：发送端向接收端发送的窗口警示，表示服务端发送窗口已满，不能再发送
	tcp zerowindow：接收端向发送端的窗口警示，表示客户端接收窗口已满，不能再接收
	tcp window update：当接收方应用程序处理了部分数据或者释放了一些接收缓冲区空间时，
	它可以通过TCP报文中的确认（ACK）来告知发送方，它可以接收更多数据。这个过程被称为TCP窗口更新。
分析得到：
	该情况是由于发送端和接收端的缓冲区的大小是有限制，主要是因为缓冲区被占满
	发送端和接收端的速率不一致有关系
解决方法：
	修改setsockopt函数，调整服务端和客户端发送和接收缓冲区的大小
	修改发送端的代码，将之前的一次发送，修改为分批发送
	修改客户端的代码，将之前的一次接收，修改为分批接收
```

## 4.2 客户端主动断开连接后，服务端卡死

```
现象：服务端重复读取，然后日志一直重复输出，程序陷入死循环
排查代码：
	当客户端断开时，服务端已经不能获取到数据啦，然后没有进行错误保护，导致进入死循环
解决方法：
	对read函数的返回值，进行判断，如果出现-1或者0的情况，则跳出循环，关闭socket
```

# 五、增加新模块，需要做的改动

## 5.1 服务端

（1）增加一个socket_data, 枚举值

（2）增加一个cotroller类的子类

（3）在map中配置枚举值和cotroller类的映射关系

**备注：如果你要增加一个子功能，可以使用增加action的方式**

## 5.2 客户端

（1）增加一个socket_data, 枚举值

（2）增加一个UI界面，把这个UI界面加入到stack，可以实现切换

（3）实现对应的功能函数

```
当前UI界面：
    public:
        void changeWindowRequest(); // 切换界面时要做的请求
        void handleResponse(const QJsonObject& obj); // 获取对应得响应

    signals:
        void sendSocketData(int type, QJsonObject& obj); // 发送请求的信号

主界面：
	// 将UI界面的信号和主界面的槽函数进行绑定，实现发送数据的统一处理
	connect(m_optlogview, &OptLogView::sendSocketData, this, &HomeWidget::handleSendSocketData);
	
	
	// 在主界面上的handleReadyRead这个函数中对服务端给你返回的响应做统一处理
```
