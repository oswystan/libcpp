/*
************************************************************************************
* CopyRight (C) WangYu
*
* FileName     : code.cpp
* Author       : WangYu
* Description  : 编码类, 用于实现网络消息的编码;
* Function List:
* History      : 1. 2007-05-03 WangYu 初稿完成;
************************************************************************************
*/

#include "lib_code.h"


/****************************************************************
* Function Name : TCode();
* Description   : 构造函数;
* Calls         : NA;
* Called By     : NA;
* Input         : NA;
* Output        : NA;
* Return        : NA;
* Table Accessed: NA;
* Table Updated : NA;
* Other         : NA;
****************************************************************/
TCode::TCode()
{
    pBuf      = NULL;
    pBegin    = pBuf;
    iLen      = -1;
    iCapacity = -1;
}

/****************************************************************
* Function Name : TCode();
* Description   : 构造函数, 用一个code类构造;
* Calls         : NA;
* Called By     : NA;
* Input         : NA;
* Output        : NA;
* Return        : NA;
* Table Accessed: NA;
* Table Updated : NA;
* Other         : NA;
****************************************************************/
TCode::TCode(const TCode &otherCode)
{
    if(&otherCode == this)
    {
        return;
    }

    if(otherCode.iLen > 0)
    {
        NEW_A(pBuf, char, otherCode.iLen);
        if(NULL == pBuf)
        {
            return;
        }
        memcpy(pBuf, otherCode.pBuf, otherCode.iLen);
        pBegin    = pBuf;
        iLen      = otherCode.iLen;
        iCapacity = iLen;
    }
    else
    {
        pBuf      = NULL;
        pBegin    = pBuf;
        iLen      = -1;
        iCapacity = -1;
    }
}

/****************************************************************
* Function Name : ~TCode();
* Description   : 析构函数, 用来清除内存;
* Calls         : NA;
* Called By     : NA;
* Input         : NA;
* Output        : NA;
* Return        : NA;
* Table Accessed: NA;
* Table Updated : NA;
* Other         : NA;
****************************************************************/
TCode::~TCode()
{
    Clear();
}

/****************************************************************
* Function Name : operator = ();
* Description   : 赋值操作符函数;
* Calls         : otherCode: 用来初始化的code对象;
* Called By     : NA;
* Input         : NA;
* Output        : NA;
* Return        : 返回当前code类的引用;
* Table Accessed: NA;
* Table Updated : NA;
* Other         : NA;
****************************************************************/
TCode& TCode::operator = (TCode &otherCode)
{
    if(&otherCode == this)
    {
        return *this;
    }

    if(otherCode.iLen > 0)
    {
        DELETE(pBuf);
        pBegin    = pBuf;
        iLen      = -1;
        iCapacity = -1;

        NEW_A(pBuf, char, otherCode.iLen);
        if(NULL == pBuf)
        {
            return *this;
        }

        memcpy(pBuf, otherCode.pBuf, otherCode.iLen);
        pBegin    = pBuf;
        iLen      = otherCode.iLen;
        iCapacity = iLen;
    }
    else
    {
        DELETE(pBuf);
        pBegin    = pBuf;
        iLen      = -1;
        iCapacity = -1;
    }

    return *this;
}

/****************************************************************
* Function Name : Assign();
* Description   : 用一个缓冲区指针来初始化一个code类;
* Calls         : NA;
* Called By     : NA;
* Input         : pCode: 指针; iContentLen: 指针内容的大小;
* Output        : NA;
* Return        : 0: 成功; 1: 失败;
* Table Accessed: NA;
* Table Updated : NA;
* Other         : NA;
****************************************************************/
int TCode::Assign(void* pCode, int iContentLen)
{
    if(iContentLen <= 0 || NULL == pCode)
    {
        return 1;
    }

    DELETE(pBuf);
    pBegin    = pBuf;
    iLen      = -1;
    iCapacity = -1;

    NEW_A(pBuf, char, iContentLen);
    if(NULL == pBuf)
    {
        return 1;
    }

    memcpy(pBuf, pCode, iContentLen);
    pBegin    = pBuf;
    iLen      = iContentLen;
    iCapacity = iLen;

    return 0;
}

