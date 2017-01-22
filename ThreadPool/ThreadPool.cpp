// ===============================================================================
// 类名:    
// 版本:    v1.0.0.0
// 作者           :    Qiangp
// 创建时间   :    2016-5-20 16:18:10
// 类描述  :    具体线程池类
// ===============================================================================
#include "ThreadPool.h"
#include "IThreadTask.h"
#include "IThread.h"
#include "Thread.h"
#include "CriticalLock.h"
namespace thead_pool
{
	CThreadPool::CThreadPool()
		:m_bThreadPoolDestoryed(FALSE)
		,m_pPoolStatusLock(NULL)
	{

	}

	CThreadPool::~CThreadPool()
	{
		delete m_pPoolStatusLock;
		m_pPoolStatusLock = NULL;
	}

	BOOL CThreadPool::IsThreadPoolDestoryed()
	{
		m_pPoolStatusLock->Lock();
		BOOL bDestoryed = m_bThreadPoolDestoryed;
		m_pPoolStatusLock->UnLock();
		return bDestoryed;
	}

	void CThreadPool::InitThreadPool(int nThreadCounts)
	{
		m_pPoolStatusLock = new CCriticalLock();

		for (int indexThread = 0;indexThread < nThreadCounts;++ indexThread)
		{
			IThread *pThread = new CThread(this);
			m_idleThreadStack.AddIdleThread(pThread);
		}
	}
	    
	BOOL CThreadPool::AddTask(IThreadTask *pTask)
	{
		if (IsThreadPoolDestoryed())
			return FALSE;

		m_taskQueue.AddThreadTask(pTask);
		IThread *pThread = m_idleThreadStack.PopIdleThread();
		if (NULL != pThread)
		{
			pThread->AssignTask(m_taskQueue.GetThreadTask());
			m_busyThreadList.AddBusyThread(pThread);
		}
		return TRUE;
	}

	IThreadTask* CThreadPool::GetNextTask()
	{
		return m_taskQueue.GetThreadTask();
	}

	void CThreadPool::DestoryThreadPool()
	{
		m_pPoolStatusLock->Lock();
		m_bThreadPoolDestoryed = TRUE;
		m_pPoolStatusLock->UnLock();

		m_busyThreadList.CleanAllBusyThreads();
		m_idleThreadStack.CleanAllIdleThreads();
		m_taskQueue.CleanAllTask();
	}

	void CThreadPool::AssignNextTask(BOOL bJustChangeState, IThread *pThread)
	{
		if (IsThreadPoolDestoryed())
			return;

		if (bJustChangeState || m_taskQueue.IsTaskAllComplete())
		{
			m_busyThreadList.DeleteBusyThread(pThread);
			m_idleThreadStack.AddIdleThread(pThread);
		}
		else
		{
			pThread->AssignTask(GetNextTask());
		}
	}
}