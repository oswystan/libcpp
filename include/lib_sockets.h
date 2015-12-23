/*
************************************************************************************
* CopyRight (C) WangYu
*
* FileName     : sockets.h
* Author       :
* Description  :
* Function List:
* History      :
************************************************************************************
*/
#ifndef SOCKETS_H
#define SOCKETS_H

#include <assert.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <errno.h>

#include "lib_code.h"
#include "lib_exceptionhandler.h"

#define CLOSE_SOCKET(sockfd) { close(sockfd); sockfd = -1; }

class CSocket
{
    enum { EN_MAXLISTEN = 5 };
public:
    //构造与析构函数
    CSocket();
    ~CSocket();

    //打开对指定服务器的连接
    int ConnectServer(const char* serverIP, int nPort, const char* localIP = NULL, int localPort = 0);

    //以指定IP和端口打开服务器监听
    int OpenServer(const char* listenIP, int nPort, int nMax = EN_MAXLISTEN);

    //接受客户端的连接
    int AcceptClient(struct sockaddr* clientInfo);

    //发送数据
    int Send(const TCode& sendContent);

    //读取数据
    int ReadData(TCode& receiveBuf);

    //关闭套接字
    int Close();

    //关闭指定套节字
    int Close(int iSocketFD);

    //设置套接字
    int SetSocketHandle(int socketHandle);

    //获取当前缓冲区中可读的数据大小
    int BytesToRead();

    //获取内部错误号
    const CExceptionHandler& GetLatestError() { return m_socketException; }

private:
    void SetError()
    {
        m_socketException.m_iErrorNo    = errno;
        m_socketException.m_strDes      = strerror(m_socketException.m_iErrorNo);
        m_socketException.m_strSolution = "";
    }
private:
    int m_socketHandle;

    CExceptionHandler m_socketException;
};

#endif /*SOCKETS_H*/

/********************************** END *******************************************/