/****************************************************************
* Function Name : SetBufAddress();
* Description   : 仅初始化指针, 不进行内容的拷贝;
* Calls         : pCode: 指针; iContentLen: 指针内容的大小;
* Called By     : NA;
* Input         : NA;
* Output        : NA;
* Return        : 0: 成功; 1: 失败;
* Table Accessed: NA;
* Table Updated : NA;
* Other         : NA;
****************************************************************/
int TCode::SetBufAddress(void* pCode, int iContentLen)
{
    if(iContentLen <= 0 || NULL == pCode)
    {
        return 1;
    }

    DELETE(pBuf);
    pBegin    = pBuf;
    iLen      = -1;
    iCapacity = -1;

    pBegin    = pBuf;
    iLen      = iContentLen;
    iCapacity = iLen;

    return 0;
}

/****************************************************************
* Function Name : Append();
* Description   : 向当前的缓冲区中添加指定大小的内容;
* Calls         : NA;
* Called By     : NA;
* Input         : pCode: 指针; iContentLen: 指针内容的大小;
* Output        : NA;
* Return        : 当前code类的引用;
* Table Accessed: NA;
* Table Updated : NA;
* Other         : NA;
****************************************************************/
TCode& TCode::Append(const void* pVoid, int iSize)
{
    if(iSize <= 0 || NULL == pVoid)
    {
        return *this;
    }

    char* pTmpBuf = NULL;

    CExceptionHandler eHandle;

    if(iLen <= 0)
    {
        NEW_A(pTmpBuf, char, iSize);
        if(NULL == pTmpBuf)
        {
            eHandle.m_iErrorNo = ECode_AllocateMem;
            eHandle.m_strDes   = MCode_AllocateMemDes;
            throw(eHandle);
            return *this;
        }
        memcpy(pTmpBuf, pVoid, iSize);

        DELETE_A(pBuf);
        pBuf      = pTmpBuf;
        pBegin    = pBuf;
        iLen      = iSize;
        iCapacity = iSize;

        return *this;
    }
    else if(iLen + iSize > iCapacity)
    {
        NEW_A(pTmpBuf, char, 2 * (iCapacity + iSize));
        if(NULL == pTmpBuf)
        {
            eHandle.m_iErrorNo = ECode_AllocateMem;
            eHandle.m_strDes   = MCode_AllocateMemDes;
            throw(eHandle);
            return *this;
        }

        memcpy(pTmpBuf, pBuf, iLen);
        DELETE_A(pBuf);

        pBuf      = pTmpBuf;
        pBegin    = pBuf;
        iCapacity = 2 * (iCapacity + iSize);
    }

    memcpy(pBuf + iLen, pVoid, iSize);
    iLen = iLen + iSize;

    return *this;
}

/****************************************************************
* Function Name : PopUp();
* Description   : 从当前code类的缓冲区中弹出一个指定大小的内容到pOut中;
* Calls         : NA;
* Called By     : NA;
* Input         : iSize: 指定输出缓冲区的大小;
* Output        : pOut: 输出缓冲区;
* Return        : 当前code类的引用;
* Table Accessed: NA;
* Table Updated : NA;
* Other         : NA;
****************************************************************/
TCode& TCode::PopUp(void* pOut, int iSize)
{
    CExceptionHandler eHandle;

    if(iSize <= 0 || iLen < iSize || NULL == pBegin || NULL == pBuf || pBegin < pBuf ||
        pBegin > pBuf + iLen)
    {
        eHandle.m_iErrorNo = ECode_InvalidState;
        eHandle.m_strDes   = MCode_InvalidStateDes;
        throw (eHandle);
        return *this;
    }

    //保证不会越界
    if(pBegin + iSize > pBuf + iLen)
    {
        eHandle.m_iErrorNo = ECode_NotEnoughBuf;
        eHandle.m_strDes   = MCode_NotEnoughBufDes;
        throw (eHandle);
        return *this;
    }

    memcpy(pOut, pBegin, iSize);

    pBegin = pBegin + iSize;
    return *this;
}

