// ===============================================================================
// 类名:    
// 版本:    v1.0.0.0
// 作者           :    Qiangp
// 创建时间   :    2016-5-20 13:38:45
// 类描述  :    运行态线程列表
//                    涉及到线程从活动态到空闲态的频繁转换，链表删除节点效率高，故采取list结构
// ===============================================================================
#pragma once
#include <list>
#include <objbase.h>
using namespace  std;
interface ILock;

namespace thead_pool
{
	interface IThread;

	class CBusyThreadList
	{
	public:
		CBusyThreadList(void);
		~CBusyThreadList(void);
		// 向运行线程列表中添加线程
		void AddBusyThread(IThread* pThread);
		// 从运行线程列表中删除线程
		void DeleteBusyThread(IThread* pThread); 
		// 清理运行线程
		void CleanAllBusyThreads();
	private:
		ILock *m_pLock;
		list<IThread*>m_busyThreadList;
	};

}

