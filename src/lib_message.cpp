/*
************************************************************************************
* CopyRight (C) WangYu
*
* FileName     : message.cpp
* Author       :
* Description  :
* Function List:
* History      :
************************************************************************************
*/

#include "lib_message.h"

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
CMessage::CMessage(CMsgFactory &msgFactory)
{
    msgFactory.Register(this);
    m_msgID = MsgID_InvalidMin;
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
CMessage::~CMessage()
{
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
TMsgID CMessage::DecodeMsgID(TCode &codeContent)
{
    int iID = -1;
    codeContent >> iID;

    return (TMsgID)iID;
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
int CMessage::DecodeMsgLength(TCode &codeContent)
{
    int iLen = -1;
    codeContent >> iLen;

    return iLen;
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
CMsgFactory::CMsgFactory()
{
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
CMsgFactory::~CMsgFactory()
{
    map<TMsgID, CMessage*>::iterator itMsg;
    for(itMsg = m_mapRegMsg.begin(); itMsg != m_mapRegMsg.end(); ++itMsg)
    {
        DELETE((*itMsg).second);
    }

    m_mapRegMsg.clear();
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
int CMsgFactory::Register(CMessage* pMsg)
{
    if(NULL == pMsg)
    {
        return 1;
    }

    //鏌ョ湅娑堟伅鏄惁瀛樺湪
    if(m_mapRegMsg.find(pMsg->GetMsgID()) == m_mapRegMsg.end())
    {
        m_mapRegMsg[pMsg->GetMsgID()] = pMsg;
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
CMessage* CMsgFactory::CreateMsg(TMsgID msgID)
{
    map<TMsgID, CMessage*>::iterator itMsg;

    itMsg = m_mapRegMsg.find(msgID);
    if(itMsg != m_mapRegMsg.end())
    {
        return (*itMsg).second->Clone();
    }

    return NULL;
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
CUpdateCfgMsg::CUpdateCfgMsg()
{
    m_msgID      = MsgID_UpdateCfg;
    m_updateType = EN_Reload;
    m_strField   = "";
    m_strItem    = "";
    m_strValue   = "";
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
CUpdateCfgMsg::CUpdateCfgMsg(CMsgFactory &msgFactory):CMessage(msgFactory)
{
    m_msgID      = MsgID_UpdateCfg;
    m_updateType = EN_Reload;
    m_strField   = "";
    m_strItem    = "";
    m_strValue   = "";
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
int CUpdateCfgMsg::Encode(TCode &codeContent)
{
    if(0 != CheckValid())
    {
        return 1;
    }

    try
    {
        codeContent << (int)m_msgID;
        codeContent << GetLength();
        codeContent << m_updateType;
        codeContent << m_strField.c_str();
        codeContent << m_strItem.c_str();
        codeContent << m_strValue.c_str();

        return 0;
    }
    catch(const CExceptionHandler &eHandle)
    {
        PrintErr("Fail to encode: " << eHandle.m_strDes);
        return 1;
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
int CUpdateCfgMsg::Decode(TCode &codeContent)
{
    char szBuf[EN_MAXCFGLEN + 1];

    memset(szBuf, 0x00, sizeof(szBuf));

    try
    {
        codeContent >> m_updateType;
        codeContent >> szBuf;
        m_strField = szBuf;
        memset(szBuf, 0x00, sizeof(szBuf));
        codeContent >> szBuf;
        m_strItem  = szBuf;
        memset(szBuf, 0x00, sizeof(szBuf));
        codeContent >> szBuf;
        m_strValue = szBuf;

        return 0;
    }
    catch(CExceptionHandler &eHandle)
    {
        PrintErr("Fail to decode: " << eHandle.m_strDes);
        return 1;
    }
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
CMessage* CUpdateCfgMsg::Clone()
{
    CUpdateCfgMsg* pMsg = NULL;
    NEW(pMsg, CUpdateCfgMsg);
    if(NULL == pMsg)
    {
        return NULL;
    }

    pMsg->m_msgID      = m_msgID;
    pMsg->m_updateType = m_updateType;
    pMsg->m_strField   = m_strField;
    pMsg->m_strItem    = m_strItem;
    pMsg->m_strValue   = m_strValue;

    return pMsg;
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
int CUpdateCfgMsg::Print()
{
    PrintDebug("CUpdateCfgMsg as follows:");
    PrintDebug("\tm_updateType = " << m_updateType);
    PrintDebug("\tm_strField   = " << m_strField);
    PrintDebug("\tm_strItem    = " << m_strItem);
    PrintDebug("\tm_strValue   = " << m_strValue);
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
int CUpdateCfgMsg::GetLength()
{
    return sizeof(m_updateType) + m_strField.size() + 1 +
           m_strItem.size() + 1 + m_strValue.size() + 1;
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
int CUpdateCfgMsg::CheckValid()
{
    if(m_updateType <= MsgID_InvalidMin || m_updateType >= MsgID_InvalidMax)
    {
        return 1;
    }

    if(EN_Update == m_updateType)
    {
        if(m_strField.empty() || m_strItem.empty() || m_strValue.empty())
        {
            return 1;
        }
    }

    return 0;
}


//////////////////////Test CODE /////////////////////////////////
#if 0
CMsgFactory msgFactory;

int CreateAllMsg()
{
    CMessage *pMsg = NULL;
    NEW(pMsg, CUpdateCfgMsg(msgFactory));
    if(NULL == pMsg)
    {
        return 1;
    }

    return 0;
}

int main()
{
    if(0 != CreateAllMsg())
    {
        cout << "Fail to create all msg" << endl;
        return 1;
    }

    CMessage *pMsg = NULL;
    pMsg = msgFactory.CreateMsg(MsgID_UpdateCfg);
    if(NULL == pMsg)
    {
        cout << "Fail to create msg" << endl;
        return 0;
    }

    CUpdateCfgMsg msg;
    msg.m_updateType = CUpdateCfgMsg::EN_Reload;
    //msg.m_strField   = "system";
    //msg.m_strItem    = "backup";
    //msg.m_strValue   = "0";

    TCode code;
    msg.Encode(code);

    cout << "MessageID    :" << pMsg->DecodeMsgID(code) << endl;
    cout << "MessageLength:" << pMsg->DecodeMsgLength(code) << endl;

    pMsg->Decode(code);
    pMsg->Print();

}
#endif
/********************************** END *******************************************/

