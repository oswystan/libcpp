/*
************************************************************************************
* CopyRight (C) WangYu
*
* FileName     : wfc_test.h
* Author       :
* Description  :
* Function List:
* History      :
************************************************************************************
*/
#ifndef WFC_TEST_H
#define WFC_TEST_H

#include <iostream>
#include <string>
using namespace std;

#ifdef WFC_TEST
    #define PrintErr(info)  { cout << "ERROR:" << info << endl; }
    #define PrintInfo(info) { cout << "INFO :" << info << endl; }
    #define PrintDebug(info){ cout << "DEBUG:" << info << endl; }
#else
    #define PrintErr(info)   {}
    #define PrintInfo(info)  {}
    #define PrintDebug(info) {}
#endif


#endif /*WFC_TEST_H*/

/********************************** END *******************************************/

