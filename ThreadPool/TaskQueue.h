// ===============================================================================
// 类名:    
// 版本:    v1.0.0.0
// 作者           :    Qiangp
// 创建时间   :    2016-5-20 14:52:27
// 类描述  :任务队列，采用先进先出的队列作为任务容器    
// ===============================================================================
#pragma once
#include <queue>
#include <objbase.h>
using namespace std;
interface ILock;

namespace thead_pool
{

	interface IThreadTask;

	class CTaskQueue
	{
	public:
		CTaskQueue(void);
		~CTaskQueue(void);
		void AddThreadTask(IThreadTask *pThreadTask);
		IThreadTask* GetThreadTask();
		BOOL IsTaskAllComplete();
		void CleanAllTask();
	private:
		ILock *m_pLock;
		queue<IThreadTask*>m_threadTaskQueue;
	};
}

