/*
************************************************************************************
* CopyRight (C) WangYu
*
* FileName     : pattern_command.cpp
* Author       :
* Description  :
* Function List:
* History      :
************************************************************************************
*/

#include <stdlib.h>
#include "lib_pattern_command.h"

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
int CCmdTransaction::AddCmd(CCommandBase* pCmd)
{
    assert(pCmd);
    m_vecCmds.push_back(pCmd);

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
int CCmdTransaction::RmCmd(CCommandBase* pCmd)
{
    if(NULL == pCmd)
    {
        m_vecCmds.clear();
        return 0;
    }

    for(int i=0; i<m_vecCmds.size(); ++i)
    {
        if(pCmd == m_vecCmds[i])
        {
            m_vecCmds.erase(m_vecCmds.begin() + i);
            return 0;
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
int CCmdTransaction::Execute()
{
    int iErr = 0;
    int i    = 0;

    PrintDebug("Begin to execute command(s) ...");

    for(i=0; i<m_vecCmds.size(); ++i)
    {
        if(0 != m_vecCmds[i]->Do())
        {
            iErr = 1;
            break;
        }
    }

    if(iErr)
    {
        PrintDebug("Some errors occurred, roll back");
        for(i=i-1; i>=0; --i)
        {
            m_vecCmds[i]->UnDo();
        }
    }
    else
    {
        for(i=0; i<m_vecCmds.size(); ++i)
        {
            if(0 != m_vecCmds[i]->Commit())
            {
                iErr = 1;
                break;
            }
        }
    }

    if(!iErr)
    {
        PrintDebug("Execute all commands OK");
    }
    return iErr;
}

////////////////////////////// Test Class Defination ////////////////////////
int CCopyCmd::SetFileName(const string &src, const string &dst)
{
    m_strSrc = src;
    m_strDst = dst;
    return 0;
}

int CCopyCmd::Do()
{
    string strCmd;

    strCmd = "cp " + m_strSrc + " " + m_strDst + " 2>/dev/null";
    if(0 != system(strCmd.c_str()))
    {
        PrintErr("Fail to do command [" << strCmd << "]");
        return 1;
    }

    return 0;
}

int CCopyCmd::UnDo()
{
    string strCmd;

    strCmd = "rm -f " + m_strDst + " 2>/dev/null";
    if(0 != system(strCmd.c_str()))
    {
        PrintErr("Fail to do command [" << strCmd << "]");
        return 1;
    }

    return 0;
}

int CMoveCmd::SetFileName(const string &src, const string &dst)
{
    m_strSrc = src;
    m_strDst = dst;
    return 0;
}

int CMoveCmd::Do()
{
    string strCmd;

    strCmd = "mv " + m_strSrc + " " + m_strDst + " 2>/dev/null";
    if(0 != system(strCmd.c_str()))
    {
        PrintErr("Fail to do command [" << strCmd << "]");
        return 1;
    }

    return 0;
}

int CMoveCmd::UnDo()
{
    string strCmd;

    strCmd = strCmd = "mv " + m_strDst + " " + m_strSrc + " 2>/dev/null";
    if(0 != system(strCmd.c_str()))
    {
        PrintErr("Fail to do command [" << strCmd << "]");
        return 1;
    }

    return 0;
}
/////////// Test Code ///////////
#if 0
int main()
{
    CCopyCmd a, b, c;
    CMoveCmd d;

    a.SetFileName(string("/home/wong/usr/config.cpp"), string("/home/wong/usr/dir1/config.cpp"));
    b.SetFileName(string("/home/wong/usr/config.h"), string("/home/wong/usr/dir1/config.h"));
    c.SetFileName(string("/home/wong/usr/myfile.txt"), string("/home/wong/usr/dir1/myfile.txt"));
    d.SetFileName(string("/home/wong/usr/1.txt"), string("/home/wong/usr/dir1/1.txt"));

    CCmdTransaction tc;
    tc.AddCmd(&a);
    tc.AddCmd(&b);
    tc.AddCmd(&c);
    tc.AddCmd(&d);

    tc.Execute();

    return 0;
}
#endif

/********************************** END *******************************************/

