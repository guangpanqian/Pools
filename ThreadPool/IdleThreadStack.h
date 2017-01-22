// ===============================================================================
// 类名:    
// 版本:    v1.0.0.0
// 作者           :    Qiangp
// 创建时间   :    2016-5-20 13:23:08
// 类描述  :    空闲线程队列
//					  根据局部性原理，刚运行完的线程之内存更容易命中，内存换页的开销小，
//					  故采取栈结构存放空闲线程
// ===============================================================================
#pragma once
#include <stack>
#include <objbase.h>
using namespace std;

interface ILock;
namespace thead_pool
{
	interface IThread;
	class CIdleThreadStack
	{
	public:
		CIdleThreadStack(void);
		~CIdleThreadStack(void);
		// 向空闲线程栈中添加线程
		void AddIdleThread(IThread *pThread);
		// 从空闲线程栈中取线程
		IThread* PopIdleThread();
		// 清理空闲线程
		void CleanAllIdleThreads();
	private:
		ILock *m_pLock;
		stack<IThread*>m_idleThreads;
	};
}

