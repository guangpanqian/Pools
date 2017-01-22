// ===============================================================================
// 类名:    
// 版本:    v1.0.0.0
// 作者           :    Qiangp
// 创建时间   :    2016-5-20 15:01:37
// 类描述  :    线程池线程具体类
// ===============================================================================
#include "Thread.h"
#include <process.h>
#include "IThreadPool.h"


namespace thead_pool
{
	CThread::CThread(IThreadPool *pThreadPool,BOOL bAutoDelete)
		:m_hThread(NULL)
		,m_hEventTaskArrive(NULL)
		,m_pThreadPool(pThreadPool)
		,m_bRunning(FALSE)
		,m_pThreadTask(NULL)
		,m_bAutoDelete(bAutoDelete)
	{
		CreateTaskNotifyEvent();
		CreateThread();
	}


	CThread::~CThread(void)
	{
		CloseHandle(m_hThread);
		CloseHandle(m_hEventTaskArrive);
	}

	void CThread::CreateTaskNotifyEvent()
	{
		m_hEventTaskArrive = CreateEvent(NULL,TRUE,FALSE,L"");
	}

	void CThread::CreateThread()
	{
		m_bRunning = TRUE;
		m_hThread = (HANDLE)_beginthreadex(NULL,0,ProcThread,this,0,NULL);
	}

	void CThread::NotifyThreadExit()
	{
		m_bRunning = FALSE;
		SetEvent(m_hEventTaskArrive);
		WaitForSingleObject(m_hThread,INFINITE);
	}

	void CThread::Run()
	{
		while(TRUE)
		{
			//倘若没有任务到来，那么本线程挂起，不占用cpu等资源
			DWORD dwWaitResult =  WaitForSingleObject(m_hEventTaskArrive, INFINITE);
			// 如果等到了，先把手头的任务做完，再看是否已被通知退出
			if (WAIT_OBJECT_0 == dwWaitResult)
			{
				// 需要重置事件，以便下次使用
				ResetEvent(m_hEventTaskArrive);

				// 如果有任务那么去处理任务
				if (NULL != m_pThreadTask)
				{
					m_pThreadTask->ProcessTask();
					delete m_pThreadTask;
					m_pThreadTask = NULL;
				}

				// 如果此时已被通知退出，那么不再索要任务了，回收本线程到空闲线程栈
				m_pThreadPool->AssignNextTask(!m_bRunning,this);	

			}
			// 等失败了，出现了某些情况？
			else if (WAIT_FAILED == dwWaitResult)
			{
				// 线程池通知线程退出
				if (!m_bRunning)
				{
					// 如果此时已被通知退出，回收本线程到空闲线程栈
					m_pThreadPool->AssignNextTask(TRUE,this);	
					break;
				}
			}

			// 线程已被回收到空闲线程栈，本线程可以安心退出了
			if (!m_bRunning)
				break;

		}

		if(m_bAutoDelete)
		{
			delete this;
		}
	}

	// 该函数由外部线程队列维护互斥，不会重入
	void CThread::AssignTask(IThreadTask *pTask)
	{
		m_pThreadTask = pTask;
		SetEvent(m_hEventTaskArrive);
	}

	unsigned int __stdcall CThread::ProcThread(void *pParam)
	{
		CThread *pThread = (CThread*)pParam;
		pThread->Run();

		return 0;
	}

}