/****************************************************************
* Function Name : Clear();
* Description   : 清空缓冲区内容;
* Calls         : NA;
* Called By     : NA;
* Input         : NA;
* Output        : NA;
* Return        : 0: 成功;
* Table Accessed: NA;
* Table Updated : NA;
* Other         : NA;
****************************************************************/
int TCode::Clear()
{
    iLen      = -1;
    iCapacity = -1;
    DELETE(pBuf);
    pBegin    = pBuf;

    return 0;
}

/****************************************************************
* Function Name : IsValid();
* Description   : 判断当前的状态是否合法;
* Calls         : NA;
* Called By     : NA;
* Input         : NA;
* Output        : NA;
* Return        : 0: 合法; 1: 非法;
* Table Accessed: NA;
* Table Updated : NA;
* Other         : NA;
****************************************************************/
int TCode::IsValid() const
{
    if(NULL == pBuf || NULL == pBegin || iLen <= 0 || iCapacity <= 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/****************************************************************
* Function Name : 输出操作符函数;
* Description   : 将缓冲区的内容;
* Calls         : NA;
* Called By     : NA;
* Input         : NA;
* Output        : NA;
* Return        : NA;
* Table Accessed: NA;
* Table Updated : NA;
* Other         : NA;
****************************************************************/
TCode& TCode::operator << (int iInt)
{
    return Append((void*)&iInt, sizeof(iInt));
}

TCode& TCode::operator << (unsigned int iPara)
{
    return Append((void*)&iPara, sizeof(iPara));
}

TCode& TCode::operator << (long lPara)
{
    return Append((void*)&lPara, sizeof(lPara));
}

TCode& TCode::operator << (unsigned long ulPara)
{
    return Append((void*)&ulPara, sizeof(ulPara));
}

TCode& TCode::operator << (short shortPara)
{
    return Append((void*)&shortPara, sizeof(shortPara));
}

TCode& TCode::operator << (unsigned short shortPara)
{
    return Append((void*)&shortPara, sizeof(shortPara));
}

TCode& TCode::operator << (float fPara)
{
    return Append((void*)&fPara, sizeof(float));
}

TCode& TCode::operator << (double dPara)
{
    return Append((void*)&dPara, sizeof(dPara));
}

TCode& TCode::operator << (const char* pPara)
{
    if(NULL == pPara)
    {
        return *this;
    }

    int iCnt = strlen(pPara);
    return Append(pPara, iCnt + 1);
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
TCode& TCode::operator >> (int &iPara)
{
    return PopUp((void*)&iPara, sizeof(int));
}

TCode& TCode::operator >> (unsigned int &iPara)
{
    return PopUp((void*)&iPara, sizeof(iPara));
}

TCode& TCode::operator >> (long &lPara)
{
    return PopUp((void*)&lPara, sizeof(lPara));
}

TCode& TCode::operator >> (unsigned long &ulPara)
{
    return PopUp((void*)&ulPara, sizeof(ulPara));
}

TCode& TCode::operator >> (short &shortPara)
{
    return PopUp((void*)&shortPara, sizeof(shortPara));
}

TCode& TCode::operator >> (unsigned short &shortPara)
{
    return PopUp((void*)&shortPara, sizeof(shortPara));
}

TCode& TCode::operator >> (float &fPara)
{
    return PopUp((void*)&fPara, sizeof(float));
}

TCode& TCode::operator >> (double &dPara)
{
    return PopUp((void*)&dPara, sizeof(dPara));
}

TCode& TCode::operator >> (char* pPara)
{
    if(NULL == pBegin)
    {
        return *this;
    }

    int iCnt = strlen(pBegin);
    return PopUp(pPara, iCnt + 1);
}
////////////////////////////////////////////////////////

/*
int main()
{
    TCode code;
    double i = 3.101001;

    code << "my first day";
    code << i;

    char a[100];
    double j;
    code >> a >> j;
    cout << a << endl;
    printf("%.6f\n", j);

    sprintf(a, "this a good code");
    code.Assign(a, strlen(a) + 1);

    char b[100];
    code >> b;
    cout << b << endl;
}
*/
/********************************** END *******************************************/

