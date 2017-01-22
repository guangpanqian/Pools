// ===============================================================================
// 类名:    
// 版本:    v1.0.0.0
// 作者           :    Qiangp
// 创建时间   :    2016-5-20 13:15:22
// 类描述  :    临界区锁
// ===============================================================================
#pragma once
#include "ILock.h"
class CCriticalLock:public ILock
{
public:
	CCriticalLock(void);
	~CCriticalLock(void);

	virtual void Lock();
	virtual void UnLock();
private:
	CRITICAL_SECTION m_critcalSection;
};


