/*
************************************************************************************
* CopyRight (C) WangYu
*
* FileName     : monitor.h
* Author       : WangYu
* Description  : 监控程序框架
* Function List:
* History      :
************************************************************************************
*/
#ifndef MONITOR_H
#define MONITOR_H

#include "lib_wfc_test.h"

#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/statvfs.h>
#include <signal.h>

#include <iostream>
#include <fstream>
#include <list>
using namespace std;


void OnTerminate(int);

class CMonitorBase;

//监控程序控制类
class CMonitorCtrl
{
public:
    CMonitorCtrl(){}
    ~CMonitorCtrl(){}

    //注册需要执行的监控类
    int Register(CMonitorBase* pMonitor);

    //执行监控活动
    int Run();

private:
    list<CMonitorBase*> m_lstMonitorEntity;
};

//监控类的基类
class CMonitorBase
{
public:
    CMonitorBase(CMonitorCtrl& monitorCtrl);
    virtual ~CMonitorBase();

    //执行监控工作
    virtual int OnMonitor() = 0;

protected:
    CMonitorCtrl* m_mainCtrl;
};

//用于实现程序休眠的监控类
class CDelayMonitor : public CMonitorBase
{
    enum { EN_DefaultDelayTime = 1 };
public:
    CDelayMonitor(CMonitorCtrl &monitorCtrl, int iDelayTime = 1);
    ~CDelayMonitor() {}

    //监控函数, 用来使程序休眠
    int OnMonitor();

private:
    int m_iDelayTime;
};


//////////////////// Test Class ////////////////////

//监控文件修改类
class CFileMonitor : public CMonitorBase
{
public:
    CFileMonitor(CMonitorCtrl &monitorCtrl, const char* pszFile);
    ~CFileMonitor() {}

    //执行对指定文件的检测, 如果文件修改了, 则通知用户
    int OnMonitor();

private:
    string m_strFile;
    time_t m_tmLastModify;
};


//磁盘空间监测类
class CDiskMonitor : public CMonitorBase
{
    enum { EN_DefaultPercent = 80 };
public:
    CDiskMonitor(CMonitorCtrl &monitorCtrl, const char* pszMntDir, int iPercent = EN_DefaultPercent);
    ~CDiskMonitor() {}

    //监测指定挂接目录的磁盘空间大小
    int OnMonitor();

private:
    string m_strMntDir;
    int    m_iPercent;
};

//内存使用率监测
class CMemMonitor : public CMonitorBase
{
    enum { EN_DefaultPercent = 80 };
public:
    CMemMonitor(CMonitorCtrl &monitorCtrl, int iPercent = EN_DefaultPercent);
    ~CMemMonitor() {}

    //监测内存使用情况
    int OnMonitor();

private:
    int m_iPercent;
};

//CPU使用率监测
class CCPUMonitor : public CMonitorBase
{
    enum { EN_DefaultPercent = 80 };
public:
    CCPUMonitor(CMonitorCtrl &monitorCtrl, int iPercent = EN_DefaultPercent);
    ~CCPUMonitor() {}

    //监测内存使用情况
    int OnMonitor();

private:
    int m_iPercent;

    unsigned long m_ulUser;
    unsigned long m_ulNice;
    unsigned long m_ulSys;
    unsigned long m_ulIdle;
};

#endif /*MONITOR_H*/

/********************************** END *******************************************/

