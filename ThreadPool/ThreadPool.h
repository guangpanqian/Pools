// ===============================================================================
// 类名:    
// 版本:    v1.0.0.0
// 作者           :    Qiangp
// 创建时间   :    2016-5-20 16:10:13
// 类描述  :    具体线程池类，开放三个接口，1.线程池初始化；2.线程池销毁；3.向线程池投放任务
// ===============================================================================
#pragma once
#include "IdleThreadStack.h"
#include "BusyThreadList.h"
#include "TaskQueue.h"
#include "IThreadPool.h"

namespace thead_pool
{
	interface IThreadPool;
	class CThreadPool:public IThreadPool
	{

	public:
		CThreadPool();
		~CThreadPool();

		virtual BOOL AddTask(IThreadTask *pTask);
		virtual void InitThreadPool(int nThreadCounts);
		virtual void DestoryThreadPool();

	protected:
		virtual BOOL IsThreadPoolDestoryed();
		virtual IThreadTask* GetNextTask();
		virtual void AssignNextTask(BOOL bJustChangeState,IThread *pThread);

	private:

		// 线程池是否已被销毁
		BOOL m_bThreadPoolDestoryed;
		// 线程池状态锁
		ILock* m_pPoolStatusLock;

		// 空闲线程栈
		CIdleThreadStack m_idleThreadStack;
		// 运行线程列表
		CBusyThreadList  m_busyThreadList;
		// 任务队列
		CTaskQueue m_taskQueue;

	};
}