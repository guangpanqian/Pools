// ===============================================================================
// 类名:    
// 版本:    v1.0.0.0
// 作者           :    Qiangp
// 创建时间   :    2016-5-20 12:36:06
// 类描述  :    线程池接口
// ===============================================================================
#pragma once
namespace thead_pool
{
	interface IThreadTask;
	interface IThread;
		
	interface IThreadPool
	{
		// 初始化线程池，需指定线程池中线程个数
		virtual void InitThreadPool(int nThreadCounts) = 0;

		// 销毁线程池，注意次序：先通知线程退出，待线程退出后销毁线程对象，再去释放尚未完成的任务对象
		virtual void DestoryThreadPool() = 0;

		// 线程池是否已经销毁，此时不能再向线程池投放任务
		virtual BOOL IsThreadPoolDestoryed() = 0;

		// 向线程池中投放任务，添加之前查询线程池是否已被销毁，若已被销毁返回FALSE,调用者需要销毁任务
		virtual BOOL AddTask(IThreadTask *pTask) = 0;

		// 取得任务队列中下一个任务
		virtual IThreadTask* GetNextTask() = 0;

		// 从任务队列中获取下一个任务给该线程，若队列为空或者只是想回收线程，那么将线程放入空闲队列
		virtual void AssignNextTask(BOOL bJustChangeState ,IThread *pThread) = 0;
	};
}