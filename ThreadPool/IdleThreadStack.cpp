// ===============================================================================
// 类名:    
// 版本:    v1.0.0.0
// 作者           :    Qiangp
// 创建时间   :    2016-5-20 13:23:08
// 类描述  :    空闲线程队列
//					  根据局部性原理，刚运行完的线程之内存更容易命中，故采取栈结构存放空闲线程
// ===============================================================================
#include "IdleThreadStack.h"
#include "CriticalLock.h"
#include "IThread.h"

namespace thead_pool
{
	CIdleThreadStack::CIdleThreadStack(void)
	{
		m_pLock = new CCriticalLock();
	}


	CIdleThreadStack::~CIdleThreadStack(void)
	{
		delete m_pLock;
		m_pLock = NULL;
	}

	void CIdleThreadStack::AddIdleThread(IThread *pThread)
	{
		m_pLock->Lock();
		m_idleThreads.push(pThread);
		m_pLock->UnLock();
	}

	IThread* CIdleThreadStack::PopIdleThread()
	{
		IThread *pThread = NULL;
		m_pLock->Lock();
		if (!m_idleThreads.empty())
		{
			pThread = m_idleThreads.top();
			m_idleThreads.pop();
		}
		m_pLock->UnLock();
		return pThread;
	}

	void CIdleThreadStack::CleanAllIdleThreads()
	{
		// 进行到这一步，不会再有线程添加进来，为空就不加锁了
		if (!m_idleThreads.empty())
		{
			m_pLock->Lock();

			IThread *pThread = m_idleThreads.top();

			// 通知线程退出即可，让线程运行完自行销毁
			pThread->NotifyThreadExit();
			m_idleThreads.pop();
			m_pLock->UnLock();
		}
	}
}

