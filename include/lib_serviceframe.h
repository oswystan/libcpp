/*
************************************************************************************
* CopyRight (C) WangYu
*
* FileName     : serviceframe.h
* Author       :
* Description  :
* Function List:
* History      :
************************************************************************************
*/
#ifndef SERVICEFRAME_H
#define SERVICEFRAME_H

#include "lib_common.h"
#include "lib_wfc_test.h"

#include <vector>
#include <string>

using namespace std;

//动态库中的服务启动函数名
#define fnStartService   "startService"
#define fnStopService    "stopService"

typedef int (*FN_StartService)(const string& strSer);

//服务状态类型
enum EnStatus
{
    enStart = 0,
    enStop
};

//转换服务状态为字符串
const char* Status2String(EnStatus enstatus)
{
    switch(enstatus)
    {
    case enStart:
        return "Start";

    case enStop:
        return "Stop!!";

    default:
        return "Unknown";
    }
}

//动态库操作类
class CDll
{
public:
    CDll();

    int Load(const string &strFile);
    int Load();
    int UnLoad();

    int StartService(const string &serName);
    int StopService (const string &serName);

public:
    string m_strDllFile;
    void*  m_dllHandle;
};

//服务类, 实现业务的启动, 停止等操作
class CService
{
public:
    CService(const string &serviceName, const string &dllFile);

    int Start();
    int Stop ();

    bool IsActive();

private:
    CService() {}

public:
    string   m_strServiceName;
    EnStatus m_enStatus;

private:
    CDll     m_dllObj;
};

//服务管理器
class CServiceManager
{
public:
    //获取服务管理器指针
    static CServiceManager* Instance();

    //添加, 删除服务
    static int AddService(const string &serName, const string &dllFile);
    static int DelService(const string &serName);

    //启动, 停止指定服务
    static int StartService(const string &serName);
    static int StopService (const string &serName);

    //打印所有业务状态
    static void PrintAllService();

private:
    CServiceManager() {}

    //获取指定服务的下标
    static int GetServiceIdx(const string &serName)
    {
        for(int i=0; i<m_vecServices.size(); ++i)
        {
            if(m_vecServices[i]->m_strServiceName == serName)
            {
                return i;
            }
        }

        return -1;
    }

private:
    static CServiceManager*  m_instance;        //实例指针
    static vector<CService*> m_vecServices;     //服务列表
};


#endif /*SERVICEFRAME_H*/

/********************************** END *******************************************/

