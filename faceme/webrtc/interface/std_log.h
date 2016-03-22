#ifndef __STD_LOG_HPP__
#define __STD_LOG_HPP__
#include <sstream>

// 标准日志函数指针
extern void voipLog(const std::string& str, int nType);

enum {
	LOGDEBUG = 0,
	LOGINFO,
	LOGWARNING,
	LOGERROR,
};

class CStdLog{
public:

	inline CStdLog(int type) 
		:nType(type)
		, szFile(NULL)
		, nLine(0)
		, szFunc(NULL){
	};

	inline CStdLog(int type
		, const char* file
		, const char* func
		, int line)
		:nType(type)
		, szFile(file)
		, nLine(line)
		, szFunc(func){
	};

	inline ~CStdLog()
	{
#ifdef __DEBUG__
		if (szFunc)
			strContent<<"\t\t\tfunc: "<<szFunc;
		if (nLine)
			strContent<< "   line: " << nLine;
		if (szFile)
			strContent<<"   file: "<<szFile;
#endif//__DEBUG__

		const std::string& strLog = strContent.str();
		if (!strLog.empty())
		{
			voipLog(strLog, nType);
		}
	}

	template <typename T>
	inline CStdLog& operator<<(const T& t)
	{
		strContent << t;
		return *this;
	}
	
private:
	int nType;
	std::stringstream strContent;
	const char* szFile;
	int nLine;
	const char* szFunc;
};


// debug
#ifdef __DEBUG__
#define logD CStdLog(LOGDEBUG, __FILE__, __FUNCTION__, __LINE__)
#else
#define logD CStdLog(LOGDEBUG)
#endif

// info
#define logI CStdLog(LOGINFO)
// warning
#define logW CStdLog(LOGWARNING)
// error
#define logE CStdLog(LOGERROR)


/* example


void test__()
{
	std::string str("test 3,hahaha!");
	LogD<< "test only";
	LogD<< "test 1" << " good bye";
	LogD<< "test 2" << " good bye " << 2;
	LogD << str;
}
*/


#endif//__STD_LOG_HPP__