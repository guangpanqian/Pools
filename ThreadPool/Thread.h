// ===============================================================================
// 类名:    
// 版本:    v1.0.0.0
// 作者           :    Qiangp
// 创建时间   :    2016-5-20 15:01:37
// 类描述  :    线程池线程具体类
// ===============================================================================
#pragma once
#include "IThread.h"
#include "IThreadTask.h"
namespace thead_pool
{
	interface IThreadPool;
	class CThread:public IThread
	{
	public:
		CThread(IThreadPool *pThreadPool, BOOL bAutoDelete = TRUE);
		~CThread(void);
		virtual void CreateThread();
		virtual void NotifyThreadExit();
		virtual void Run();
		virtual void AssignTask(IThreadTask *pTask);

		static unsigned int __stdcall ProcThread(void *pParam);

	private:
		// 创建任务通知事件
		void CreateTaskNotifyEvent();

		// 线程是否自运行完成后自销毁
		BOOL	m_bAutoDelete;
		// 线程是否在运行
		BOOL m_bRunning;
		// 线程句柄
		HANDLE m_hThread;
		// 事件通知句柄，用以同步任务的到来
		HANDLE m_hEventTaskArrive;
		// 保存线程池指针，当执行完当前任务后，回收当前线程到空闲线程栈
		IThreadPool* m_pThreadPool;
		// 当前执行的任务指针
		IThreadTask* m_pThreadTask; 
	};
}


