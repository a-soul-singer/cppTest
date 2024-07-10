#include "logger.h"

#include <filesystem>
#include <mutex>
#include <algorithm>
#include <ctime> // time.h
#include <cstring> // string.h
#include <cstdarg> // stdarg.h
#include <sys/types.h>
#include <sys/stat.h>
#include <thread>
#include <unistd.h>

using namespace std;
using namespace this_thread;
using namespace filesystem;

namespace LogConstant
{
	const size_t timeMaxLen = 64; // 时间字符串的最大长度
	const size_t pathMaxLen = 1024; // 路径的最长长度
	const size_t fullLogMaxLen = 2048; // 临时的字符数组最大长度
	const size_t charMaxLen = 1024; // 临时的字符数组最大长度
	const size_t fileMaxSize = 10 * 1024 * 1024; // 单个日志文件最大大小
	const size_t fileMaxNum = 10; // 日志文件最大数量
	const char* logFileSuffix = ".log"; // 日志文件后缀
	const char* logFilePrefix = "car_manage_sys"; // 日志文件前缀，一般为项目名称 
	const char* logMsgTimeFmt = "%Y-%m-%d %H:%M:%S"; // 日志消息体中的时间格式
	const char* logFileTimeFmt = "%Y-%m-%d_%H%M%S"; // 日志文件名中的时间格式
};

// 日志对象单例
RunLog* RunLog::logIns = nullptr;
// 日志文件对象
ofstream* RunLog::logFileStream = nullptr;
// 当前日志文件夹路径
string RunLog::logDirPath = "";

// 日志对象互斥锁
static mutex g_logInsLock;
// 日志写入锁
static mutex g_logWriteLock;

RunLog::RunLog()
{
	string fullFileName;
	char currPath[LogConstant::pathMaxLen] = {0};
	getcwd(currPath, sizeof(currPath)); // 获取当前目录
	strcat(currPath, "/../log"); // 存放日志的目录

	// 如果文件夹不存在，则创建文件夹
	struct stat info;
	if (stat(currPath, &info) != 0) {
		mkdir(currPath, 0775);
	}
	RunLog::logDirPath = currPath;

	// 文件夹如果存在，则获取当前目录下所有文件
	vector<string> files;
	getFiles(currPath, files, LogConstant::logFileSuffix);

	if (files.empty()) {
		genFileName(currPath, fullFileName);
	} else {
		sort(files.begin(), files.end());
		string lastNewFileName = files.back(); // 查找最新的日志文件

		// 查看最新的日志文件大小
		ifstream ifs(lastNewFileName);
		ifs.seekg(ios::end);
		long long size = ifs.tellg();
		ifs.close();

		// 如果文件大小超过LogConstant::fileMaxSize，则生成新的日志文件名
		if (size >= LogConstant::fileMaxSize) {
			// 如果文件个数超过限制，则删除存放时间最久的日志文件
			if (files.size() >= LogConstant::fileMaxNum) {
				remove(files.front().c_str());
			}
			genFileName(currPath, fullFileName);
			if (logFileStream != nullptr) {
				logFileStream->close();
				delete logFileStream;
			}
		} else {
			fullFileName = lastNewFileName;
		}
	}
	logFileStream = new ofstream(fullFileName, ios::app);
}

void RunLog::release()
{
	if (logIns != nullptr) {
		delete logIns;
		logIns = nullptr;
	}
	if (logFileStream != nullptr) {
		logFileStream->close();
		delete logFileStream;
		logFileStream = nullptr;
	}
}

RunLog* RunLog::getInstance()
{
	if (logIns == nullptr) {
		lock_guard<mutex> lgLogInsLock(g_logInsLock);
		if (logIns == nullptr) {
			logIns = new RunLog();
		}
	}
	return logIns;
}

void RunLog::genFileName(const string& dirname, string& fullFileName)
{
	char currTime[LogConstant::timeMaxLen] = {0};
	getCurrentTime(currTime, sizeof(currTime), LogConstant::logFileTimeFmt);
	// 完整文件名： 文件夹 + "/" + 前缀 + "_" + 时间 + 后缀
	fullFileName = dirname + "/" + LogConstant::logFilePrefix + "_" + currTime + LogConstant::logFileSuffix;
}

void RunLog::getFiles(const string& path, vector<string>& files, const char* fType)
{
    for (const directory_entry& entry : directory_iterator(path)) {
        if (!entry.is_directory()) {
            files.push_back(entry.path().string());
        }
    }
}

void RunLog::checkFileSize()
{
	if (logFileStream != nullptr) {
		long long size = logFileStream->tellp();
		// 如果文件大小超过LogConstant::fileMaxSize，则生成新的日志文件名
		if (size >= LogConstant::fileMaxSize) {
			// 如果文件个数超过限制，则删除存放时间最久的日志文件
			vector<string> files;
			getFiles(RunLog::logDirPath, files, LogConstant::logFileSuffix);
			if (files.size() >= LogConstant::fileMaxNum) {
				remove(files.front().c_str());
			}
			string fullFileName;
			genFileName(RunLog::logDirPath, fullFileName);
			if (logFileStream != nullptr) {
				logFileStream->close();
				delete logFileStream;
			}
			logFileStream = new ofstream(fullFileName, ios::app);
		}
	}
}

void RunLog::getCurrentTime(char* currTime, size_t size, const char* format)
{
	time_t _time = time(nullptr);
    tm* _tm = localtime(&_time);
	// strftime(char * __restrict__ _Buf,size_t _SizeInBytes,const char * __restrict__ _Format,const struct tm * __restrict__ _Tm);
	strftime(currTime, size, format, _tm);
}

void RunLog::log(const char* filepath, const char* funcname, int line, const char* level, const char* msg, ...)
{
	// 生成日志对象，并生成日志文件对象
	getInstance();

	{
		// 多线程写入文件要进行加锁处理
		lock_guard<mutex> lgLogCheckLock(g_logWriteLock);
		// 检查文件大小
		checkFileSize();
	}

	// 获取当前的时间
	char currTime[LogConstant::timeMaxLen] = {0};
	getCurrentTime(currTime, sizeof(currTime), LogConstant::logMsgTimeFmt);

	// 支持windows的文件路径
	char* p = strrchr(const_cast<char*>(filepath), '\\');
	if (p == nullptr) {
		// 支持linux的文件路径
		p = strrchr(const_cast<char*>(filepath), '/');
	}

	// 获取不定长参数
	char formatMsg[LogConstant::charMaxLen] = {0};
	va_list vaList;
	va_start(vaList, msg);
	vsprintf(formatMsg, msg, vaList);
	va_end(vaList);

	// 拼接最终的日志格式： 时间 [filename:文件名] [funcname:函数名] [line:行号] [tid:线程ID] [日志级别] msg
	char fullMsg[LogConstant::fullLogMaxLen] = {0};
	sprintf(fullMsg, "%s [filename:%s] [funcname:%s] [line:%d] [tid:%lld] [%s] %s\n",
			  currTime, p + 1, funcname, line, get_id(), level, formatMsg);

	cout << fullMsg;
	// 写入文件中
	logFileStream->write(fullMsg, strlen(fullMsg));
	logFileStream->flush();
}
