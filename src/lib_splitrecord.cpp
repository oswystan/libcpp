/*
************************************************************************************
* CopyRight (C) WangYu
*
* FileName     : splitrecord.cpp
* Author       :
* Description  :
* Function List:
* History      :
************************************************************************************
*/


#include <string.h>
#include <stdio.h>
#include "lib_splitrecord.h"

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
int CSplitRecord::Split(const char* pszOneRec, const char* pszDelimeter, int iIsLastDelimeter)
{
    assert(pszOneRec);
    assert(pszDelimeter);

    int iLen = strlen(pszDelimeter);
    int iPos1 = 0;
    int iPos2 = 0;

    string strRec = pszOneRec;

    m_vecFields.clear();

    while(string::npos != (iPos2 = strRec.find(pszDelimeter, iPos1)))
    {
        m_vecFields.push_back(strRec.substr(iPos1, iPos2 - iPos1));
        iPos1 = iPos2 + iLen;
    }

    if(0 == iIsLastDelimeter)
    {
        m_vecFields.push_back(strRec.substr(iPos1, strRec.size()-iPos1));
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
int CSplitRecord::Split(vector<string> &vecOut, const char* pszOneRec, const char* pszDelimeter, int iIsLastDelimeter)
{
    assert(pszOneRec);
    assert(pszDelimeter);

    Split(pszOneRec, pszDelimeter, iIsLastDelimeter);
    vecOut = m_vecFields;

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
int CSplitRecord::GetField(int iPos, string &strOut)
{
    if(iPos > m_vecFields.size() || iPos < 0)
    {
        return 1;
    }

    strOut = m_vecFields[iPos];
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
int CSplitRecord::GetField(int iPos, char* pszOut)
{
    assert(pszOut);

    if(iPos > m_vecFields.size() || iPos < 0)
    {
        return 1;
    }

    sprintf(pszOut, "%s", m_vecFields[iPos].c_str());
    return 0;
}

#if 0
////// Test head files ////////
#include <iostream>
////////// END ////////////////
int main()
{
    CSplitRecord sp;
    vector<string> v;
    sp.Split(v, "1|2|3|5|", "|", 1);
    cout <<"Total fields: " << sp.TotalFileds()<<endl;
    for(int i=0; i<sp.TotalFileds(); ++i)
    {
        string s;
        s = v[i];
        cout <<"field[" << i << "]=" << s << endl;
    }
}
#endif
/********************************** END *******************************************/

