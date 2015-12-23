/*
*****************************************************************************************
* CopyRight
*
* FileName     :
* Author       :
* Description  :
* Function List:
* History      :
*****************************************************************************************
*/

#ifndef CLOG_H
#define CLOG_H

#include <assert.h>
#include <iostream>
#include <fstream>
using namespace std;

#ifndef _FL_
    #define _FL_ __FILE__, __LINE__
#endif

#ifndef NULL
    #define NULL 0
#endif

class CLog
{
public:
    //构造函数与析构函数
    CLog();
    ~CLog();

    //初始化函数
    int Init(const char* pszFile);

    //记录提示信息
    void LogInfo(const char* pszSrc, int iLine, const char* pszInfo, ...);

    //记录警告日志
    void LogWarn(const char* pszSrc, int iLine, const char* pszInfo, ...);

    //记录错误日志
    void LogErr(const char* pszSrc, int iLine, const char* pszInfo, ...);

    //记录开始日志
    void LogStart(const char* pszProgram, int argc, char* argv[]);

    //记录结束日志
    void LogEnd();

private:
    void GetTimeStr();

    enum
    {
        EN_MAXBUFLEN      = 4096,
        EN_MAXFILENAMELEN = 512
    };

    ofstream m_ofsLog;                          //文件输出流
    char*    m_pszBuffer;                       //日志缓冲区指针
    char     m_szNow[24];                       //当前时间字符串
    char     m_szFile[EN_MAXFILENAMELEN];       //日志文件名
};

#endif /*CLOG_H*/

/********************************** END ************************************************/

