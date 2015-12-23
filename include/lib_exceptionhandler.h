/*
************************************************************************************
* CopyRight (C) WangYu
*
* FileName     : exceptionhandler.h
* Author       :
* Description  :
* Function List:
* History      :
************************************************************************************
*/
#ifndef EXCEPTIONHANDLER_H
#define EXCEPTIONHANDLER_H

#include <string>
using namespace std;

class CExceptionHandler
{
public:
    CExceptionHandler(int iError = 0, const char* errDescription = NULL, const char* errSolution = NULL)
    {
        if(0 == iError)
        {
            m_iErrorNo    = 0;
            m_strDes      = "";
            m_strSolution = "";
        }
        else
        {
            m_iErrorNo = iError;
            if(NULL == errDescription)
            {
                m_strDes = "";
            }
            else
            {
                m_strDes = "";
            }

            if(NULL == errSolution)
            {
                m_strSolution = "";
            }
            else
            {
                m_strSolution = errSolution;
            }
        }
    }
    ~CExceptionHandler(){}

    void Clear()
    {
        m_iErrorNo    = 0;
        m_strDes      = "";
        m_strSolution = "";
    }

    int    m_iErrorNo;
    string m_strDes;
    string m_strSolution;
};

#endif /*EXCEPTIONHANDLER_H*/

/********************************** END *******************************************/

