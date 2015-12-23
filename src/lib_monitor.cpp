/*
************************************************************************************
* CopyRight (C) WangYu
*
* FileName     : monitor.cpp
* Author       : WangYu
* Description  : 监控程序框架, 需要后台运行
* Function List:
* History      :
************************************************************************************
*/

#include <stdlib.h>
#include <string.h>
#include "lib_monitor.h"

/****************************************************************
* Function Name : ;
* Description   : ;
* Calls         : NA;
* Called By     : NA;
* Input         : NA;
* Output        : NA;
* Return        : NA;
* Table Accessed: NA;
* Table Updated : NA;
* Other         : NA;
****************************************************************/
void OnTerminate(int)
{
    PrintInfo("Monitor terminated");
    exit(0);
}

/****************************************************************
* Function Name : ;
* Description   : ;
* Calls         : NA;
* Called By     : NA;
* Input         : NA;
* Output        : NA;
* Return        : NA;
* Table Accessed: NA;
* Table Updated : NA;
* Other         : NA;
****************************************************************/
int CMonitorCtrl::Register(CMonitorBase* pMonitor)
{
    list<CMonitorBase*>::iterator itMon;

    for(itMon = m_lstMonitorEntity.begin(); itMon != m_lstMonitorEntity.end(); ++itMon)
    {
        if(pMonitor == *itMon)
        {
            PrintDebug("The monitor has been registerred");
            return 1;
        }
    }

    m_lstMonitorEntity.push_back(pMonitor);

    return 0;
}

/****************************************************************
* Function Name : ;
* Description   : ;
* Calls         : NA;
* Called By     : NA;
* Input         : NA;
* Output        : NA;
* Return        : NA;
* Table Accessed: NA;
* Table Updated : NA;
* Other         : NA;
****************************************************************/
int CMonitorCtrl::Run()
{
    if(0 == m_lstMonitorEntity.size())
    {
        PrintInfo("No task need to be run, exit");
        return 0;
    }

    //设置中断消息处理函数
    signal(SIGINT, OnTerminate);

    list<CMonitorBase*>::iterator itMon;
    itMon = m_lstMonitorEntity.begin();

    //循环调用监控函数
    while(m_lstMonitorEntity.end() != (itMon = m_lstMonitorEntity.begin()))
    {
        m_lstMonitorEntity.pop_front();
        (*itMon)->OnMonitor();
    }
}

/****************************************************************
* Function Name : ;
* Description   : ;
* Calls         : NA;
* Called By     : NA;
* Input         : NA;
* Output        : NA;
* Return        : NA;
* Table Accessed: NA;
* Table Updated : NA;
* Other         : NA;
****************************************************************/
CMonitorBase::CMonitorBase(CMonitorCtrl& monitorCtrl)
{
    monitorCtrl.Register(this);
    m_mainCtrl = &monitorCtrl;
}

/****************************************************************
* Function Name : ;
* Description   : ;
* Calls         : NA;
* Called By     : NA;
* Input         : NA;
* Output        : NA;
* Return        : NA;
* Table Accessed: NA;
* Table Updated : NA;
* Other         : NA;
****************************************************************/
CMonitorBase::~CMonitorBase()
{
}

/****************************************************************
* Function Name : ;
* Description   : ;
* Calls         : NA;
* Called By     : NA;
* Input         : NA;
* Output        : NA;
* Return        : NA;
* Table Accessed: NA;
* Table Updated : NA;
* Other         : NA;
****************************************************************/
CDelayMonitor::CDelayMonitor(CMonitorCtrl &monitorCtrl, int iDelayTime):CMonitorBase(monitorCtrl)
{
    if(iDelayTime <= 0)
    {
        m_iDelayTime = EN_DefaultDelayTime;
    }
    else
    {
        m_iDelayTime = iDelayTime;
    }
}

/****************************************************************
* Function Name : ;
* Description   : ;
* Calls         : NA;
* Called By     : NA;
* Input         : NA;
* Output        : NA;
* Return        : NA;
* Table Accessed: NA;
* Table Updated : NA;
* Other         : NA;
****************************************************************/
int CDelayMonitor::OnMonitor()
{
    sleep(m_iDelayTime);
    m_mainCtrl->Register(this);

    return 0;
}

