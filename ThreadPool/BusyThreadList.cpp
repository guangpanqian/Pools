// ===============================================================================
// 类名:    
// 版本:    v1.0.0.0
// 作者           :    Qiangp
// 创建时间   :    2016-5-20 13:38:45
// 类描述  :    运行态线程列表
//                    涉及到线程从活动态到空闲态的频繁转换，链表删除节点效率高，故采取list结构
// ===============================================================================
#include "BusyThreadList.h"
#include "CriticalLock.h"
#include "IThread.h"

namespace thead_pool
{
	CBusyThreadList::CBusyThreadList(void)
	{
		m_pLock = new CCriticalLock();
	}


	CBusyThreadList::~CBusyThreadList(void)
	{
		delete m_pLock;
		m_pLock = NULL;
	}

	void CBusyThreadList::AddBusyThread(IThread* pThread)
	{
		m_pLock->Lock();
		m_busyThreadList.push_back(pThread);
		m_pLock->UnLock();
	}

	void CBusyThreadList::DeleteBusyThread(IThread* pThread)
	{
		m_pLock->Lock();
		if (!m_busyThreadList.empty())
		{
			m_busyThreadList.remove(pThread);
		}
		m_pLock->UnLock();
	}

	void CBusyThreadList::CleanAllBusyThreads()
	{
		// 进行到这一步，不会再有线程添加进来，为空就不加锁了
		if (!m_busyThreadList.empty())
		{
			m_pLock->Lock();
			list<IThread*>::iterator first = m_busyThreadList.begin();
			list<IThread*>::iterator last = m_busyThreadList.end();
			for(;first!=last;++first)
			{
				// 通知线程退出即可，让线程运行完自行销毁
				(*first)->NotifyThreadExit();
			}
			m_busyThreadList.clear();
			m_pLock->UnLock();
		}
	}
}
