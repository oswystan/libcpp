/*
************************************************************************************
* CopyRight (C) WangYu
*
* FileName     : sockets.cpp
* Author       :
* Description  :
* Function List:
* History      :
************************************************************************************
*/

#include "lib_sockets.h"

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
CSocket::CSocket()
{
    m_socketHandle = -1;
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
CSocket::~CSocket()
{
    Close();
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
int CSocket::ConnectServer(const char* serverIP, int nPort, const char* localIP, int localPort)
{
    assert(serverIP);

    //先关闭以前的socket连接
    Close();

    //对IP和端口进行转换
    struct sockaddr_in stRemote;
    struct sockaddr_in stLocal;
    memset(&stRemote, 0x00, sizeof(sockaddr_in));
    memset(&stLocal,  0x00, sizeof(sockaddr_in));

    stRemote.sin_family = AF_INET;
    stLocal.sin_family  = AF_INET;

    if(inet_pton(AF_INET, serverIP, &stRemote.sin_addr) <= 0)
    {
        SetError();
        return 1;
    }

    stRemote.sin_port = htons(nPort);

    if(NULL != localIP)
    {
        if(inet_pton(AF_INET, localIP, &stLocal.sin_addr) <= 0)
        {
            SetError();
            return 1;
        }
    }
    else
    {
        stLocal.sin_addr.s_addr = htonl(INADDR_ANY);
    }

    if(localPort > 0)
    {
        stLocal.sin_port = htons(localPort);
    }
    else
    {
        stLocal.sin_port = htons(0);
    }

    int tmpSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(tmpSocket < 0)
    {
        SetError();
        return 1;
    }

    //绑定指定IP和端口
    if(NULL != localIP || localPort > 0)
    {
        if(bind(tmpSocket, (sockaddr*)&stLocal, sizeof(sockaddr)) < 0)
        {
            SetError();
            CLOSE_SOCKET(tmpSocket);
            return 1;
        }
    }

    //连接服务器
    if(0 != connect(tmpSocket, (sockaddr*)&stRemote, sizeof(stRemote)))
    {
        SetError();
        CLOSE_SOCKET(tmpSocket);
        return 1;
    }

    m_socketHandle = tmpSocket;

    PrintDebug("Connect server OK");
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
int CSocket::OpenServer(const char* listenIP, int nPort, int nMax)
{
    assert(listenIP);
    assert(nPort > 0);

    //先关闭以前的socket连接
    Close();

    struct sockaddr_in stLocal;
    memset(&stLocal,  0x00, sizeof(sockaddr_in));
    stLocal.sin_family = AF_INET;

    if(inet_pton(AF_INET, listenIP, &stLocal.sin_addr) <= 0)
    {
        SetError();
        return 1;
    }

    stLocal.sin_port = htons(nPort);

    int tmpSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(tmpSocket < 0)
    {
        SetError();
        return 1;
    }

    //绑定指定IP和端口
    if(bind(tmpSocket, (sockaddr*)&stLocal, sizeof(sockaddr)) < 0)
    {
        SetError();
        CLOSE_SOCKET(tmpSocket);
        return 1;
    }

    //监听端口
    if(nMax <= 0)
    {
        nMax = EN_MAXLISTEN;
    }

    if(0 != listen(tmpSocket, nMax))
    {
        SetError();
        CLOSE_SOCKET(tmpSocket);
        return 1;
    }

    m_socketHandle = tmpSocket;

    PrintDebug("Open server OK");
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
int CSocket::AcceptClient(struct sockaddr* clientInfo)
{
    if(m_socketHandle < 0)
    {
        m_socketException.m_iErrorNo = -1;
        m_socketException.m_strDes   = "Server is not opened yet";
        return -1;
    }

    int iClient = -1;
    if(NULL == clientInfo)
    {
        iClient = accept(m_socketHandle, clientInfo, NULL);
    }
    else
    {
        socklen_t addrLen;
        iClient = accept(m_socketHandle, clientInfo, &addrLen);
    }
    if(iClient < 0)
    {
        SetError();
        return -1;
    }
    else
    {
        PrintDebug("Receive one client [" << iClient << "]");
        return iClient;
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
int CSocket::Send(const TCode& sendContent)
{
    if(0 != sendContent.IsValid())
    {
        return 1;
    }

    //准备发送
    int iLeft      = sendContent.iLen;
    int iWrite     = 0;
    char* pContent = sendContent.pBuf;

    while(iLeft > 0)
    {
        iWrite = write(m_socketHandle, pContent, iLeft);
        if(iWrite < 0)
        {
            if(errno == EINTR)
            {
                iWrite = 0;
            }
            else
            {
                SetError();
                return 1;
            }
        }

        iLeft    -= iWrite;
        pContent += iWrite;
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
int CSocket::ReadData(TCode& receiveBuf)
{
    receiveBuf.Clear();
    char szBuf[1024] = {0};

    int iRead = 0;
    while(0 != (iRead = read(m_socketHandle, szBuf, sizeof(szBuf))))
    {
        if(iRead < 0)
        {
            if(EINTR == errno)
            {
                continue;
            }
            else
            {
                SetError();
                return 1;
            }
        }

        receiveBuf.Append((void*)szBuf, iRead);

        //判断接收缓冲区是否为空
        if(0 == BytesToRead())
        {
            break;
        }
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
int CSocket::BytesToRead()
{
    if(m_socketHandle < 0)
    {
        return 0;
    }

    int iBytes = 0;
    ioctl(m_socketHandle, FIONREAD, &iBytes);

    return iBytes;
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
int CSocket::Close()
{
    if(-1 != m_socketHandle)
    {
        CLOSE_SOCKET(m_socketHandle);
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
int CSocket::SetSocketHandle(int socketHandle)
{
    Close();
    if(socketHandle < 0)
    {
        m_socketHandle = -1;
    }
    else
    {
        m_socketHandle = socketHandle;
    }

    return 0;
}

/////////////////Test CODE ////////////////////////
#if 0
int main()
{
    CSocket sock;
    if(sock.OpenServer("192.168.8.128", 5000) != 0)
    {
        cout << sock.GetLatestError().m_strDes << endl;
        cout << "Fail to create server" << endl;
        return 1;
    }

    int iSock;
    CSocket sockets;
    TCode   code;

    PrintDebug("Server running ...");
    while(1 != (iSock = sock.AcceptClient(NULL)))
    {
        sockets.SetSocketHandle(iSock);
        PrintDebug("Receive a client : " << iSock);
        code.Clear();
        code << "Hello client! " << iSock;
        sockets.Send(code);
    }

    return 0;
}
#endif
/********************************** END *******************************************/

