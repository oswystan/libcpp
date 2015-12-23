/*
************************************************************************************
* CopyRight (C) WangYu
*
* FileName     : showerr.cpp
* Author       :
* Description  :
* Function List:
* History      :
************************************************************************************
*/

#include <stdlib.h>
#include <map>
#include <iostream>
#include <fstream>
using namespace std;

#include "lib_splitrecord.h"

#define ERRORINFO_FILE "errinfo.dat"

#define ErrorFields 2

typedef map<int, string> MapErrInfo;

int Usage(char* argv[])
{
    cout << endl;
    cout << "Usage: " << argv[0] << " $errno" << endl;
    cout << endl;
}

int ShowErr(int iErrorNo)
{
    ifstream ifs;
    ifs.open(ERRORINFO_FILE);
    if(!ifs)
    {
        cout << "ERROR: Fail to open the error description file: " << ERRORINFO_FILE << endl;
        return 1;
    }

    CSplitRecord reader;

    string strLine;
    string strErrNo;
    string strDesc;

    int    iLine      = 0;
    int    iReadErrNo = 0;

    MapErrInfo errMap;

    while(getline(ifs, strLine))
    {
        ++iLine;
        if(strLine.empty())
        {
            continue;
        }

        reader.Split(strLine.c_str());
        if(ErrorFields != reader.TotalFileds())
        {
            cout << "WARN : Find an invalid record at line: " << iLine << "in file: " << ERRORINFO_FILE << endl;
            continue;
        }

        reader.GetField(0, strErrNo);
        reader.GetField(1, strDesc);

        iReadErrNo = atoi(strErrNo.c_str());
        errMap[iReadErrNo] = strDesc;
    }

    MapErrInfo::iterator itMap;
    itMap = errMap.find(iErrorNo);
    if(itMap != errMap.end())
    {
        cout << endl;
        cout << "      errno: " << iErrorNo << endl;
        cout << "description: " << itMap->second << endl;
        cout << endl;
    }
    else
    {
        cout << "WARN : Can not find the error description of errno " << iErrorNo << endl;
    }

    return 0;
}

int main(int argc, char* argv[])
{
    if(2 != argc)
    {
        return Usage(argv);
    }

    int iErr = atoi(argv[1]);

    return ShowErr(iErr);
}

/********************************** END *******************************************/

