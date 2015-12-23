/*
************************************************************************************
* CopyRight (C) WangYu
*
* FileName     : datetime.h
* Author       :
* Description  :
* Function List:
* History      :
************************************************************************************
*/
#ifndef DATETIME_H
#define DATETIME_H

#include <stdio.h>
#include <time.h>

class CDateTime
{
public:
    enum TDateType
    {
        EN_DATE     = 8,    //日期类型
        EN_TIME     = 6,    //时间类型
        EN_DATETIME = 14    //日期时间类型
    };

	enum TStatus
    {
        EN_NOTINIT,         //尚未初始化数据
        EN_INITOK,          //初始化数据完毕
        EN_ERROR            //数据错误
    };

public:
    CDateTime(const time_t tmSrc);
    CDateTime(const struct tm &tmTime);
    CDateTime(const char* pszTime, TDateType eType = EN_DATE);
    CDateTime(int iYear, int iMon, int iDay, int iHour = 0, int iMin = 0, int iSec = 0);
    CDateTime();
    ~CDateTime();

    //返回当前状态
    TStatus   GetStatus()   const { return m_eStatus; }
    TDateType GetDateType() const { return m_eDateType; }

    //返回当前时间
    static int GetCurDateTime(CDateTime& outDate);
    static int GetCurDateTime(char* outDate, int iSize);
    static int CheckValid(const char* pszTime, TDateType eType);
    static int IsLeapYear(int iYear);

    //返回日期中的各个部分的值
    int GetYear()  const { return m_iYear;  }
    int GetMonth() const { return m_iMonth; }
    int GetDay()   const { return m_iDay;   }
    int GetHour()  const { return m_iHour;  }
    int GetMin()   const { return m_iMin;   }
    int GetSec()   const { return m_iSec;   }

    //返回字符串类型的日期值
    void GetDateStr(char* pszOut)
    {
        snprintf(pszOut, EN_DATE + 1, "%04d%02d%02d", m_iYear, m_iMonth, m_iDay);
    }

    void GetTimeStr(char* pszOut)
    {
        snprintf(pszOut, EN_TIME + 1, "%02d%02d%02d", m_iHour, m_iMin, m_iSec);
    }

    void GetDateTimeStr(char* pszOut)
    {
        snprintf(pszOut, EN_DATETIME + 1, "%04d%02d%02d%02d%02d%02d", m_iYear,
                 m_iMonth, m_iDay, m_iHour, m_iMin, m_iSec);
    }

    //实现时间的运算
    int Add(int iYear = 0, int iMon = 0, int iDay = 0, int iHour = 0, int iMin = 0, int iSec = 0);

    //获取2个日期相差的天数
    int GetSpanDays(const CDateTime& rValue, int &iDays) const;

    //获取2个时间相差的秒数(不能超过37年)
    int GetSpanSecs(const CDateTime& rValue, int &iSecs) const;

    //操作符重载
    CDateTime& operator = (const CDateTime& dateValue);
    CDateTime& operator = (const char* pszTime);

    int operator >  (const CDateTime& dateValue);
    int operator >= (const CDateTime& dateValue);
    int operator <  (const CDateTime& dateValue);
    int operator <= (const CDateTime& dateValue);
    int operator == (const CDateTime& dateValue);

    int operator >  (const char* pszTime);
    int operator >= (const char* pszTime);
    int operator <  (const char* pszTime);
    int operator <= (const char* pszTime);
    int operator == (const char* pszTime);

private:
    void Reset()
    {
        m_iYear     = 0;
        m_iMonth    = 1;
        m_iDay      = 1;
        m_iHour     = 0;
        m_iMin      = 0;
        m_iSec      = 0;
		m_eDateType = EN_DATE;
        m_eStatus   = EN_INITOK;
    }
private:
    int m_iYear;
    int m_iMonth;
    int m_iDay;
    int m_iHour;
    int m_iMin;
    int m_iSec;

	TDateType m_eDateType;
    TStatus   m_eStatus;
};

#endif /*DATETIME_H*/

/********************************** END *******************************************/

