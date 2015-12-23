/*
*****************************************************************************************
* CopyRight (C) WangYu studio
*
* FileName     :
* Author       :
* Description  :
* Function List:
* History      :
*****************************************************************************************
*/

#include <stdarg.h>
#include <string.h>
#include "lib_log.h"
#include "lib_common.h"

/*******************************************************************
* Function      :
* Description   :
* Calls         :
* Called By     :
* Input         :
* Output        :
* Return        :
* Table Accessed:
* Table Updated :
* Other         :
*******************************************************************/
CLog::CLog()
{
	m_pszBuffer = NULL;
    memset(m_szNow,  0x00, sizeof(m_szNow));
    memset(m_szFile, 0x00, sizeof(m_szFile));
}

/****************************************************************
* Function Name :
* Description   :
* Calls         :
* Called By     :
* Input         :
* Output        :
* Return        :
* Table Accessed:
* Table Updated :
* Other         :
****************************************************************/
CLog::~CLog()
{
	if(m_ofsLog.is_open())
	{
        m_ofsLog.close();
	}

    try
    {
        delete []m_pszBuffer;
        m_pszBuffer = NULL;
    }
    catch(...)
    {
    }
}

/****************************************************************
* Function Name :
* Description   :
* Calls         :
* Called By     :
* Input         :
* Output        :
* Return        :
* Table Accessed:
* Table Updated :
* Other         :
****************************************************************/
int CLog::Init(const char* pszFile)
{
    GetTimeStr();
    if(NULL == pszFile)
    {
        cout << "[" << m_szNow << "]Invalid parameter" << endl;
        return 1;
    }

    snprintf(m_szFile, sizeof(m_szFile), "%s", pszFile);
    m_ofsLog.open(m_szFile, ios::app);
    if(!m_ofsLog)
    {
        cout << "Fail to open the log file: " << m_szFile << endl;
        return 1;
    }

    try
    {
        m_pszBuffer = new char[EN_MAXBUFLEN];
    }
    catch(...)
    {
        m_pszBuffer = NULL;
    }

    if(NULL == m_pszBuffer)
    {
        cout << "Fail to allocate memory" << endl;
        m_ofsLog.close();
        return 1;
    }

    return 0;
}

/****************************************************************
* Function Name :
* Description   :
* Calls         :
* Called By     :
* Input         :
* Output        :
* Return        :
* Table Accessed:
* Table Updated :
* Other         :
****************************************************************/
void CLog::LogInfo(const char* pszSrc, int iLine, const char* pszInfo, ...)
{
    assert(pszSrc);
    assert(pszInfo);

    //组织日志信息
    va_list infoList;
    va_start(infoList, pszInfo);
    vsprintf(m_pszBuffer, pszInfo, infoList);
    va_end(infoList);

    //获取当前时间
    GetTimeStr();

    //在屏幕上显示并记录在文件里
    cout << "[" << m_szNow << "][" << pszSrc << "," << iLine << "] INFO: "
         << pszInfo << endl;

    m_ofsLog << "[" << m_szNow << "][" << pszSrc << "," << iLine << "] INFO: "
             << pszInfo << endl;
}

/****************************************************************
* Function Name :
* Description   :
* Calls         :
* Called By     :
* Input         :
* Output        :
* Return        :
* Table Accessed:
* Table Updated :
* Other         :
****************************************************************/
void CLog::LogWarn(const char* pszSrc, int iLine, const char* pszInfo, ...)
{
    assert(pszSrc);
    assert(pszInfo);

    //组织日志信息
    va_list infoList;
    va_start(infoList, pszInfo);
    vsprintf(m_pszBuffer, pszInfo, infoList);
    va_end(infoList);

    //获取当前时间
    GetTimeStr();

    //在屏幕上显示并记录在文件里
    cout << "[" << m_szNow << "][" << pszSrc << "," << iLine << "] WARN: "
         << pszInfo << endl;

    m_ofsLog << "[" << m_szNow << "][" << pszSrc << "," << iLine << "] WARN: "
             << pszInfo << endl;
}

