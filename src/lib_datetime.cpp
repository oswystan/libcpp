/*
************************************************************************************
* CopyRight (C) WangYu
*
* FileName     : datetime.cpp
* Author       :
* Description  :
* Function List:
* History      :
************************************************************************************
*/

#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include "lib_datetime.h"

//for test
#include <iostream>
using namespace std;

/****************************************************************
* Function Name : CDateTime();
* Description   : 根据输入参数构造时间对象;
* Calls         : NA;
* Called By     : NA;
* Input         : tmSrc: 时间参数;
* Output        : NA;
* Return        : NA;
* Table Accessed: NA;
* Table Updated : NA;
* Other         : NA;
****************************************************************/
CDateTime::CDateTime(const time_t tmSrc)
{
    struct tm stTm;

    Reset();

    if(NULL == localtime_r(&tmSrc, &stTm))
    {
        m_eStatus = EN_ERROR;
        return;
    }

    m_iYear  = stTm.tm_year + 1900;
    m_iMonth = stTm.tm_mon + 1;
    m_iDay   = stTm.tm_mday;
    m_iHour  = stTm.tm_hour;
    m_iMin   = stTm.tm_min;
    m_iSec   = stTm.tm_sec;

    m_eDateType = EN_DATETIME;
}

/****************************************************************
* Function Name : CDateTime();
* Description   : 根据输入参数构造时间对象;
* Calls         : NA;
* Called By     : NA;
* Input         : tmTime: 时间参数;
* Output        : NA;
* Return        : NA;
* Table Accessed: NA;
* Table Updated : NA;
* Other         : NA;
****************************************************************/
CDateTime::CDateTime(const struct tm &tmTime)
{
    Reset();
    m_iYear  = tmTime.tm_year + 1900;
    m_iMonth = tmTime.tm_mon + 1;
    m_iDay   = tmTime.tm_mday;
    m_iHour  = tmTime.tm_hour;
    m_iMin   = tmTime.tm_min;
    m_iSec   = tmTime.tm_sec;

    m_eDateType = EN_DATETIME;
}

/****************************************************************
* Function Name : CDateTime();
* Description   : 根据输入参数构造时间对象;
* Calls         : NA;
* Called By     : NA;
* Input         : pszTime: 时间参数;
* Output        : NA;
* Return        : NA;
* Table Accessed: NA;
* Table Updated : NA;
* Other         : NA;
****************************************************************/
CDateTime::CDateTime(const char* pszTime, TDateType eType)
{
    assert(pszTime);

    Reset();

    //判断eType是否合法
    if(EN_DATE != eType && EN_DATETIME != eType)
    {
        m_eStatus = EN_ERROR;
        return;
    }

    //检查日期是否合法
    if(0 != CheckValid(pszTime, eType))
    {
        m_eStatus = EN_ERROR;
        return;
    }

    char szTmp[8] = {0};

    strncpy(szTmp, pszTime, 4);
    m_iYear = atoi(szTmp);
    memset(szTmp, 0x00, sizeof(szTmp));
    strncpy(szTmp, pszTime + 4, 2);
    m_iMonth = atoi(szTmp);
    strncpy(szTmp, pszTime + 6, 2);
    m_iDay = atoi(szTmp);

    if(EN_DATETIME == eType)
    {
        strncpy(szTmp, pszTime + 8, 2);
        m_iHour = atoi(szTmp);
        strncpy(szTmp, pszTime + 10, 2);
        m_iMin = atoi(szTmp);
        strncpy(szTmp, pszTime + 12, 2);
        m_iSec = atoi(szTmp);
    }
    else
    {
        m_iHour = 0;
        m_iMin  = 0;
        m_iSec  = 0;
    }

    m_eDateType = eType;
}

