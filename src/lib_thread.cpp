/*
************************************************************************************
* CopyRight (C) WangYu
*
* FileName     : thread.cpp
* Author       :
* Description  :
* Function List:
* History      :
************************************************************************************
*/

#include <sys/select.h>
#include "lib_thread.h"

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
int CThread::Start()
{
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    if(0 != pthread_create(&m_threadID, &attr, gf_ThreadRoutine, (void*)this))
    {
        PrintErr("Fail to create thread");
        pthread_attr_destroy(&attr);
        return 1;
    }

    pthread_attr_destroy(&attr);
    PrintDebug("Create thread OK");
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
int CThread::Kill(int iIsForce)
{
    if(EN_RUN == m_eState)
    {
        if(iIsForce)
        {
            //目前这个取消好像不起作用
            if(0 != pthread_cancel(m_threadID))
            {
                PrintErr("Fail to cancel the thread")
                return 1;
            }
            m_iIsRun = 0;
        }
        else
        {
            m_iIsRun = 0;
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
int CThreadPool::AddThread(CThread* pThread, int iRunFlag)
{
    if(NULL == pThread)
    {
        return 1;
    }

    m_lstThreads.push_back(pThread);

    //如果需要, 则启动线程
    if(1 == iRunFlag)
    {
        if(0 != pThread->Start())
        {
            return 1;
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
int CThreadPool::DelThread(CThread* pThread)
{
    if(NULL == pThread)
    {
        return 1;
    }

    list<CThread*>::iterator it;
    for(it=m_lstThreads.begin(); it!=m_lstThreads.end(); ++it)
    {
        if(*it == pThread)
        {
            break;
        }
    }
    if(it == m_lstThreads.end())
    {
        return 0;
    }

    if(EN_RUN == pThread->GetState())
    {
        pThread->Kill(0);

        while(EN_RUN == pThread->GetState())
        {
            struct timeval tm_wait;
            tm_wait.tv_sec  = 0;
            tm_wait.tv_usec = 2000;

            pThread->Kill(0);
            select(0, NULL, NULL, NULL, &tm_wait);
        }
    }

    if(EN_STOP == pThread->GetState())
    {
        pthread_join(pThread->GetThreadID(), NULL);
    }

    delete(*it);
    *it = NULL;
    m_lstThreads.erase(it);

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
int CThreadPool::StartAllThread()
{
    list<CThread*>::iterator it;
    int iErr = 0;

    for(it=m_lstThreads.begin(); it!=m_lstThreads.end(); ++it)
    {
        if(EN_RUN != (*it)->GetState())
        {
            if(0 != (*it)->Start())
            {
                iErr = 1;
            }
        }
    }

    if(iErr)
    {
        PrintErr("Fail to start some threads");
        return 1;
    }

    PrintDebug("Start all threads OK");
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
int CThreadPool::StopAllThread(int iForce)
{
    list<CThread*>::iterator it;
    int iIsActive = 1;

    while(iIsActive)
    {
        iIsActive = 0;
        for(it=m_lstThreads.begin(); it!=m_lstThreads.end(); ++it)
        {
            if(EN_RUN == (*it)->GetState())
            {
                iIsActive = 1;
                (*it)->Kill(iForce);
            }
        }
        if(iIsActive)
        {
            struct timeval tm_wait;
            tm_wait.tv_sec  = 0;
            tm_wait.tv_usec = 2000;
            select(0, NULL, NULL, NULL, &tm_wait);
        }
    }

    for(it=m_lstThreads.begin(); it!=m_lstThreads.end(); ++it)
    {
        if(EN_STOP == (*it)->GetState())
        {
            pthread_join((*it)->GetThreadID(), NULL);
        }
    }

    PrintDebug("All threads stop");
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
int CThreadPool::StartOneThread(CThread* pThread)
{
    list<CThread*>::iterator it;

    for(it=m_lstThreads.begin(); it!=m_lstThreads.end(); ++it)
    {
        if(pThread == *it)
        {
            break;
        }
    }

    if(it == m_lstThreads.end())
    {
        return 1;
    }

    if(0 != pThread->Start())
    {
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
int CThreadPool::GetCurThreadCnt()
{
    return m_lstThreads.size();
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
int CThreadPool::GetActiveThreadCnt()
{
    list<CThread*>::iterator it;
    int iCnt = 0;

    for(it=m_lstThreads.begin(); it!=m_lstThreads.end(); ++it)
    {
        if(EN_RUN == (*it)->GetState())
        {
            ++iCnt;
        }
    }

    return iCnt;
}

/////////////////////////////////////////////////////////////////////////
#if 0
int CTestThread::Run()
{
    m_eState = EN_RUN;

    PrintInfo("The test thread running...");
    m_iIsRun = 1;

    while(m_iIsRun)
    {
        int i = 0;
        for(i=0; i<10000000; i++);
        //PrintInfo("After 1");
        //pthread_testcancel();
        for(i=0; i<10000000; i++);
        //PrintInfo("After 2");
        //pthread_testcancel();
        for(i=0; i<10000000; i++);
        //PrintInfo("After 3");
        //pthread_testcancel();
        for(i=0; i<10000000; i++);
        PrintInfo("After 4");
        for(i=0; i<10000000; i++);
        PrintInfo("After 5");
        //pthread_testcancel();
    }
    PrintInfo("test will stop")

    m_eState = EN_STOP;
}

int main()
{
    CTestThread *pth;
    CThreadPool tp;

    pth = new CTestThread;
    tp.AddThread(pth);
    sleep(1);
    pth = new CTestThread;
    tp.AddThread(pth);

    tp.StartAllThread();

    cout << "Active=" << tp.GetActiveThreadCnt() << endl;
    sleep(2);

    tp.StopAllThread(0);

    cout << "Active=" << tp.GetActiveThreadCnt() << endl;
    tp.StartAllThread();
    sleep(2);

    tp.StopAllThread(0);
}
#endif
/********************************** END *******************************************/

