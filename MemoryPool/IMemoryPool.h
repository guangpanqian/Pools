// ===============================================================================
// ����:    
// �汾:    v1.0.0.0
// ����           :    Qiangp
// ����ʱ��   :    2016-5-21 18:55:34
// ������  :    �ڴ�ؽӿ�
// ===============================================================================
#pragma once
#include <objbase.h>
namespace memory_pool
{
	interface IMemoryPool
	{
		// ��ʼ���ڴ��
		virtual void InitMemoryPool() = 0;

		// ���ڴ�������ڴ�
		virtual void* AllocMemory(DWORD dwBlockSize) = 0;

		// ���ڴ���ͷ��ڴ�
		virtual void ReleaseMemory(void *pBlockAddress,DWORD dwBlockSize) = 0;

	};
}