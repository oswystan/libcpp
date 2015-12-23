/*
************************************************************************************
* CopyRight (C) WangYu
*
* FileName     : serviceframe.cpp
* Author       :
* Description  :
* Function List:
* History      :
************************************************************************************
*/

#include "lib_serviceframe.h"
#include <dlfcn.h>

//鏈嶅姟绠＄悊鍣ㄧ殑鎴愬憳鍙橀噺
CServiceManager*  CServiceManager::m_instance = NULL;
vector<CService*> CServiceManager::m_vecServices;


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
CDll::CDll()
{
    m_dllHandle  = NULL;
    m_strDllFile = "";
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
int CDll::Load(const string &strFile)
{
    if("" == strFile)
    {
        return 1;
    }

    if(m_dllHandle)
    {
        dlclose(m_dllHandle);
        m_strDllFile = "";
        m_dllHandle  = NULL;
    }

    m_dllHandle = dlopen(strFile.c_str(), RTLD_LAZY);
    if(NULL == m_dllHandle)
    {
        PrintErr("Unable to load the dll: " << strFile);
        return 1;
    }

    m_strDllFile = strFile;
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
int CDll::Load()
{
    if("" == m_strDllFile)
    {
        return 1;
    }

    string strTmp = m_strDllFile;
    return Load(strTmp);
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
int CDll::UnLoad()
{
    if(m_dllHandle)
    {
        dlclose(m_dllHandle);
    }

    m_dllHandle = NULL;
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
int CDll::StartService(const string &serName)
{
    if(!m_dllHandle)
    {
        return 1;
    }

    FN_StartService fnStart;

    fnStart = (FN_StartService)dlsym(m_dllHandle, fnStartService);
    if(!fnStart)
    {
        PrintErr("Fail to find the function: " << fnStartService);
        return 1;
    }

    return (*fnStart)(serName);
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
int CDll::StopService (const string &serName)
{
    if("" == serName || !m_dllHandle)
    {
        return 0;
    }

    FN_StartService fnStop;
    fnStop = (FN_StartService)dlsym(m_dllHandle, fnStopService);

    if(NULL == fnStop)
    {
        PrintErr("Fail to find the function: " << fnStopService);
        return 1;
    }

    return (*fnStop)(serName);
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
CService::CService(const string &serviceName, const string &dllFile)
{
    m_strServiceName      = serviceName;
    m_dllObj.m_strDllFile = dllFile;
    m_enStatus            = enStop;
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
int CService::Start()
{
    if(enStart == m_enStatus)
    {
        PrintInfo("service: " << m_strServiceName << " is already started");
        return 0;
    }

    m_enStatus = enStop;

    if(0 != m_dllObj.Load())
    {
        return 1;
    }

    if(0 != m_dllObj.StartService(m_strServiceName))
    {
        PrintErr("Fail to start service: " << m_strServiceName);
        return 1;
    }

    m_enStatus = enStart;

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
int CService::Stop ()
{
    if(enStop == m_enStatus)
    {
        PrintInfo("service: " << m_strServiceName << " is already stopped");
        return 0;
    }

    return m_dllObj.StopService(m_strServiceName);
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
bool CService::IsActive()
{
    return m_enStatus == enStart;
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
CServiceManager* CServiceManager::Instance()
{
    if(!m_instance)
    {
        NEW(m_instance, CServiceManager);
        m_vecServices.clear();
    }

    return m_instance;
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
int CServiceManager::AddService(const string &serName, const string &dllFile)
{
    if("" == serName || "" == dllFile)
    {
        return 1;
    }

    int iIdx = GetServiceIdx(serName);
    if(-1 != iIdx)
    {
        PrintInfo("The service: " << serName << " has been added")
        return 0;
    }

    CService *serTmp = NULL;
    NEW(serTmp, CService(serName, dllFile));
    if(!serTmp)
    {
        PrintInfo("Fail to allocate memory for CService");
        return 1;
    }

    m_vecServices.push_back(serTmp);

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
int CServiceManager::DelService(const string &serName)
{
    int iIdx = GetServiceIdx(serName);
    if(-1 == iIdx)
    {
        PrintInfo("The service: " << serName << " is not existed");
        return 0;
    }

    m_vecServices[iIdx]->Stop();
    DELETE(m_vecServices[iIdx]);

    m_vecServices.erase(m_vecServices.begin() + iIdx);
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
int CServiceManager::StartService(const string &serName)
{
    int iIdx = GetServiceIdx(serName);
    if(-1 == iIdx)
    {
        PrintInfo("The service: " << serName << " is not in the service manager");
        return 1;
    }

    return m_vecServices[iIdx]->Start();
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
int CServiceManager::StopService (const string &serName)
{
    int iIdx = GetServiceIdx(serName);
    if(-1 == iIdx)
    {
        PrintInfo("The service: " << serName << " is not in the service manager");
        return 1;
    }

    return m_vecServices[iIdx]->Stop();
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
void CServiceManager::PrintAllService()
{
    PrintInfo("============================= service status =============================");
    for(int i=0; i<m_vecServices.size(); ++i)
    {
        PrintInfo("service: " << m_vecServices[i]->m_strServiceName << " is "
                  << Status2String(m_vecServices[i]->m_enStatus));
    }
    PrintInfo("================================= END ====================================");
}


///////////////////////// test code ////////////////////////
#if 0
int main()
{
    CServiceManager* pIns = CServiceManager::Instance();
    pIns->AddService(string("time"), string("./time.so"));
    pIns->StartService(string("time"));
    pIns->StartService(string("time"));
    pIns->AddService(string("show"), string("./show.so"));
    pIns->StartService(string("show"));
    pIns->StartService(string("show1"));
    pIns->PrintAllService();
    while(1)
    {
        sleep(1);
    }
    return 0;
}
#endif
/********************************** END *******************************************/

