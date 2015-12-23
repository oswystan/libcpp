/*
************************************************************************************
* CopyRight (C) WangYu
*
* FileName     : util.cpp
* Author       : WangYu
* Description  : 工具类库
* Function List: static int GetHostIP(char* pszIP);
*                static int GetHostName(char* pszHostName);
*                static int GetLoginName(char* pszUsr);
*                static int IsNumber(const char* pszSrc, int iIsNull = 0);
*                static int IsInt(const char* pszSrc, int iIsNull = 0);
*                static int IsFloat(const char* pszSrc, int iIsNull = 0);
*                static int IsDate(const char* pszSrc);
*                static int IsTime(const char* pszSrc);
*                static int IsDateTime(const char* pszSrc);
*                static int GetCurDate(char* pszOut);
*                static int GetCurTime(char* pszOut);
*                static int GetCurDateTime(char* pszOut);
*                static int GetFileSize(const char* pszFile);
*                static int IsFileExist(const char* pszFile);
*                static int IsDirExist(const char* pszDir);
*                static int MakeDir(const char* pszDir);
*                static int RemoveFile(const char* pszFile);
*                static int GetDirFiles(const char* pszDir, vector<string>& vecFiles);
*                static char* Trim(char* pszSrc);
*                static int TouchFile(const char* pszFile);
*                static int MoveFile(const char* pszSrc, const char* pszDest);
* History      : 2007-04-05 初稿完成
************************************************************************************
*/

#include <unistd.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <assert.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>

#include <iostream>
using namespace std;

#include "lib_util.h"

/****************************************************************
* Function Name : GetHostIP();
* Description   : 获取当前主机IP;
* Calls         : NA;
* Called By     : NA;
* Input         : NA;
* Output        : pszIP: 主机的IP地址;
* Return        : 0: 成功; Other: 失败;
* Table Accessed: NA;
* Table Updated : NA;
* Other         : 本函数获取/etc/hosts文件中的内容;
****************************************************************/
int CUtil::GetHostIP(char* pszIP)
{
    assert(pszIP);

    int  iErrNo = 0;
    char szHost[EN_MAX_HOSTNAME] = {0};

    *pszIP = '\0';
    if(0 != gethostname(szHost, EN_MAX_HOSTNAME))
    {
        iErrNo = errno;
        return iErrNo;
    }

    hostent * pHost = NULL;
    pHost = gethostbyname(szHost);
    if(NULL == pHost)
    {
        iErrNo = h_errno;
        return iErrNo;
    }

    char* pszStrIP = NULL;
    pszStrIP = inet_ntoa(*((in_addr*)pHost->h_addr_list[0]));
    sprintf(pszIP, "%s", pszStrIP);

    return 0;
}

/****************************************************************
* Function Name : GetHostName();
* Description   : 获取当前主机的主机名;
* Calls         : NA;
* Called By     : NA;
* Input         : NA;
* Output        : pszHostName: 输出主机名;
* Return        : 0: 成功; Other: 失败;
* Table Accessed: NA;
* Table Updated : NA;
* Other         : NA;
****************************************************************/
int CUtil::GetHostName(char* pszHostName)
{
    assert(pszHostName);

    int iErrNo = 0;

    if(0 != gethostname(pszHostName, EN_MAX_HOSTNAME))
    {
        iErrNo       = errno;
        *pszHostName = '\0';
    }

    return iErrNo;
}

/****************************************************************
* Function Name : GetLoginName();
* Description   : 获取当前登陆的用户名;
* Calls         : NA;
* Called By     : NA;
* Input         : NA;
* Output        : pszUsr: 输出用户名;
* Return        : 0: 成功; Other: 失败;
* Table Accessed: NA;
* Table Updated : NA;
* Other         : NA;
****************************************************************/
int CUtil::GetLoginName(char* pszUsr)
{
    assert(pszUsr);

    int iErrNo = 0;

    if(NULL == cuserid(pszUsr))
    {
        iErrNo  = errno;
        *pszUsr = '\0';
    }

    return iErrNo;
}