/****************************************************************
* Function Name : ;
* Description   : ;
* Calls         : NA;
* Called By     : NA;
* Input         : NA;
* Output        : NA;
* Return        : NA;
* Table Accessed: NA;
* Table Updated : NA;
* Other         : NA;
****************************************************************/
CFileMonitor::CFileMonitor(CMonitorCtrl &monitorCtrl, const char* pszFile):CMonitorBase(monitorCtrl)
{
    assert(pszFile);

    m_strFile      = pszFile;
    m_tmLastModify = 0;
}

/****************************************************************
* Function Name : ;
* Description   : ;
* Calls         : NA;
* Called By     : NA;
* Input         : NA;
* Output        : NA;
* Return        : NA;
* Table Accessed: NA;
* Table Updated : NA;
* Other         : NA;
****************************************************************/
int CFileMonitor::OnMonitor()
{
    struct stat stFile;
    if(0 != stat(m_strFile.c_str(), &stFile))
    {
        PrintErr("Fail to get last modify time of file: " << m_strFile)
        return 1;
    }

    if(m_tmLastModify != 0 && m_tmLastModify != stFile.st_mtime)
    {
        PrintInfo("File [" << m_strFile << "] has been modified");
    }

    m_tmLastModify = stFile.st_mtime;

    m_mainCtrl->Register(this);

    return 0;
}

/****************************************************************
* Function Name : ;
* Description   : ;
* Calls         : NA;
* Called By     : NA;
* Input         : NA;
* Output        : NA;
* Return        : NA;
* Table Accessed: NA;
* Table Updated : NA;
* Other         : NA;
****************************************************************/
CDiskMonitor::CDiskMonitor(CMonitorCtrl &monitorCtrl, const char* pszMntDir, int iPercent):CMonitorBase(monitorCtrl)
{
    assert(pszMntDir);
    m_strMntDir = pszMntDir;

    if(iPercent <= 0 || iPercent > 100)
    {
        m_iPercent = EN_DefaultPercent;
    }
    else
    {
        m_iPercent = iPercent;
    }
}

/****************************************************************
* Function Name : ;
* Description   : ;
* Calls         : NA;
* Called By     : NA;
* Input         : NA;
* Output        : NA;
* Return        : NA;
* Table Accessed: NA;
* Table Updated : NA;
* Other         : NA;
****************************************************************/
int CDiskMonitor::OnMonitor()
{
    struct statvfs vfs;
    if(m_strMntDir.empty() || '/' != m_strMntDir[0])
    {
        PrintErr("Invalid mount directory [" << m_strMntDir << "]");
        return 1;
    }

    if(0 != statvfs(m_strMntDir.c_str(), &vfs))
    {
        PrintErr("Fail to get disk information of directory [" << m_strMntDir
                 << "[errno=" << errno << "]");
        return 1;
    }

    double dPer = double(vfs.f_blocks - vfs.f_bavail) / vfs.f_blocks;
    int iTmp =  int(dPer * 100);

    if(iTmp > m_iPercent)
    {
        PrintInfo("Total usage of directory [" << m_strMntDir << "] is " << iTmp
                  << "%[>" << m_iPercent << "%]");
    }

    m_mainCtrl->Register(this);

    return 0;
}

/****************************************************************
* Function Name : ;
* Description   : ;
* Calls         : NA;
* Called By     : NA;
* Input         : NA;
* Output        : NA;
* Return        : NA;
* Table Accessed: NA;
* Table Updated : NA;
* Other         : NA;
****************************************************************/
CMemMonitor::CMemMonitor(CMonitorCtrl &monitorCtrl, int iPercent):CMonitorBase(monitorCtrl)
{
    if(iPercent <= 0 || iPercent > 100)
    {
        m_iPercent = EN_DefaultPercent;
    }
    else
    {
        m_iPercent = iPercent;
    }
}

