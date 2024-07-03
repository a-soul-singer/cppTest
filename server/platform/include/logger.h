#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <vector>
#include <string>
#include <fstream>

/*
日志模块：
	支持多线程写入日志
	a.运行日志：
		(1)支持两种打印日志的方式(函数内部已实现换行，写日志时不用添加换行)
			LOG_INFO("程序正在运行...");
			LOG_ERROR("连接数据库失败，错误码为：%d, 错误原因为：%s", 10060, "用户名或密码不正确");
		(2)最多支持10个日志文件，每一个日志文件最大50M
*/

class RunLog
{
private:
	// 日志对象单例
	static RunLog* logIns;
	RunLog();
	~RunLog() = default;
	RunLog(const RunLog& other) = delete;
	RunLog& operator=(const RunLog& other) = delete;
public:
	// 日志文件对象
	static std::ofstream* logFileStream;
	// 当前日志文件夹路径
	static std::string logDirPath;
	// 获取当前运行日志类的单例
	static RunLog* getInstance();
	// 写入日志到文件的主要函数
	static void log(const char* filepath, const char* funcname, int line, const char* level, const char* msg, ...);
	// 获取当前时间
	static void getCurrentTime(char* currTime, size_t size, const char* format);
	// 生成日志文件的完整路径
	static void genFileName(const std::string& dirname, std::string& fullFileName);
	// 获取日志目录下的所有日志文件
	static void getFiles(const std::string& path, std::vector<std::string>& files, const char* fType);
	// 检查文件日志大小，判断是否需要换文件操作
	static void checkFileSize();
	// 程序异常情况下，可以调用该方法释放资源
	static void release();
};

#define LOG_INFO(msg, ...) RunLog::log(__FILE__, __FUNCTION__, __LINE__, "INFO", msg, ##__VA_ARGS__)
#define LOG_ERROR(msg, ...) RunLog::log(__FILE__, __FUNCTION__, __LINE__, "ERROR", msg, ##__VA_ARGS__)
#define LOG_DEBUG(msg, ...) RunLog::log(__FILE__, __FUNCTION__, __LINE__, "DEBUG", msg, ##__VA_ARGS__)
#define LOG_WARNING(msg, ...) RunLog::log(__FILE__, __FUNCTION__, __LINE__, "WARNING", msg, ##__VA_ARGS__)
#define LOG_FATAL(msg, ...) RunLog::log(__FILE__, __FUNCTION__, __LINE__, "FATAL", msg, ##__VA_ARGS__)


#endif