/****************************************************************
* Function Name : IsNumber();
* Description   : 判断输入串是否时数字字符串;
* Calls         : NA;
* Called By     : NA;
* Input         : pszSrc: 输入串; iIsNull: 表示输入串是否可以为空;
* Output        : NA;
* Return        : 0: 合法; Other: 非法;
* Table Accessed: NA;
* Table Updated : NA;
* Other         : NA;
****************************************************************/
int CUtil::IsNumber(const char* pszSrc, int iIsNull)
{
    assert(pszSrc);

    if(0 == iIsNull && '\0' == *pszSrc)
    {
        return 1;
    }

    while('\0' != *pszSrc)
    {
        if( *pszSrc < '0' || *pszSrc > '9')
        {
            return 1;
        }

        ++pszSrc;
    }

    return 0;
}

/****************************************************************
* Function Name : IsInt();
* Description   : 判断输入字符串是否是一个int类型;
* Calls         : NA;
* Called By     : NA;
* Input         : pszSrc: 输入串; iIsNull: 表示输入串是否可以为空;
* Output        : NA;
* Return        : 0: 合法; Other: 非法;
* Table Accessed: NA;
* Table Updated : NA;
* Other         : NA;
****************************************************************/
int CUtil::IsInt(const char* pszSrc, int iIsNull)
{
    assert(pszSrc);

    const char* pszStr = pszSrc;

    if(0 == iIsNull && '\0' == *pszSrc)
    {
        return 1;
    }

    //判断第一个字符是否是'-'
    if('-' == *pszStr)
    {
        ++pszStr;

        //处理只有一个'-'的情况
        if(*pszStr < '0' || *pszStr > '9')
        {
            return 1;
        }
    }

    while('\0' != *pszStr)
    {
        if(*pszStr < '0' || *pszStr > '9')
        {
            return 1;
        }

        ++pszStr;
    }

    return 0;
}

/****************************************************************
* Function Name : IsFloat();
* Description   : 判断输入字符串是否是一个合法的float类型;
* Calls         : NA;
* Called By     : NA;
* Input         : pszSrc: 输入串; iIsNull: 表示输入串是否可以为空;
* Output        : NA;
* Return        : 0: 合法; Other: 非法;
* Table Accessed: NA;
* Table Updated : NA;
* Other         : NA;
****************************************************************/
int CUtil::IsFloat(const char* pszSrc, int iIsNull)
{
    assert(pszSrc);

    if(0 == iIsNull && '\0' == *pszSrc)
    {
        return 1;
    }

    //判断第一个字符是否是'-'
    if('-' == *pszSrc)
    {
        ++pszSrc;

        //处理只有一个'-'的情况
        if(*pszSrc < '0' || *pszSrc > '9')
        {
            return 1;
        }
    }
    else if('.' == *pszSrc)
    {
        return 1;
    }

    int iDot = 0;

    while('\0' != *pszSrc)
    {
        if(*pszSrc >= '0' && *pszSrc <='9')
        {
            ++pszSrc;
            continue;
        }
        else if('.' == *pszSrc)
        {
            if(1 == iDot)
            {
                return 1;
            }
            else
            {
                iDot = 1;
                ++pszSrc;
                continue;
            }
        }
        else
        {
            return 1;
        }
    }

    //处理最后一个字符为'.'
    if('.' == *(--pszSrc))
    {
        return 1;
    }

    return 0;
}

