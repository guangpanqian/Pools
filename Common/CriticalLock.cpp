// ===============================================================================
// 类名:    
// 版本:    v1.0.0.0
// 作者           :    Qiangp
// 创建时间   :    2016-5-20 13:17:51
// 类描述  :    临界区锁
// ===============================================================================
#include "CriticalLock.h"

CCriticalLock::CCriticalLock(void)
{
	InitializeCriticalSection(&m_critcalSection);
}


CCriticalLock::~CCriticalLock(void)
{
	DeleteCriticalSection(&m_critcalSection);
}

void CCriticalLock::Lock()
{
	EnterCriticalSection(&m_critcalSection);
}

void CCriticalLock::UnLock()
{
	LeaveCriticalSection(&m_critcalSection);
}


