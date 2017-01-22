// ===============================================================================
// ����:    
// �汾:    v1.0.0.0
// ����           :    Qiangp
// ����ʱ��   :    2016-5-21 19:09:31
// ������  :    �ڴ�ؾ�����
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

	// ��ʼ���ڴ��
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

	// ���ڴ�������ڴ�
	void* CMemoryPool::AllocMemory(DWORD dwBlockSize)
	{
		void *pResult = NULL;
		// ���ڴ���ڴ棬ֱ����Heap����
		if (dwBlockSize > MAX_BLOCK_SIZE)
		{
			pResult = malloc(dwBlockSize);
		}
		// ����С���ڴ棬Ϊ��Ч�ط�ֹ�ڴ���Ƭ�����ڴ����ȡ
		else
		{
	
			DWORD dwBlockSizeRoundUp = ADJUST_BLOCK_SIZE(dwBlockSize);
			PBLOCKNODE pFreeList = m_pFreeList[FREELIST_INDEX(dwBlockSizeRoundUp)];
			
			m_pMemoryLock->Lock();

			// ���������������ʣ���ڴ棬��ôȡ֮
			if (NULL != pFreeList )
			{
				pResult = pFreeList;

				// ������������ָ��
				PBLOCKNODE& pListHeader = m_pFreeList[FREELIST_INDEX(dwBlockSizeRoundUp)];
				pListHeader = pListHeader->pNext;
			}
			// ����������û�У���ô���ڴ����ȡ���������������
			else
			{
				pResult = RefillFreeList(dwBlockSizeRoundUp);
			}
			m_pMemoryLock->UnLock();
		}
		return pResult;
	}

	// ���ڴ�������������
	void* CMemoryPool::RefillFreeList(DWORD dwBlockSizeRoundUp)
	{
		void *pResult = NULL;
		DWORD dwBlockCanAlloc = 20;
		pResult = AllocChunk(dwBlockSizeRoundUp,dwBlockCanAlloc);

		m_pFreeList[FREELIST_INDEX(dwBlockSizeRoundUp)] = PBLOCKNODE(m_pMemoryStart);
		m_pMemoryStart += dwBlockSizeRoundUp;

		PBLOCKNODE pCurrentNode = m_pFreeList[FREELIST_INDEX(dwBlockSizeRoundUp)];
		//��һ���ڵ��Ѿ����ͻ��ˣ��ڶ����Ѿ����ӵ�ͷ�ˣ���ô�±��2��ʼ
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
			// ֻ�ܷ��䲿��
			if(dwBlockPoolCanAlloc >= 1)
			{
				dwBlockCanAlloc = dwBlockPoolCanAlloc;
			}
			// һ��Ҳ���䲻�ˣ���ô��ʱ��Ҫ��System Heap ������
			else
			{
				// �ڴ���л��е���ͷ�������ʵ�����������
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

	// ���ڴ���ͷ��ڴ�
	void CMemoryPool::ReleaseMemory(void *pMemoryAddress,DWORD dwBlockSize)
	{
		if (NULL == pMemoryAddress)
			return;

		// ����ڴ棬ֱ���ͷ���
		if (dwBlockSize > MAX_BLOCK_SIZE)
		{
			free(pMemoryAddress);
		}
		// С���ڴ棬�������������
		else
		{
			DWORD dwBlockSizeRoundUp = ADJUST_BLOCK_SIZE(dwBlockSize);
			int nFreeListIndex = FREELIST_INDEX(dwBlockSizeRoundUp);
			PBLOCKNODE(pMemoryAddress)->pNext = m_pFreeList[nFreeListIndex];
			m_pFreeList[nFreeListIndex] = (PBLOCKNODE)pMemoryAddress;
		}
	}
}
