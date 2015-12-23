/*
************************************************************************************
* CopyRight (C) WangYu
*
* FileName     : message.h
* Author       :
* Description  :
* Function List:
* History      :
************************************************************************************
*/
#ifndef MESSAGE_H
#define MESSAGE_H

#include <map>
#include "lib_code.h"

enum TMsgID
{
    MsgID_InvalidMin = -1,
    MsgID_UpdateCfg  = 0,
    MsgID_InvalidMax
};

class CMsgFactory;

class CMessage
{
public:
    //构造与析构函数
    CMessage() {}
    CMessage(CMsgFactory &msgFactory);
    virtual ~CMessage();

    //从码流中解析出MsgID
    static TMsgID DecodeMsgID(TCode &codeContent);

    //从码流中解析出消息体的长度
    static int DecodeMsgLength(TCode &codeContent);

    //消息编码函数
    virtual int Encode(TCode &codeContent) = 0;

    //消息解码函数
    virtual int Decode(TCode &codeContent) = 0;

    //克隆函数
    virtual CMessage* Clone() = 0;

    //打印消息的内容
    virtual int Print() = 0;

    //获取消息的长度
    virtual int GetLength() = 0;

    //获取MsgID
    TMsgID GetMsgID() const { return m_msgID; }

    //检查是否合法
    int CheckValid() { return 0; }

protected:
    TMsgID m_msgID;
};

class CMsgFactory
{
public:
    //构造与析构函数
    CMsgFactory();
    ~CMsgFactory();

    //注册消息
    int Register(CMessage* pMsg);

    //按消息ID创建消息
    CMessage* CreateMsg(TMsgID msgID);

protected:
    map<TMsgID, CMessage*> m_mapRegMsg;
};

class CUpdateCfgMsg : public CMessage
{
public:
    enum
    {
        EN_Reload = 0,
        EN_Update = 1
    };

    enum { EN_MAXCFGLEN = 1024 };

public:
    //构造与析构函数
    CUpdateCfgMsg();
    CUpdateCfgMsg(CMsgFactory &msgFactory);
    ~CUpdateCfgMsg() {}

    //消息编码函数
    int Encode(TCode &codeContent);

    //消息解码函数
    int Decode(TCode &codeContent);

    //克隆函数
    CMessage* Clone();

    //打印消息内容
    int Print();

    //获取消息长度
    int GetLength();

    //检查消息是否合法
    int CheckValid();

public:
    int    m_updateType;
    string m_strField;
    string m_strItem;
    string m_strValue;
};

#endif /*MESSAGE_H*/

/********************************** END *******************************************/

