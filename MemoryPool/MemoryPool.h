// ===============================================================================
// 类名:    
// 版本:    v1.0.0.0
// 作者           :    Qiangp
// 创建时间   :    2016-5-21 19:09:00
// 类描述  :    内存池具体类,维护一租自由链表，作为8~128Bytes 内存缓冲区，当大于128时直接从Heap获取
// ===============================================================================
#pragma once
#include "IMemoryPool.h"
interface ILock;
namespace memory_pool
{
	class CMemoryPool:public IMemoryPool
	{
	public:
		CMemoryPool(void);
		~CMemoryPool(void);

		// 初始化内存池
		virtual void InitMemoryPool();
		// 向内存池申请内存
		virtual void* AllocMemory(DWORD dwBlockSize);
		// 向内存池释放内存
		virtual void ReleaseMemory(void *pBlockAddress,DWORD dwBlockSize);

	private:

		// 申请N个Block
		void* AllocChunk(DWORD dwBlockSize, DWORD& dwBlockCounts);

		// 从内存池填充自由链表,并返回申请的内存块
		void* RefillFreeList(DWORD dwBlockSize);

#define MIN_BLOCK_SIZE							(8)
#define MAX_BLOCK_SIZE							(128)
#define FREE_NODE_COUNT						(MAX_BLOCK_SIZE/MIN_BLOCK_SIZE)
#define ADJUST_BLOCK_SIZE(originalsize)				((originalsize + MIN_BLOCK_SIZE - 1)&~(MIN_BLOCK_SIZE -1))
#define FREELIST_INDEX(originalsize)						((originalsize + MIN_BLOCK_SIZE - 1) /MIN_BLOCK_SIZE- 1)

		// 内存块节点
		typedef union _block_node
		{
			union _block_node *pNext;
			char *pData;
		}BLOCKNODE,*PBLOCKNODE;

		// 内存互斥锁
		ILock *m_pMemoryLock;

		// 维护一组自由链表
		PBLOCKNODE m_pFreeList[FREE_NODE_COUNT];

		// 内存池指针，分别指向内存的起止位置
		char* m_pMemoryStart;
		char* m_pMemoryLast; 

		// 申请内存时微调，随申请内存大小的增加，微调申请Heap的大小
		DWORD m_dwHeapSize;
	};
}

