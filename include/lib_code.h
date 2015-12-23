/*
************************************************************************************
* CopyRight (C) WangYu
*
* FileName     : code.h
* Author       : WangYu
* Description  : 编码类, 用于实现网络消息的编码;
* Function List:
* History      : 1. 2007-05-03 WangYu 初稿完成;
************************************************************************************
*/
#ifndef CODE_H
#define CODE_H

#include <string.h>
#include "lib_common.h"
#include "lib_exceptionhandler.h"
#include "lib_wfc_test.h"

enum
{
    ECode_InvalidState,
    ECode_AllocateMem,
    ECode_NotEnoughBuf
};

#define MCode_InvalidStateDes "Invalid code state"
#define MCode_AllocateMemDes  "Allocate memory failed"
#define MCode_NotEnoughBufDes "Not engouth buffer to pop up"

class TCode
{
    enum {EN_InitSize = 128};
public:
    TCode();
    TCode(const TCode &otherCode);
    ~TCode();

    //向TCode中追加内容
    TCode& Append(const void* pVoid, int iSize);

    //从TCode中弹出内容
    TCode& PopUp(void* pOut, int iSize);

    //将pCode指向的内容复制一份给TCode
    int Assign(void* pCode, int iContentLen);

    //进行地址的赋值, 并不复制内容
    int SetBufAddress(void* pCode, int iContentLen);

    //清空TCode中的内容
    int Clear();

    //检查数据是否合法
    int IsValid() const;

    TCode& operator = (TCode &otherCode);

    //输入操作符
    TCode& operator << (int iPara);
    TCode& operator << (unsigned int iPara);
    TCode& operator << (long lPara);
    TCode& operator << (unsigned long ulPara);
    TCode& operator << (short shortPara);
    TCode& operator << (unsigned short shortPara);
    TCode& operator << (float  fPara);
    TCode& operator << (double dPara);
    TCode& operator << (const char* pPara);

    //输出操作符
    TCode& operator >> (int &iPara);
    TCode& operator >> (unsigned int &iPara);
    TCode& operator >> (long &lPara);
    TCode& operator >> (unsigned long &ulPara);
    TCode& operator >> (short &shortPara);
    TCode& operator >> (unsigned short &shortPara);
    TCode& operator >> (float  &fPara);
    TCode& operator >> (double &dPara);
    TCode& operator >> (char* pPara);

public:
    int   iLen;             //实际码流的长度
    int   iCapacity;        //记录码流的容量
    char* pBuf;             //码流的缓冲区首指针
    char* pBegin;           //输出时的开始指针
};

#endif /*CODE_H*/

/********************************** END *******************************************/