/****************************************************************
* Function Name : ;
* Description   : ;
* Calls         : NA;
* Called By     : NA;
* Input         : NA;
* Output        : NA;
* Return        : NA;
* Table Accessed: NA;
* Table Updated : NA;
* Other         : NA;
****************************************************************/
int CMemMonitor::OnMonitor()
{
    ifstream ifs;
    ifs.open("/proc/meminfo");
    if(!ifs)
    {
        ifs.close();
        PrintErr("Fail to open the file: meminfo");
        return 1;
    }

    char szLine[1024];
    memset(szLine, 0x00, sizeof(szLine));

    //过滤掉第一行
    ifs.getline(szLine, sizeof(szLine));
    ifs.getline(szLine, sizeof(szLine));
    ifs.close();

    char* pos1 = NULL;
    char* pos2 = NULL;

    unsigned long ulTotal;
    unsigned long ulUsed;

    //过滤掉'Mem:'
    pos1 = strchr(szLine, ' ');
    if(!pos1)
    {
        PrintErr("Invalid memeroy infomation");
        return 1;
    }
    while(isspace(*pos1)) ++pos1;

    pos2 = strchr(pos1, ' ');
    if(!pos2)
    {
        PrintErr("Invalid memeroy infomation");
        return 1;
    }

    *pos2 = '\0';
    ulTotal = atol(pos1);

    pos1 = pos2 + 1;
    while(isspace(*pos1)) ++pos1;

    pos2 = strchr(pos1, ' ');
    if(!pos2)
    {
        PrintErr("Invalid memeroy infomation");
        return 1;
    }

    *pos2 = '\0';
    ulUsed = atol(pos1);

    int iPer = 0;
    iPer = (int)((double)ulUsed / ulTotal * 100);

    if(iPer >= m_iPercent)
    {
        PrintInfo("Memory usage: " << iPer << "%[>=" << m_iPercent << "%]");
    }

    m_mainCtrl->Register(this);

    return 0;
}

/****************************************************************
* Function Name : ;
* Description   : ;
* Calls         : NA;
* Called By     : NA;
* Input         : NA;
* Output        : NA;
* Return        : NA;
* Table Accessed: NA;
* Table Updated : NA;
* Other         : NA;
****************************************************************/
CCPUMonitor::CCPUMonitor(CMonitorCtrl &monitorCtrl, int iPercent):CMonitorBase(monitorCtrl)
{
    if(iPercent <= 0 || iPercent > 100)
    {
        m_iPercent = EN_DefaultPercent;
    }
    else
    {
        m_iPercent = iPercent;
    }

    m_ulUser = 0;
    m_ulNice = 0;
    m_ulSys  = 0;
    m_ulIdle = 0;
}

/****************************************************************
* Function Name : ;
* Description   : ;
* Calls         : NA;
* Called By     : NA;
* Input         : NA;
* Output        : NA;
* Return        : NA;
* Table Accessed: NA;
* Table Updated : NA;
* Other         : NA;
****************************************************************/
int CCPUMonitor::OnMonitor()
{
    ifstream ifs;
    ifs.open("/proc/stat");
    if(!ifs)
    {
        ifs.close();
        PrintErr("Fail to open the file: stat");
        return 1;
    }

    char szLine[1024];
    memset(szLine, 0x00, sizeof(szLine));

    ifs.getline(szLine, sizeof(szLine));
    ifs.close();

    char* pos1 = NULL;
    char* pos2 = NULL;

    unsigned long ulTotal;
    unsigned long ulIdle;
    unsigned long ulUser;
    unsigned long ulNice;
    unsigned long ulSys;

    sscanf(szLine, "cpu %lu %lu %lu %lu", &ulUser, &ulNice, &ulSys, &ulIdle);
    //PrintDebug("ulUser:" << ulUser << " ulNice:" << ulNice << "  ulSys:"<< ulSys << " ulIdle:" << ulIdle);
    ulTotal = (ulUser - m_ulUser) + (ulNice - m_ulNice) + (ulSys - m_ulSys) + (ulIdle - m_ulIdle);
    if(0 == ulTotal)
    {
        ulTotal = 1;
    }

    int iPer = 0;
    iPer = (int)((double)(ulTotal - (ulIdle - m_ulIdle))/ ulTotal * 100);
    if(0 == m_ulIdle)
    {
        iPer = 0;
    }

    m_ulUser = ulUser;
    m_ulNice = ulNice;
    m_ulSys  = ulSys;
    m_ulIdle = ulIdle;


    if(iPer >= m_iPercent)
    {
        PrintInfo("CPU usage: " << iPer << "%[>=" << m_iPercent << "%]");
    }

    m_mainCtrl->Register(this);

    return 0;
}


#if 0
int main()
{
    CMonitorCtrl mainCtrl;
    CDelayMonitor dm(mainCtrl);
    //CFileMonitor fm(mainCtrl, "1.txt");

    PrintInfo("A CPU monitor started");
    //CDiskMonitor diskMon(mainCtrl, "/", 60);

    //CMemMonitor cm(mainCtrl);

    CCPUMonitor cm(mainCtrl);

    mainCtrl.Run();
}
#endif

/********************************** END *******************************************/