/****************************************************************
* Function Name :
* Description   :
* Calls         :
* Called By     :
* Input         :
* Output        :
* Return        :
* Table Accessed:
* Table Updated :
* Other         :
****************************************************************/
void CLog::LogErr(const char* pszSrc, int iLine, const char* pszInfo, ...)
{
    assert(pszSrc);
    assert(pszInfo);

    //组织日志信息
    va_list infoList;
    va_start(infoList, pszInfo);
    vsprintf(m_pszBuffer, pszInfo, infoList);
    va_end(infoList);

    //获取当前时间
    GetTimeStr();

    //在屏幕上显示并记录在文件里
    cout << "[" << m_szNow << "][" << pszSrc << "," << iLine << "]ERROR: "
         << pszInfo << endl;

    m_ofsLog << "[" << m_szNow << "][" << pszSrc << "," << iLine << "]ERROR: "
             << pszInfo << endl;
}

/****************************************************************
* Function Name :
* Description   :
* Calls         :
* Called By     :
* Input         :
* Output        :
* Return        :
* Table Accessed:
* Table Updated :
* Other         :
****************************************************************/
void CLog::LogStart(const char* pszProgram, int argc, char* argv[])
{
    assert(pszProgram);
    assert(argv);

    GetTimeStr();
    int iPos = 0;

    cout << endl;
    cout << "[" << m_szNow << "]";
    cout << "********************* START ***********************" << endl;
    cout << "[" << m_szNow << "]";
    cout << "* log file    : " << m_szFile << endl;
    cout << "[" << m_szNow << "]";
    cout << "* program name: " << pszProgram << endl;
    cout << "[" << m_szNow << "]";
    cout << "* argurements : ";

    for(iPos = 1; iPos < argc; ++iPos)
    {
        cout << argv[iPos];
        if(iPos != argc - 1)
        {
            cout << " ";
        }
    }

    cout << endl;
    cout << "[" << m_szNow << "]";
    cout << "***************************************************" << endl;

    m_ofsLog << endl;
    m_ofsLog << "[" << m_szNow << "]";
    m_ofsLog << "********************* START ***********************" << endl;
    m_ofsLog << "[" << m_szNow << "]";
    m_ofsLog << "* log file    : " << m_szFile << endl;
    m_ofsLog << "[" << m_szNow << "]";
    m_ofsLog << "* program name: " << pszProgram << endl;
    m_ofsLog << "[" << m_szNow << "]";
    m_ofsLog << "* argurements : ";

    for(iPos = 1; iPos < argc; ++iPos)
    {
        m_ofsLog << argv[iPos];
        if(iPos != argc - 1)
        {
            m_ofsLog << " ";
        }
    }

    m_ofsLog << endl;
    m_ofsLog << "[" << m_szNow << "]";
    m_ofsLog << "***************************************************" << endl;
}

/****************************************************************
* Function Name :
* Description   :
* Calls         :
* Called By     :
* Input         :
* Output        :
* Return        :
* Table Accessed:
* Table Updated :
* Other         :
****************************************************************/
void CLog::LogEnd()
{
    GetTimeStr();
    cout << "[" << m_szNow << "]";
    cout << "***************************************************" << endl;
    cout << "[" << m_szNow << "]";
    cout << "* " << m_szFile << endl;
    cout << "[" << m_szNow << "]";
    cout << "*********************** END ***********************" << endl;
    cout << endl;

    m_ofsLog << "[" << m_szNow << "]";
    m_ofsLog << "***************************************************" << endl;
    m_ofsLog << "[" << m_szNow << "]";
    m_ofsLog << "* " << m_szFile << endl;
    m_ofsLog << "[" << m_szNow << "]";
    m_ofsLog << "*********************** END ***********************" << endl;
    m_ofsLog << endl;
}

/****************************************************************
* Function Name :
* Description   :
* Calls         :
* Called By     :
* Input         :
* Output        :
* Return        :
* Table Accessed:
* Table Updated :
* Other         :
****************************************************************/
void CLog::GetTimeStr()
{
    time_t stNow;
    tm     *pTm;

    stNow = time(NULL);
    pTm   = localtime(&stNow);
    snprintf(m_szNow, sizeof(m_szNow), "%d-%02d-%02d %02d:%02d:%02d",
                 pTm->tm_year + 1900, pTm->tm_mon + 1, pTm->tm_mday,
                 pTm->tm_hour, pTm->tm_min, pTm->tm_sec);
}

//=================================================================
#if 0
int main(int argc, char* argv[])
{
    CLog log;
    if( 0 != log.Init("1.log"))
    {
        cout << "Fail to init log" << endl;
        return 1;
    }

    log.LogStart(argv[0], argc, argv);
    log.LogInfo(_FL_, "CLog test");
    log.LogWarn(_FL_, "CLog test");
    log.LogErr (_FL_, "CLog test");
    log.LogEnd();

    return 0;
}
#endif
/********************************** END ************************************************/

