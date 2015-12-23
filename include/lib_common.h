/*
************************************************************************************
* CopyRight (C)
*
* FileName     : common.h
* Author       : WangYu
* Description  :
* Function List:
* History      :
************************************************************************************
*/
#ifndef COMMON_H
#define COMMON_H

/********************** Typedef ******************/
typedef int             TINT;
typedef unsigned int    TUINT;
typedef long            TLONG;
typedef unsigned long   TULONG;
typedef const char*     TCPCHAR;
typedef char*           TPCHAR;
typedef short           TSHORT;
typedef unsigned short  TUSHORT;

#ifndef NULL
    #define NULL 0
#endif

#define NEW(pAddr, Object) \
{\
    try\
    {\
        pAddr = new Object;\
    }\
    catch(...)\
    {\
        pAddr = NULL;\
    }\
}

#define DELETE(pAddr)\
{\
    if(NULL != pAddr)\
    {\
        delete pAddr;\
        pAddr = NULL;\
    }\
}

#define NEW_A(pAddr, Object, Cnt)\
{\
    try\
    {\
        pAddr = new Object[Cnt];\
    }\
    catch(...)\
    {\
        pAddr = NULL;\
    }\
}

#define DELETE_A(pAddr)\
{\
    if(NULL != pAddr)\
    {\
        delete []pAddr;\
        pAddr = NULL;\
    }\
}

#endif /*COMMON_H*/

/********************************** END *******************************************/

