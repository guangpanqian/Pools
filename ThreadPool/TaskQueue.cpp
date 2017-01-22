// ===============================================================================
// 类名:    
// 版本:    v1.0.0.0
// 作者           :    Qiangp
// 创建时间   :    2016-5-20 14:52:27
// 类描述  :任务队列，采用先进先出的队列作为任务容器    
// ===============================================================================
#include "TaskQueue.h"
#include "CriticalLock.h"
#include "IThreadTask.h"

namespace thead_pool
{
	CTaskQueue::CTaskQueue(void)
	{
		m_pLock = new CCriticalLock();
	}

	CTaskQueue::~CTaskQueue(void)
	{
		delete m_pLock;
		m_pLock = NULL;
	}

	void CTaskQueue::AddThreadTask(IThreadTask *pThreadTask)
	{
		m_pLock->Lock();
		m_threadTaskQueue.push(pThreadTask);
		m_pLock->UnLock();
	}

	IThreadTask* CTaskQueue::GetThreadTask()
	{
		IThreadTask *pThreadTask = NULL;
		m_pLock->Lock();
		if (!m_threadTaskQueue.empty())
		{
			pThreadTask = m_threadTaskQueue.front();
			m_threadTaskQueue.pop();
		}
		m_pLock->UnLock();
		return pThreadTask;
	}

	void CTaskQueue::CleanAllTask()
	{
		// 进行到这一步，不会再有任务添加进来，为空就不加锁了
		if (!m_threadTaskQueue.empty())
		{
			m_pLock->Lock();
			IThreadTask *pThreadTask = m_threadTaskQueue.front();
			delete pThreadTask;
			pThreadTask = NULL;
			m_threadTaskQueue.pop();
			m_pLock->UnLock();
		}
	}

	BOOL CTaskQueue::IsTaskAllComplete()
	{
		m_pLock->Lock();
		BOOL bAllComplete =  m_threadTaskQueue.empty();
		m_pLock->UnLock();
		return bAllComplete;
	}

}
