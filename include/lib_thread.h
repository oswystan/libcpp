/*
************************************************************************************
* CopyRight (C) WangYu
*
* FileName     : thread.h
* Author       :
* Description  :
* Function List:
* History      :
************************************************************************************
*/
#ifndef THREAD_H
#define THREAD_H

#include "lib_wfc_test.h"

#include <unistd.h>
#include <pthread.h>
#include <list>

using namespace std;


//线程状态
enum PThreadState
{
    EN_INIT,
    EN_RUN,
    EN_STOP
};

class CThread
{
public:
    //线程构造函数与析构函数
    CThread() { m_eState = EN_INIT; m_iIsRun = 1; }
    virtual ~CThread() {};

    //线程初始化函数
    virtual int Init(void* pArg) {};

    //线程启动函数, 调用通用线程函数实现线程的启动
    virtual int Start();

    //线程主处理函数
    virtual int Run()  = 0;

    //杀掉当前线程
    virtual int Kill(int iIsForce = 0);

    //异常推出函数
    virtual int OnExit() { return 0; }

    //获取当前线程状态
    PThreadState GetState() { return m_eState; }

    //获取当前线程ID
    pthread_t    GetThreadID() { return m_threadID; }

protected:
    pthread_t    m_threadID;            //线程ID
    PThreadState m_eState;              //当前线程的状态

    int          m_iIsRun;              //线程是否需要运行的标记位
};

//线程终止函数, 用来实现线程被其他线程终止时的异常处理
void gf_ThreadTerminate(void* pObj)
{
    ((CThread*)pObj)->OnExit();
}

//通用线程函数, 调用线程类的Run函数实现线程的启动
void* gf_ThreadRoutine(void* pObj)
{
    if(NULL == pObj)
    {
        return NULL;
    }

    int i1 = pthread_setcanceltype(PTHREAD_CANCEL_ENABLE, NULL);
    int i2 = pthread_setcancelstate(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);

    if(i1 != 0 || i2 != 0)
    {
        PrintErr("Fail to set cancel type or state for the current thread");
        return pObj;
    }

    pthread_cleanup_push(gf_ThreadTerminate, pObj);
    ((CThread*)pObj)->Run();
    pthread_cleanup_pop(0);
    return pObj;
}

class CThreadPool
{
public:
    //构造与析构函数
    CThreadPool()  {}
    ~CThreadPool() {}

    //添加一个线程到线程池
    int AddThread(CThread* pThread, int iRunFlag = 0);

    //从线程池中删除一个线程
    int DelThread(CThread* pThread);

    //启动线程池中的所有线程
    int StartAllThread();

    //终止所有线程
    int StopAllThread(int iForce = 0);

    //启动一个线程
    int StartOneThread(CThread* pThread);

    //终止一个线程
    int StopOneThread(CThread* pThread, int iForce = 0);

    //获取线程池中线程的个数
    int GetCurThreadCnt();

    //获取线程池中活动的线程个数
    int GetActiveThreadCnt();

private:
    list<CThread*> m_lstThreads;
};

//测试线程类
class CTestThread : public CThread
{
public:
    CTestThread(){}
    ~CTestThread(){}

    int Run();
    int OnExit() { PrintInfo("CTestThread is terminated by another thread!"); m_eState = EN_STOP; }
};

#endif /*THREAD_H*/

/********************************** END *******************************************/

