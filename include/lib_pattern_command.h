/*
************************************************************************************
* CopyRight (C) WangYu
*
* FileName     : pattern_command.h
* Author       :
* Description  :
* Function List:
* History      :
************************************************************************************
*/
#ifndef PATTERN_COMMAND_H
#define PATTERN_COMMAND_H


#include "lib_wfc_test.h"

#include <assert.h>
#include <vector>
using namespace std;

//命令类基类
class CCommandBase
{
public:
    CCommandBase(){}
    virtual ~CCommandBase(){}

    virtual int Do()     = 0;
    virtual int UnDo()   = 0;
    virtual int Commit() = 0;
};


//命令事务类, 调用命令类的函数实现事务操作
class CCmdTransaction
{
public:
    CCmdTransaction(){}
    ~CCmdTransaction(){}

    int AddCmd(CCommandBase* pCmd);
    int RmCmd(CCommandBase* pCmd = NULL);
    int Execute();

private:
    vector<CCommandBase*> m_vecCmds;
};


////////////////////////// Test Class /////////////////////////

class CCopyCmd : public CCommandBase
{
public:
    CCopyCmd(){}
    ~CCopyCmd(){}

    int SetFileName(const string &src, const string &dst);
    int Do();
    int UnDo();
    int Commit(){ return 0; }

private:
    string m_strSrc;
    string m_strDst;
};

class CMoveCmd : public CCommandBase
{
public:
    CMoveCmd(){}
    ~CMoveCmd(){}

    int SetFileName(const string &src, const string &dst);
    int Do();
    int UnDo();
    int Commit(){ return 0; }

private:
    string m_strSrc;
    string m_strDst;
};

#endif /*PATTERN_COMMAND_H*/

/********************************** END *******************************************/

