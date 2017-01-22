// ===============================================================================
// ����:    
// �汾:    v1.0.0.0
// ����           :    Qiangp
// ����ʱ��   :    2016-5-21 19:09:00
// ������  :    �ڴ�ؾ�����,ά��һ������������Ϊ8~128Bytes �ڴ滺������������128ʱֱ�Ӵ�Heap��ȡ
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

		// ��ʼ���ڴ��
		virtual void InitMemoryPool();
		// ���ڴ�������ڴ�
		virtual void* AllocMemory(DWORD dwBlockSize);
		// ���ڴ���ͷ��ڴ�
		virtual void ReleaseMemory(void *pBlockAddress,DWORD dwBlockSize);

	private:

		// ����N��Block
		void* AllocChunk(DWORD dwBlockSize, DWORD& dwBlockCounts);

		// ���ڴ�������������,������������ڴ��
		void* RefillFreeList(DWORD dwBlockSize);

#define MIN_BLOCK_SIZE							(8)
#define MAX_BLOCK_SIZE							(128)
#define FREE_NODE_COUNT						(MAX_BLOCK_SIZE/MIN_BLOCK_SIZE)
#define ADJUST_BLOCK_SIZE(originalsize)				((originalsize + MIN_BLOCK_SIZE - 1)&~(MIN_BLOCK_SIZE -1))
#define FREELIST_INDEX(originalsize)						((originalsize + MIN_BLOCK_SIZE - 1) /MIN_BLOCK_SIZE- 1)

		// �ڴ��ڵ�
		typedef union _block_node
		{
			union _block_node *pNext;
			char *pData;
		}BLOCKNODE,*PBLOCKNODE;

		// �ڴ滥����
		ILock *m_pMemoryLock;

		// ά��һ����������
		PBLOCKNODE m_pFreeList[FREE_NODE_COUNT];

		// �ڴ��ָ�룬�ֱ�ָ���ڴ����ֹλ��
		char* m_pMemoryStart;
		char* m_pMemoryLast; 

		// �����ڴ�ʱ΢�����������ڴ��С�����ӣ�΢������Heap�Ĵ�С
		DWORD m_dwHeapSize;
	};
}