/****************************************************************
* Function Name : IsDate();
* Description   : 判断输入串是否是一个合法日期, 格式YYYYMMDD;
* Calls         : NA;
* Called By     : NA;
* Input         : pszSrc: 输入串;
* Output        : NA;
* Return        : 0: 合法; Other: 非法;
* Table Accessed: NA;
* Table Updated : NA;
* Other         : NA;
****************************************************************/
int CUtil::IsDate(const char* pszSrc)
{
    assert(pszSrc);

    if(8 != strlen(pszSrc))
    {
        return 1;
    }

    const char* pszTmp = pszSrc;

    while('\0' != *pszTmp)
    {
        if( *pszTmp < '0' || *pszTmp > '9')
        {
            return 1;
        }

        ++pszTmp;
    }

    char szTmp[8] = {0};
    int  iMaxDay[]={31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int  iYear;
    int  iMonth;
    int  iDay;

    strncpy(szTmp, pszSrc + 6, 2);
    iDay = atoi(szTmp);
    strncpy(szTmp, pszSrc + 4, 2);
    iMonth = atoi(szTmp);
    strncpy(szTmp, pszSrc + 0, 4);
    iYear = atoi(szTmp);

    if(iMonth < 1 || iMonth >= 12)
    {
        return 1;
    }

    //闰年判断
    if((0 == iYear % 4 && 0 != iYear % 100) || 0 == iYear % 400)
    {
        iMaxDay[1] = 29;
    }

    if(iDay > iMaxDay[iMonth - 1])
    {
        return 1;
    }

    return 0;
}

/****************************************************************
* Function Name : IsTime();
* Description   : 判断输入串是否是一个合法实践, 格式HHMMSS;
* Calls         : NA;
* Called By     : NA;
* Input         : pszSrc: 输入串;
* Output        : NA;
* Return        : 0: 合法; Other: 非法;
* Table Accessed: NA;
* Table Updated : NA;
* Other         : NA;
****************************************************************/
int CUtil::IsTime(const char* pszSrc)
{
    assert(pszSrc);

    if(6 != strlen(pszSrc))
    {
        return 1;
    }

    const char* pszTmp = pszSrc;

    while('\0' != *pszTmp)
    {
        if( *pszTmp < '0' || *pszTmp > '9')
        {
            return 1;
        }

        ++pszTmp;
    }

    char szTmp[8] = {0};
    int  iHour;
    int  iMin;
    int  iSec;

    strncpy(szTmp, pszSrc + 4, 2);
    iSec = atoi(szTmp);
    strncpy(szTmp, pszSrc + 2, 2);
    iMin = atoi(szTmp);
    strncpy(szTmp, pszSrc + 0, 2);
    iHour = atoi(szTmp);

    if(iHour > 23 || iMin > 59 || iSec > 59)
    {
        return 1;
    }

    return 0;
}

/****************************************************************
* Function Name : IsDateTime();
* Description   : 判断输入串是否是一个合法实践, 格式YYYYMMDDHHMMSS;
* Calls         : NA;
* Called By     : NA;
* Input         : pszSrc: 输入串;
* Output        : NA;
* Return        : 0: 合法; Other: 非法;
* Table Accessed: NA;
* Table Updated : NA;
* Other         : NA;
****************************************************************/
int CUtil::IsDateTime(const char* pszSrc)
{
    assert(pszSrc);

    if(14 != strlen(pszSrc))
    {
        return 1;
    }

    const char* pszTmp = pszSrc;

    while('\0' != *pszTmp)
    {
        if( *pszTmp < '0' || *pszTmp > '9')
        {
            return 1;
        }

        ++pszTmp;
    }

    char szTmp[8] = {0};
    int  iMaxDay[]={31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int  iYear;
    int  iMonth;
    int  iDay;
    int  iHour;
    int  iMin;
    int  iSec;

    strncpy(szTmp, pszSrc + 12, 2);
    iSec = atoi(szTmp);
    strncpy(szTmp, pszSrc + 10, 2);
    iMin = atoi(szTmp);
    strncpy(szTmp, pszSrc + 8, 2);
    iHour = atoi(szTmp);
    strncpy(szTmp, pszSrc + 6, 2);
    iDay = atoi(szTmp);
    strncpy(szTmp, pszSrc + 4, 2);
    iMonth = atoi(szTmp);
    strncpy(szTmp, pszSrc + 0, 4);
    iYear = atoi(szTmp);

    if(iMonth < 1 || iMonth >= 12 || iHour > 23 || iMin > 59 || iSec > 59)
    {
        return 1;
    }

    //闰年判断
    if((0 == iYear % 4 && 0 != iYear % 100) || 0 == iYear % 400)
    {
        iMaxDay[1] = 29;
    }

    if(iDay > iMaxDay[iMonth - 1])
    {
        return 1;
    }

    return 0;
}

/****************************************************************
* Function Name : GetCurDate();
* Description   : 获取系统当前日期;
* Calls         : NA;
* Called By     : NA;
* Input         : NA;
* Output        : pszOut: 当前系统日期, 格式YYYYMMDD;
* Return        : 0: 成功; 1: 失败;
* Table Accessed: NA;
* Table Updated : NA;
* Other         : NA;
****************************************************************/
int CUtil::GetCurDate(char* pszOut)
{
    assert(pszOut);

    time_t stNow;
    tm     stTm;

    stNow = time(NULL);
    if(NULL == localtime_r(&stNow, &stTm))
    {
        return 1;
    }

    sprintf(pszOut, "%d%02d%2d", stTm.tm_year + 1900, stTm.tm_mon + 1,
              stTm.tm_mday);

    return 0;
}

/****************************************************************
* Function Name : GetCurTime();
* Description   : 获取系统当前时间;
* Calls         : NA;
* Called By     : NA;
* Input         : NA;
* Output        : pszOut: 当前系统时间, 格式HHMMSS;
* Return        : 0: 成功; 1: 失败;
* Table Accessed: NA;
* Table Updated : NA;
* Other         : NA;
****************************************************************/
int CUtil::GetCurTime(char* pszOut)
{
    assert(pszOut);

    time_t stNow;
    tm     stTm;

    stNow = time(NULL);
    if(NULL == localtime_r(&stNow, &stTm))
    {
        return 1;
    }

    sprintf(pszOut, "%02d%02d%02d",stTm.tm_hour, stTm.tm_min, stTm.tm_sec);

    return 0;
}

/****************************************************************
* Function Name : GetCurDateTime();
* Description   : 获取系统当前日期时间;
* Calls         : NA;
* Called By     : NA;
* Input         : NA;
* Output        : pszOut: 当前系统时间, 格式YYYYMMDDHHMMSS;
* Return        : 0: 成功; 1: 失败;
* Table Accessed: NA;
* Table Updated : NA;
* Other         : NA;
****************************************************************/
int CUtil::GetCurDateTime(char* pszOut)
{
    assert(pszOut);

    time_t stNow;
    tm     stTm;

    stNow = time(NULL);
    if(NULL == localtime_r(&stNow, &stTm))
    {
        return 1;
    }

    sprintf(pszOut, "%d%02d%2d%02d%02d%02d", stTm.tm_year + 1900, stTm.tm_mon + 1,
              stTm.tm_mday, stTm.tm_hour, stTm.tm_min, stTm.tm_sec);

    return 0;
}

/****************************************************************
* Function Name : GetFileSize();
* Description   : 获取指定文件大小;
* Calls         : NA;
* Called By     : NA;
* Input         : pszFile();
* Output        : NA;
* Return        : -1: 失败; Other: 文件大小;
* Table Accessed: NA;
* Table Updated : NA;
* Other         : NA;
****************************************************************/
int CUtil::GetFileSize(const char* pszFile)
{
    assert(pszFile);

    struct stat statFile;

    if(0 != stat(pszFile, &statFile))
    {
        return -1;
    }

    return statFile.st_size;
}

/****************************************************************
* Function Name : IsFileExist();
* Description   : 判断指定文件是否存在;
* Calls         : NA;
* Called By     : NA;
* Input         : pszFile: 指定文件名;
* Output        : NA;
* Return        : 0: 存在; -1: 非文件; Other: 出错;
* Table Accessed: NA;
* Table Updated : NA;
* Other         : NA;
****************************************************************/
int CUtil::IsFileExist(const char* pszFile)
{
    assert(pszFile);

    struct stat statDir;

    if(0 != stat(pszFile, &statDir))
    {
        return errno;
    }

    if(!S_ISREG(statDir.st_mode))
    {
        return -1;
    }

    return 0;
}

/****************************************************************
* Function Name : IsDirExist();
* Description   : 判断指定目录是否存在;
* Calls         : NA;
* Called By     : NA;
* Input         : pszFile: 指定目录名;
* Output        : NA;
* Return        : 0: 存在; -1: 非目录; Other: 出错;
* Table Accessed: NA;
* Table Updated : NA;
* Other         : NA;
****************************************************************/
int CUtil::IsDirExist(const char* pszDir)
{
    assert(pszDir);

    struct stat statDir;

    if(0 != stat(pszDir, &statDir))
    {
        return errno;
    }

    if(!S_ISDIR(statDir.st_mode))
    {
        return -1;
    }

    return 0;
}

/****************************************************************
* Function Name : MakeDir();
* Description   : 创建制定目录;
* Calls         : NA;
* Called By     : NA;
* Input         : pszDir 目录名;
* Output        : NA;
* Return        : 0    : 成功;
*                 -1   : 存在, 但为文件;
*                 Other: errno;
* Table Accessed: NA;
* Table Updated : NA;
* Other         : 如果目录存在会返回成功;
****************************************************************/
int CUtil::MakeDir(const char* pszDir)
{
    assert(pszDir);

    int iErrNo = 0;

    //创建目录, 默认权限为:0750
    if(0 != mkdir(pszDir, S_IRWXU|S_IRGRP|S_IXGRP|S_IROTH))
    {
        iErrNo = errno;
        if(EEXIST != iErrNo)
        {
            return iErrNo;
        }

        //已经存在, 判断是否为目录
        struct stat stDir;
        if(0 != stat(pszDir, &stDir))
        {
            return errno;
        }

        if(!S_ISDIR(stDir.st_mode))
        {
            return -1;
        }

    }

    return 0;
}

/****************************************************************
* Function Name : RemoveFile();
* Description   : 删除指定文件;
* Calls         : NA;
* Called By     : NA;
* Input         : pszFile: 指定文件;
* Output        : NA;
* Return        : 0: 成功; Other: 失败;
* Table Accessed: NA;
* Table Updated : NA;
* Other         : NA;
****************************************************************/
int CUtil::RemoveFile(const char* pszFile)
{
    int iErrNo = 0;

    if(0 != unlink(pszFile))
    {
        iErrNo = errno;
        if(ENOENT != iErrNo)
        {
            return iErrNo;
        }
    }

    return 0;
}

/****************************************************************
* Function Name : GetDirFiles();
* Description   : 获取指定目录下的文件;
* Calls         : NA;
* Called By     : NA;
* Input         : pszDir: 指定目录;
* Output        : vecFiles: 输出文件列表;
* Return        : 0: 成功; Other: 失败;
* Table Accessed: NA;
* Table Updated : NA;
* Other         : NA;
****************************************************************/
int CUtil::GetDirFiles(const char* pszDir, vector<string>& vecFiles)
{
    assert(pszDir);

    DIR *pDir = NULL;
    struct dirent *pDirent = NULL;

    vecFiles.clear();

    if(NULL == (pDir = opendir(pszDir)))
    {
        return errno;
    }

    string strFile;
    while(NULL != (pDirent = readdir(pDir)))
    {
        strFile = pDirent->d_name;
        if("." == strFile || ".." == strFile)
        {
            continue;
        }

        if(0 == IsFileExist(strFile.c_str()))
        {
            vecFiles.push_back(strFile);
        }
    }

    closedir(pDir);
    pDir = NULL;
    delete pDirent;
    pDirent = NULL;

    return 0;
}

/****************************************************************
* Function Name : Trim();
* Description   : 删除输入字符串中的空格和Tab键;
* Calls         : NA;
* Called By     : NA;
* Input         : pszSrc: 输入串;
* Output        : pszSrc: 输出串;
* Return        : 结果串;
* Table Accessed: NA;
* Table Updated : NA;
* Other         : NA;
****************************************************************/
char* CUtil::Trim(char* pszSrc)
{
    assert(pszSrc);

    string strTmp = "";

    char* pszStart = pszSrc;
    char* pszEnd   = pszSrc + strlen(pszSrc) - 1;

    //去掉前面的空格和Tab键
    while(pszStart <= pszEnd)
    {
        if(' ' != *pszStart && '\t' != *pszStart)
        {
            break;
        }
        else
        {
            ++pszStart;
        }
    }

    //去掉后面的空格和Tab
    while(pszEnd > pszStart)
    {
        if(' ' != *pszEnd && '\t' != *pszEnd)
        {
            break;
        }
        else
        {
            --pszEnd;
        }
    }

    *(++pszEnd) = '\0';
    strTmp = pszStart;
    sprintf(pszSrc, "%s", strTmp.c_str());

    return pszSrc;
}

/****************************************************************
* Function Name : TouchFile();
* Description   : 创建一个文件, 如果文件存在, 则将文件内容清空;
* Calls         : NA;
* Called By     : NA;
* Input         : pszFile: 指定文件名;
* Output        : NA;
* Return        : 0: 成功; Other: 失败;
* Table Accessed: NA;
* Table Updated : NA;
* Other         : NA;
****************************************************************/
int CUtil::TouchFile(const char* pszFile)
{
    assert(pszFile);

    FILE *fp = NULL;
    fp = fopen(pszFile, "w");
    if(NULL == fp)
    {
        return errno;
    }
    fclose(fp);

    return 0;
}


/****************************************************************
* Function Name : MoveFile();
* Description   : 将文件改名;
* Calls         : NA;
* Called By     : NA;
* Input         : pszSrc: 源文件名; pszDest: 目的文件名;
* Output        : NA;
* Return        : 0: 成功; Other: 失败;
* Table Accessed: NA;
* Table Updated : NA;
* Other         : NA;
****************************************************************/
int CUtil::MoveFile(const char* pszSrc, const char* pszDest)
{
    assert(pszSrc);
    assert(pszDest);

    int iErrNo = 0;

    errno = 0;
    if(0 != rename(pszSrc, pszDest))
    {
        iErrNo = errno;
    }

    return iErrNo;
}

#if 0
int main(int argc, char* argv[])
{
    char szHost[64];
    CUtil::GetHostName(szHost);
    cout << szHost << endl;
    CUtil::GetLoginName(szHost);
    cout <<szHost<< endl;
    sprintf(szHost, "777");
    cout<<szHost<< endl;
    cout << CUtil::IsNumber(szHost, 1) << endl;

    sprintf(szHost, "1.txt");
    cout << CUtil::RemoveFile(szHost) << endl;

    //CUtil::GetHostIP(szHost);
    //cout << szHost << endl;

    //sprintf(szHost, "");
    //cout <<"Before:"<<szHost << "---"<<endl;
    //CUtil::Trim(szHost);
    //cout <<"After :"<<szHost<<"---"<<endl;

    //CUtil::GetCurDate(szHost);
    //cout << "CurDate: " << szHost << endl;

    //CUtil::GetCurTime(szHost);
    //cout << "CurTime: " << szHost << endl;

    //CUtil::GetCurDateTime(szHost);
    //cout << "CurDateTime: " << szHost << endl;

    //cout << "FileSize:" << CUtil::GetFileSize("3.txt") << endl;

    //cout <<CUtil::IsInt(argv[1]) << endl;
    //cout <<CUtil::IsFloat(argv[1]) << endl;

    //cout << CUtil::MakeDir("mydir") << endl;

    //cout <<CUtil::IsFileExist("mydir") << endl;
    //cout << CUtil::IsDate(argv[1]) << endl;
    //cout << CUtil::TouchFile("myfile.txt") << endl;

    vector<string> vec;
    cout << CUtil::GetDirFiles(".", vec) <<endl;
    for(int i=0; i<vec.size(); ++i)
    {
        cout <<vec[i] << " ";
    }
    cout <<endl;
}
#endif
/********************************** END *******************************************/

