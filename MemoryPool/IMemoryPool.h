// ===============================================================================
// 类名:    
// 版本:    v1.0.0.0
// 作者           :    Qiangp
// 创建时间   :    2016-5-21 18:55:34
// 类描述  :    内存池接口
// ===============================================================================
#pragma once
#include <objbase.h>
namespace memory_pool
{
	interface IMemoryPool
	{
		// 初始化内存池
		virtual void InitMemoryPool() = 0;

		// 向内存池申请内存
		virtual void* AllocMemory(DWORD dwBlockSize) = 0;

		// 向内存池释放内存
		virtual void ReleaseMemory(void *pBlockAddress,DWORD dwBlockSize) = 0;

	};
}