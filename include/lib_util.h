/*
************************************************************************************
* CopyRight
*
* FileName     : util.h
* Author       :
* Description  :
* Function List:
* History      :
************************************************************************************
*/
#ifndef UTIL_H
#define UTIL_H

#include <string>
#include <vector>
using namespace std;

class CUtil
{
    enum
    {
        EN_MAX_HOSTNAME = 256
    };

private:
    CUtil();

public:
    //获取当前系统IP
    static int GetHostIP(char* pszIP);

    //获取当前主机名
    static int GetHostName(char* pszHostName);

    //获取当前登陆的用户名
    static int GetLoginName(char* pszUsr);

    //检查输入串是否为数字字符串
    static int IsNumber(const char* pszSrc, int iIsNull = 0);

    //检查输入串是否是int类型
    static int IsInt(const char* pszSrc, int iIsNull = 0);

    //检查输入串是否是float类型
    static int IsFloat(const char* pszSrc, int iIsNull = 0);

    //检查输入串是否是一个合法日期, 格式YYYYMMDD
    static int IsDate(const char* pszSrc);

    //检查输入串是否是一个合法时间串, 格式HHMMSS
    static int IsTime(const char* pszSrc);

    //检查输入串是否是一个合法日期时间串, 格式YYYYMMDDHHMMSS
    static int IsDateTime(const char* pszSrc);

    //获取系统当前时间, 格式YYYYMMDD
    static int GetCurDate(char* pszOut);

    //获取当前系统时间, 格式HHMMSS
    static int GetCurTime(char* pszOut);

    //获取当前系统日期时间, 格式YYYYMMDDHHMMSS
    static int GetCurDateTime(char* pszOut);

    //获取指定文件大小
    static int GetFileSize(const char* pszFile);

    //检查指定的文件是否存在
    static int IsFileExist(const char* pszFile);

    //检查指定的目录是否存在
    static int IsDirExist(const char* pszDir);

    //创建指定的目录
    static int MakeDir(const char* pszDir);

    //删除指定文件
    static int RemoveFile(const char* pszFile);

    //获取指定目录下的所有文件名
    static int GetDirFiles(const char* pszDir, vector<string>& vecFiles);

    //删除指定字符串的空格和Tab键
    static char* Trim(char* pszSrc);

    //创建文件, 如果文件存在则将文件内容清空
    static int TouchFile(const char* pszFile);

    //移动文件
    static int MoveFile(const char* pszSrc, const char* pszDest);
};

#endif /*UTIL_H*/

/********************************** END *******************************************/

