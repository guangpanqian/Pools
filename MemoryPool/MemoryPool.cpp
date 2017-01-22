// ===============================================================================
// 类名:    
// 版本:    v1.0.0.0
// 作者           :    Qiangp
// 创建时间   :    2016-5-21 19:09:31
// 类描述  :    内存池具体类
// ===============================================================================
#include "MemoryPool.h"
#include "CriticalLock.h"

namespace memory_pool
{
	CMemoryPool::CMemoryPool(void)
	{
	
	}

	CMemoryPool::~CMemoryPool(void)
	{
		delete m_pMemoryLock;
		m_pMemoryLock = NULL;
	}

	// 初始化内存池
	void CMemoryPool::InitMemoryPool()
	{
		m_dwHeapSize = 0;
		m_pMemoryLock = new CCriticalLock();
		m_pMemoryStart = NULL;
		m_pMemoryLast = NULL;
		for (int indexFreeList = 0;indexFreeList <FREE_NODE_COUNT;++indexFreeList)
		{
			m_pFreeList[indexFreeList] = NULL;
		}
	}

	// 向内存池申请内存
	void* CMemoryPool::AllocMemory(DWORD dwBlockSize)
	{
		void *pResult = NULL;
		// 对于大块内存，直接向Heap申请
		if (dwBlockSize > MAX_BLOCK_SIZE)
		{
			pResult = malloc(dwBlockSize);
		}
		// 对于小块内存，为有效地防止内存碎片，由内存池中取
		else
		{
	
			DWORD dwBlockSizeRoundUp = ADJUST_BLOCK_SIZE(dwBlockSize);
			PBLOCKNODE pFreeList = m_pFreeList[FREELIST_INDEX(dwBlockSizeRoundUp)];
			
			m_pMemoryLock->Lock();

			// 如果自由链表中有剩余内存，那么取之
			if (NULL != pFreeList )
			{
				pResult = pFreeList;

				// 调整自由链表指针
				PBLOCKNODE& pListHeader = m_pFreeList[FREELIST_INDEX(dwBlockSizeRoundUp)];
				pListHeader = pListHeader->pNext;
			}
			// 自由链表中没有，那么从内存池中取，并填充自由链表
			else
			{
				pResult = RefillFreeList(dwBlockSizeRoundUp);
			}
			m_pMemoryLock->UnLock();
		}
		return pResult;
	}

	// 从内存池填充自由链表
	void* CMemoryPool::RefillFreeList(DWORD dwBlockSizeRoundUp)
	{
		void *pResult = NULL;
		DWORD dwBlockCanAlloc = 20;
		pResult = AllocChunk(dwBlockSizeRoundUp,dwBlockCanAlloc);

		m_pFreeList[FREELIST_INDEX(dwBlockSizeRoundUp)] = PBLOCKNODE(m_pMemoryStart);
		m_pMemoryStart += dwBlockSizeRoundUp;

		PBLOCKNODE pCurrentNode = m_pFreeList[FREELIST_INDEX(dwBlockSizeRoundUp)];
		//第一个节点已经给客户端，第二个已经链接到头了，那么下标从2开始
		for (DWORD dwBlockIndex = 2;dwBlockIndex < dwBlockCanAlloc;++dwBlockIndex)
		{
			PBLOCKNODE pNextNode = PBLOCKNODE(m_pMemoryStart);
			pCurrentNode->pNext = pNextNode;

			pCurrentNode = pNextNode;
			m_pMemoryStart += dwBlockSizeRoundUp;
		}
		pCurrentNode->pNext = NULL;
		return pResult;
	}

	void* CMemoryPool::AllocChunk(DWORD dwBlockSizeRoundUp, DWORD& dwBlockCanAlloc)
	{
		void *pResult = NULL;
		int nBytesLeft = m_pMemoryLast - m_pMemoryStart;
		DWORD dwBlockPoolCanAlloc = nBytesLeft /dwBlockSizeRoundUp;

		if (dwBlockPoolCanAlloc < dwBlockCanAlloc)
		{
			// 只能分配部分
			if(dwBlockPoolCanAlloc >= 1)
			{
				dwBlockCanAlloc = dwBlockPoolCanAlloc;
			}
			// 一点也分配不了，那么此时就要从System Heap 请求了
			else
			{
				// 内存池中还有点零头，插入适当的自由链表
				if(nBytesLeft >= MIN_BLOCK_SIZE)
				{
					DWORD dwFreeIndex = FREELIST_INDEX(nBytesLeft);
					((PBLOCKNODE)m_pMemoryStart)->pNext = m_pFreeList[dwFreeIndex];
					m_pFreeList[dwFreeIndex] = (PBLOCKNODE)m_pMemoryStart;
					m_pMemoryStart =   m_pMemoryStart + (dwFreeIndex+1) *MIN_BLOCK_SIZE;
				}

				DWORD dwAllocSize = (dwBlockCanAlloc * 2) * dwBlockSizeRoundUp + ADJUST_BLOCK_SIZE((m_dwHeapSize >>4));
				m_dwHeapSize = dwAllocSize;

				m_pMemoryStart = (char*)malloc(dwAllocSize);
				m_pMemoryLast = m_pMemoryStart + dwAllocSize;
			}
		}
		
		pResult = m_pMemoryStart;
		m_pMemoryStart += dwBlockSizeRoundUp;
		return pResult;
	}

	// 向内存池释放内存
	void CMemoryPool::ReleaseMemory(void *pMemoryAddress,DWORD dwBlockSize)
	{
		if (NULL == pMemoryAddress)
			return;

		// 大块内存，直接释放了
		if (dwBlockSize > MAX_BLOCK_SIZE)
		{
			free(pMemoryAddress);
		}
		// 小块内存，编入自由链表吧
		else
		{
			DWORD dwBlockSizeRoundUp = ADJUST_BLOCK_SIZE(dwBlockSize);
			int nFreeListIndex = FREELIST_INDEX(dwBlockSizeRoundUp);
			PBLOCKNODE(pMemoryAddress)->pNext = m_pFreeList[nFreeListIndex];
			m_pFreeList[nFreeListIndex] = (PBLOCKNODE)pMemoryAddress;
		}
	}
}