/****************************************************************
* Function Name : CDateTime();
* Description   : 根据输入参数构造时间对象;
* Calls         : NA;
* Called By     : NA;
* Input         : iYear,iMon,iDay,iHour,iMin,iSec: 时间参数;
* Output        : NA;
* Return        : NA;
* Table Accessed: NA;
* Table Updated : NA;
* Other         : NA;
****************************************************************/
CDateTime::CDateTime(int iYear, int iMon, int iDay, int iHour, int iMin, int iSec)
{
    Reset();

    if(iYear < 0 || iYear > 9999 || iMon < 1 || iMon > 12 || iHour < 0 ||
        iHour > 23 || iMin < 0 || iMin > 59 || iSec < 0 || iSec > 60)
    {
        m_eStatus = EN_ERROR;
        return;
    }

    int  iMaxDay[]={31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if((0 == iYear % 4 && 0 != iYear % 100) || 0 == iYear % 400)
    {
        iMaxDay[1] = 29;
    }

    if(iDay > iMaxDay[iMon - 1])
    {
        m_eStatus = EN_ERROR;
        return;
    }

    m_iYear  = iYear;
    m_iMonth = iMon;
    m_iDay   = iDay;
    m_iHour  = iHour;
    m_iMin   = iMin;
    m_iSec   = iSec;

    m_eDateType = EN_DATE;
}

/****************************************************************
* Function Name : CDateTime();
* Description   : 构造函数, 使用当前时间构造一个日期对象;
* Calls         : NA;
* Called By     : NA;
* Input         : NA;
* Output        : NA;
* Return        : NA;
* Table Accessed: NA;
* Table Updated : NA;
* Other         : NA;
****************************************************************/
CDateTime::CDateTime()
{
    Reset();

    time_t stNow;
    tm     stTm;

    stNow = time(NULL);
    if(NULL == localtime_r(&stNow, &stTm))
    {
        m_eStatus = EN_ERROR;
        return;
    }

    m_iYear  = stTm.tm_year + 1900;
    m_iMonth = stTm.tm_mon + 1;
    m_iDay   = stTm.tm_mday;
    m_iHour  = stTm.tm_hour;
    m_iMin   = stTm.tm_min;
    m_iSec   = stTm.tm_sec;

    m_eDateType = EN_DATE;
}

/****************************************************************
* Function Name : CDateTime();
* Description   : 析构函数;
* Calls         : NA;
* Called By     : NA;
* Input         : NA;
* Output        : NA;
* Return        : NA;
* Table Accessed: NA;
* Table Updated : NA;
* Other         : NA;
****************************************************************/
CDateTime::~CDateTime()
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
int CDateTime::GetCurDateTime(CDateTime& outDate)
{
    time_t stNow;
    tm     stTm;

    stNow = time(NULL);
    if(NULL == localtime_r(&stNow, &stTm))
    {
        return 1;
    }

    outDate.m_iYear  = stTm.tm_year + 1900;
    outDate.m_iMonth = stTm.tm_mon + 1;
    outDate.m_iDay   = stTm.tm_mday;
    outDate.m_iHour  = stTm.tm_hour;
    outDate.m_iMin   = stTm.tm_min;
    outDate.m_iSec   = stTm.tm_sec;

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
int CDateTime::GetCurDateTime(char* outDate, int iSize)
{
    assert(outDate && iSize >= 0);

    time_t stNow;
    tm     stTm;

    stNow = time(NULL);
    if(NULL == localtime_r(&stNow, &stTm))
    {
        return 1;
    }

    snprintf(outDate, iSize, "%d%02d%2d%02d%02d%02d", stTm.tm_year + 1900,
             stTm.tm_mon + 1, stTm.tm_mday, stTm.tm_hour, stTm.tm_min, stTm.tm_sec);

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
int CDateTime::IsLeapYear(int iYear)
{
    if(iYear < 0)
    {
        return 1;
    }

    if((0 == iYear % 4 && 0 != iYear % 100) || 0 == iYear % 400)
    {
        return 0;
    }
    else
    {
        return 1;
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
int CDateTime::CheckValid(const char* pszTime, TDateType eType)
{
    assert(pszTime);

    if(eType != strlen(pszTime))
    {
        return 1;
    }

    const char* pszTmp = pszTime;

    while('\0' != *pszTmp)
    {
        if(*pszTmp < '0' || *pszTmp > '9')
        {
            return 1;
        }
        ++pszTmp;
    }

    int iYear  = 0;
    int iMonth = 0;
    int iDay   = 0;
    int iHour  = 0;
    int iMin   = 0;
    int iSec   = 0;

    char szTmp[8] = {0};

    int  iMaxDay[]={31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    if(EN_DATE == eType || EN_DATETIME == eType)
    {
        strncpy(szTmp, pszTime, 4);
        iYear = atoi(szTmp);
        memset(szTmp, 0x00, sizeof(szTmp));
        strncpy(szTmp, pszTime + 4, 2);
        iMonth = atoi(szTmp);
        strncpy(szTmp, pszTime + 6, 2);
        iDay = atoi(szTmp);

        //闰年判断
        if((0 == iYear % 4 && 0 != iYear % 100) || 0 == iYear % 400)
        {
            iMaxDay[1] = 29;
        }

        if(iMonth < 1 || iMonth > 12 || iDay > iMaxDay[iMonth - 1])
        {
            return 1;
        }

        if(EN_DATETIME == eType)
        {
            strncpy(szTmp, pszTime + 8, 2);
            iHour = atoi(szTmp);
            strncpy(szTmp, pszTime + 10, 2);
            iMin = atoi(szTmp);
            strncpy(szTmp, pszTime + 12, 2);
            iSec = atoi(szTmp);

            if(iHour > 23 || iMin > 59 || iSec > 59)
            {
                return 1;
            }
        }
    }
    else
    {
        strncpy(szTmp, pszTime + 0, 2);
        iHour = atoi(szTmp);
        strncpy(szTmp, pszTime + 2, 2);
        iMin = atoi(szTmp);
        strncpy(szTmp, pszTime + 4, 2);
        iSec = atoi(szTmp);
        if(iHour > 23 || iMin > 59 || iSec > 59)
        {
            return 1;
        }
    }

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
int CDateTime::Add(int iYear, int iMon, int iDay, int iHour, int iMin, int iSec)
{
    if(EN_INITOK != m_eStatus)
    {
        return 1;
    }

    //检查参数合法性
    if(iYear <= -9999 || iYear >= 9999)
    {
        return 1;
    }

    int iTmpYear = 0;
    int iTmpMon  = 0;
    int iTmpDay  = 0;
    int iTmpHour = 0;
    int iTmpMin  = 0;
    int iTmpSec  = 0;

    iTmpSec = m_iSec + iSec;
    iTmpMin = m_iMin + iMin;
    iTmpMin = iTmpMin + (int)(iTmpSec / 60);
    iTmpSec = iTmpSec % 60;
    if(iTmpSec < 0)
    {
        iTmpSec = 60 + iTmpSec;
        --iTmpMin;
    }

    iTmpHour = m_iHour + iHour;
    iTmpHour = iTmpHour + (int)(iTmpMin / 60);
    iTmpMin  = iTmpMin % 60;
    if(iTmpMin < 0)
    {
        iTmpMin = 60 + iTmpMin;
        --iTmpHour;
    }

    iTmpDay = m_iDay + iDay;
    iTmpDay = iTmpDay + (int)(iTmpHour / 24);
    iTmpHour = iTmpHour % 24;
    if(iTmpHour < 0)
    {
        iTmpHour = 24 + iTmpHour;
        --iTmpDay;
    }

    int  iMaxDay[]={31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    //计算月份
    iTmpMon = m_iMonth + iMon;
    iTmpYear = m_iYear + iYear;
    iTmpYear = iTmpYear + int((iTmpMon - 1) / 12);
    iTmpMon = (iTmpMon - 1) % 12 + 1;
    if(iTmpMon < 0)
    {
        iTmpMon = 12 + iTmpMon;
        --iTmpYear;
    }

    if(iTmpYear < 0)
    {
        return 1;
    }

    if((0 == iTmpYear % 4 && 0 != iTmpYear % 100) || 0 == iTmpYear % 400)
    {
        iMaxDay[1] = 29;
    }

    int iPos = 0;
    if(iTmpDay <= 0)
    {
        iTmpMon = iTmpMon - 1;
        while(1)
        {
            for(iPos = iTmpMon - 1; iPos > 0; --iPos)
            {
                iTmpDay = iTmpDay + iMaxDay[iPos];
                if(iTmpDay > 0)
                {
                    m_iMonth = iPos + 1;
                    m_iDay   = iTmpDay;
                    m_iYear  = iTmpYear;
                    break;
                }
            }
            if(iTmpDay > 0)
            {
                break;
            }
            else
            {
                iTmpMon  = 12;
                iTmpYear = iTmpYear - 1;
                if(iTmpYear < 0)
                {
                    return 1;
                }
                if((0 == iTmpYear % 4 && 0 != iTmpYear % 100) || 0 == iTmpYear % 400)
                {
                    iMaxDay[1] = 29;
                }
                else
                {
                    iMaxDay[1] = 28;
                }
            }
        }
    }
    else
    {
        while(1)
        {
            for(iPos = iTmpMon - 1; iPos < 12; ++iPos)
            {
                iTmpDay = iTmpDay - iMaxDay[iPos];
                if(iTmpDay <= 0)
                {
                    m_iDay   = iTmpDay + iMaxDay[iPos];
                    m_iMonth = iPos + 1;
                    m_iYear  = iTmpYear;
                    break;
                }
            }
            if(iTmpDay <= 0)
            {
                break;
            }
            else
            {
                iTmpMon  = 1;
                iTmpYear = iTmpYear + 1;
                if(iTmpYear > 9999)
                {
                    return 1;
                }

                if((0 == iTmpYear % 4 && 0 != iTmpYear % 100) || 0 == iTmpYear % 400)
                {
                    iMaxDay[1] = 29;
                }
                else
                {
                    iMaxDay[1] = 28;
                }
            }
        }
    }

    //将时分秒进行赋值
    m_iHour = iTmpHour;
    m_iMin  = iTmpMin;
    m_iSec  = iTmpSec;

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
int CDateTime::GetSpanDays(const CDateTime& rValue, int &iDays) const
{
    if(EN_INITOK != m_eStatus || EN_INITOK != rValue.GetStatus())
    {
        return 1;
    }

    int  iMaxDay[]={31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if(0 == IsLeapYear(m_iYear))
    {
        iMaxDay[1] = 29;
    }

    int iPos = 0;

    //获取当前日期对象的总的天数
    int iOrgDays = m_iYear * 365 - (int)(m_iYear / 100) + (int)(m_iYear / 400) + 366;
    for(iPos = 0; iPos < m_iMonth - 1; ++iPos)
    {
        iOrgDays += iMaxDay[iPos];
    }
    iOrgDays += m_iDay;

    //获取输入参数的总的天数
    int iDestDays = rValue.GetYear() * 365 - (int)(rValue.GetYear() / 100) +
                    (int)(rValue.GetYear() / 400) + 366;

    if(0 == IsLeapYear(rValue.GetYear()))
    {
        iMaxDay[1] = 29;
    }
    else
    {
        iMaxDay[1] = 28;
    }

    for(iPos = 0; iPos < rValue.GetMonth() - 1; ++iPos)
    {
        iDestDays += iMaxDay[iPos];
    }
    iDestDays += rValue.GetDay();

    //计算2个日期值之间的天数:rValue - *this
    iDays = iDestDays - iOrgDays;

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
int CDateTime::GetSpanSecs(const CDateTime& rValue, int &iSecs) const
{
    if(EN_INITOK != m_eStatus || EN_INITOK != rValue.GetStatus())
    {
        return 1;
    }

    //不能超过67年, 否则会导致返回值溢出
    int iSpanYears = m_iYear - rValue.GetYear();
    if(iSpanYears > 67 || iSpanYears < -67)
    {
        return 1;
    }

    int iSpanSecs = 0;
    int iSpanDays = 0;
    GetSpanDays(rValue, iSpanDays);

    iSpanSecs  = (rValue.GetHour() - m_iHour) * 3600;
    iSpanSecs += (rValue.GetMin()  - m_iMin)   * 60;
    iSpanSecs += (rValue.GetSec()  - m_iSec);

    iSecs = iSpanDays * 86400 + iSpanSecs;

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
CDateTime& CDateTime::operator = (const CDateTime& dateValue)
{
    if(&dateValue == this || EN_INITOK != dateValue.GetStatus())
    {
        return *this;
    }

    m_iYear     = dateValue.GetYear();
    m_iMonth    = dateValue.GetMonth();
    m_iDay      = dateValue.GetDay();
    m_iHour     = dateValue.GetHour();
    m_iMin      = dateValue.GetMin();
    m_iSec      = dateValue.GetSec();
    m_eDateType = dateValue.GetDateType();
    m_eStatus   = dateValue.GetStatus();

    return *this;
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
CDateTime& CDateTime::operator = (const char* pszTime)
{
    assert(pszTime);

    CDateTime tmpDate(pszTime, (TDateType)strlen(pszTime));
    if(EN_INITOK != tmpDate.GetStatus())
    {
        return *this;
    }

    *this = tmpDate;

    return *this;
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
int CDateTime::operator >  (const CDateTime& dateValue)
{
    int iTmpYear = dateValue.GetYear();
    int iTmpMon  = dateValue.GetMonth();
    int iTmpDay  = dateValue.GetDay();
    int iTmpHour = dateValue.GetHour();
    int iTmpMin  = dateValue.GetMin();
    int iTmpSec  = dateValue.GetSec();

    if( (m_iYear > iTmpYear) || (m_iYear == iTmpYear && m_iMonth > iTmpMon) ||
        (m_iYear == iTmpYear && m_iMonth == iTmpMon && m_iDay > iTmpDay) )
    {
        return 1;
    }

    if(EN_DATETIME == m_eDateType || EN_DATETIME == dateValue.GetDateType())
    {
        if( (m_iHour > iTmpHour) || (m_iHour == iTmpHour && m_iMin > iTmpMin) ||
            (m_iHour == iTmpHour && m_iMin == iTmpMin && m_iSec > iTmpSec) )
        {
            return 1;
        }
    }

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
int CDateTime::operator >= (const CDateTime& dateValue)
{
    int iTmpYear = dateValue.GetYear();
    int iTmpMon  = dateValue.GetMonth();
    int iTmpDay  = dateValue.GetDay();
    int iTmpHour = dateValue.GetHour();
    int iTmpMin  = dateValue.GetMin();
    int iTmpSec  = dateValue.GetSec();

    if( (m_iYear >= iTmpYear) || (m_iYear == iTmpYear && m_iMonth >= iTmpMon) ||
        (m_iYear == iTmpYear && m_iMonth == iTmpMon && m_iDay >= iTmpDay) )
    {
        return 1;
    }

    if(EN_DATETIME == m_eDateType || EN_DATETIME == dateValue.GetDateType())
    {
        if( (m_iHour >= iTmpHour) || (m_iHour == iTmpHour && m_iMin >= iTmpMin) ||
            (m_iHour == iTmpHour && m_iMin == iTmpMin && m_iSec >= iTmpSec) )
        {
            return 1;
        }
    }

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
int CDateTime::operator <  (const CDateTime& dateValue)
{
    int iTmpYear = dateValue.GetYear();
    int iTmpMon  = dateValue.GetMonth();
    int iTmpDay  = dateValue.GetDay();
    int iTmpHour = dateValue.GetHour();
    int iTmpMin  = dateValue.GetMin();
    int iTmpSec  = dateValue.GetSec();

    if( (m_iYear < iTmpYear) || (m_iYear == iTmpYear && m_iMonth < iTmpMon) ||
        (m_iYear == iTmpYear && m_iMonth == iTmpMon && m_iDay < iTmpDay) )
    {
        return 1;
    }

    if(EN_DATETIME == m_eDateType || EN_DATETIME == dateValue.GetDateType())
    {
        if( (m_iHour < iTmpHour) || (m_iHour == iTmpHour && m_iMin < iTmpMin) ||
            (m_iHour == iTmpHour && m_iMin == iTmpMin && m_iSec < iTmpSec) )
        {
            return 1;
        }
    }

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
int CDateTime::operator <= (const CDateTime& dateValue)
{
    int iTmpYear = dateValue.GetYear();
    int iTmpMon  = dateValue.GetMonth();
    int iTmpDay  = dateValue.GetDay();
    int iTmpHour = dateValue.GetHour();
    int iTmpMin  = dateValue.GetMin();
    int iTmpSec  = dateValue.GetSec();

    if( (m_iYear <= iTmpYear) || (m_iYear == iTmpYear && m_iMonth <= iTmpMon) ||
        (m_iYear == iTmpYear && m_iMonth == iTmpMon && m_iDay <= iTmpDay) )
    {
        return 1;
    }

    if(EN_DATETIME == m_eDateType || EN_DATETIME == dateValue.GetDateType())
    {
        if( (m_iHour <= iTmpHour) || (m_iHour == iTmpHour && m_iMin <= iTmpMin) ||
            (m_iHour == iTmpHour && m_iMin == iTmpMin && m_iSec <= iTmpSec) )
        {
            return 1;
        }
    }

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
int CDateTime::operator == (const CDateTime& dateValue)
{
    int iTmpYear = dateValue.GetYear();
    int iTmpMon  = dateValue.GetMonth();
    int iTmpDay  = dateValue.GetDay();
    int iTmpHour = dateValue.GetHour();
    int iTmpMin  = dateValue.GetMin();
    int iTmpSec  = dateValue.GetSec();

    if( m_iYear != iTmpYear ||  m_iMonth != iTmpMon || m_iDay != iTmpDay )
    {
        return 0;
    }

    if(EN_DATETIME == m_eDateType || EN_DATETIME == dateValue.GetDateType())
    {
         if( m_iHour != iTmpHour || m_iMin != iTmpMin || m_iSec != iTmpSec )
        {
            return 0;
        }
    }

    return 1;
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
int CDateTime::operator >  (const char* pszTime)
{
    assert(pszTime);

    CDateTime tmpDate(pszTime, (TDateType)strlen(pszTime));
    if(EN_INITOK != tmpDate.GetStatus())
    {
        return 0;
    }

    return *this > tmpDate;
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
int CDateTime::operator >= (const char* pszTime)
{
    assert(pszTime);

    CDateTime tmpDate(pszTime, (TDateType)strlen(pszTime));
    if(EN_INITOK != tmpDate.GetStatus())
    {
        return 0;
    }

    return *this >= tmpDate;
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
int CDateTime::operator <  (const char* pszTime)
{
    assert(pszTime);

    CDateTime tmpDate(pszTime, (TDateType)strlen(pszTime));
    if(EN_INITOK != tmpDate.GetStatus())
    {
        return 0;
    }

    return *this < tmpDate;
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
int CDateTime::operator <= (const char* pszTime)
{
    assert(pszTime);

    CDateTime tmpDate(pszTime, (TDateType)strlen(pszTime));
    if(EN_INITOK != tmpDate.GetStatus())
    {
        return 0;
    }

    return *this <= tmpDate;
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
int CDateTime::operator == (const char* pszTime)
{
    assert(pszTime);

    CDateTime tmpDate(pszTime, (TDateType)strlen(pszTime));
    if(EN_INITOK != tmpDate.GetStatus())
    {
        return 0;
    }

    return *this == tmpDate;
}


//Test code
#if 0
int main()
{
    CDateTime d("20080415235959", CDateTime::EN_DATETIME);
    char sdate[15];
    memset(sdate, 0, sizeof(sdate));
    d.GetDateTimeStr(sdate);
    cout <<"    time:"<<sdate<<endl;

    //for(int i=0; i<365*4+1; ++i)
    {
    //int iRest = d.Add(9999,0,0,0,0,0);
    //d.GetDateTimeStr(sdate);
    //cout <<"add time:"<<sdate<<endl;
    }

    CDateTime dd("20080415235959", CDateTime::EN_DATETIME);
    int iDays = 0;
    dd.GetDateTimeStr(sdate);
    cout<<sdate << endl;
    //dd.GetSpanSecs(d, iDays);
    //cout << iDays <<endl;

    if(dd >= d)
    {
        cout << ">=" << endl;
    }
    else
    {
        cout << "!" << endl;
    }
}
#endif

/********************************** END *******************************************/

