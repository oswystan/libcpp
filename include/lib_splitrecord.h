/*
************************************************************************************
* CopyRight (C) WangYu
*
* FileName     : splitrecord.h
* Author       :
* Description  :
* Function List:
* History      :
************************************************************************************
*/
#ifndef SPLITRECORD_H
#define SPLITRECORD_H

#include <assert.h>
#include <vector>
#include <string>
using namespace std;

class CSplitRecord
{
public:
    CSplitRecord()  {}
    ~CSplitRecord() {}

    //分离字段
    int Split(const char* pszOneRec, const char* pszDelimeter = "|", int iIsLastDelimeter = 1);
    int Split(vector<string> &vecOut, const char* pszOneRec, const char* pszDelimeter = "|",
              int iIsLastDelimeter = 1);

    //获取当前记录的字段数
    int TotalFileds(){ return m_vecFields.size(); }

    //获取指定字段值
    int GetField(int iPos, string &strOut);
    int GetField(int iPos, char*   pszOut);

private:
    vector<string> m_vecFields;
};

#endif /*SPLITRECORD_H*/

/********************************** END *******************************************/

