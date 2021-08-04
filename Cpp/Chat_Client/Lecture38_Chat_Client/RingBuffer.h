#ifndef RING_BUFFER_H
#define RING_BUFFER_H

#include "stdafx.h"

class CRingBuffer
{
public:
	CRingBuffer();
	CRingBuffer(int iBufferSize);
	virtual ~CRingBuffer();

public:
	void Init(int iBufferSize);
	void Resize(int iSize);
	int GetBufferSize();
	int GetUseSize();
	int GetFreeSize();
	int GetNotBrokenGetSize();
	int GetNotBrokenPutSize();
	int Enqueue(char *chpData, int iSize);
	int Dequeue(char *chpDest, int iSize);
	int Peek(char *chpDest, int iSize);
	void RemoveData(int iSize);
	int MoveWritePos(int iSize);
	void ClearBuffer();
	char *GetBufferPtr();
	char *GetReadBufferPtr();
	char *GetWriteBufferPtr();

	char* m_Buffer;		
	int m_Head;		
	int m_Tail;
	int m_BufferSize;
};

#endif