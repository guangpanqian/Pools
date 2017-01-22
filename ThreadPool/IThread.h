// ===============================================================================
// 类名:    
// 版本:    v1.0.0.0
// 作者           :    Qiangp
// 创建时间   :    2016-5-20 12:38:23
// 类描述  :    
// ===============================================================================
#pragma once
#include <objbase.h>
namespace thead_pool
{
	interface IThreadTask;
	interface IThread
	{
		// 创建线程
		virtual void CreateThread() = 0;

		// 通知线程退出
		virtual void NotifyThreadExit() = 0;

		// 线程执行实体
		virtual void Run() = 0;

		// 给线程安排任务:由于每次线程池分派任务，必然是从空闲线程列表中摘取线程，
		// 而该列表互斥保护，故此函数不会被重入，不需加锁
		virtual void AssignTask(IThreadTask *pTask) = 0;
	};
}