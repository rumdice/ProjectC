#include "stdafx.h"
#include "Ringbuffer.h"
#include "define.h"

CRingBuffer::CRingBuffer()
{
	m_Buffer = NULL;
	m_Head = 0;
	m_Tail = 0;
	m_BufferSize = 0;

	Init(DEFAULT_RING_BUF_SIZE);
}

CRingBuffer::CRingBuffer(int iBufferSize)
{
	m_Buffer = NULL;
	m_Head = 0;
	m_Tail = 0;
	m_BufferSize = 0;

	Init(iBufferSize);
}

CRingBuffer::~CRingBuffer()
{
	if (m_Buffer != NULL)
	{
		free(m_Buffer);
	}

	m_Buffer = NULL;
	m_Head = 0;
	m_Tail = 0;
	m_BufferSize = 0;
}

void CRingBuffer::Init(int iBufferSize)
{
	m_Buffer = (char*)malloc(iBufferSize);
	m_Head = 0;
	m_Tail = 0;
	m_BufferSize = iBufferSize;
}

void CRingBuffer::Resize(int iSize)
{
	m_Buffer = (char*)realloc(m_Buffer, iSize);
	m_Head = 0;
	m_Tail = 0;
	m_BufferSize = iSize;
}

int CRingBuffer::GetBufferSize()
{
	return m_BufferSize;
}

int CRingBuffer::GetUseSize()
{
	int result = 0;
	if (m_Head <= m_Tail)
	{
		result = m_Tail - m_Head;
	}
	else
	{
		result = m_BufferSize - m_Head + m_Tail;
	}

	return result;
}

int CRingBuffer::GetFreeSize()
{
	return m_BufferSize - (GetUseSize() + DEFAULT_RING_BUF_SPACE_SIZE);
}

int CRingBuffer::GetNotBrokenGetSize()
{
	int result = 0;
	if (m_Head <= m_Tail)
	{
		result = m_Tail - m_Head;
	}
	else
	{
		result = m_BufferSize - m_Head;
	}

	return result;
}

int CRingBuffer::GetNotBrokenPutSize()
{
	int result = 0;
	if (m_Tail <= m_Head)
	{
		result = (m_Head - m_Tail) - DEFAULT_RING_BUF_SPACE_SIZE;
	}
	else
	{
		if (m_Head < DEFAULT_RING_BUF_SPACE_SIZE)
		{
			result = (m_BufferSize - m_Tail) - (DEFAULT_RING_BUF_SPACE_SIZE - m_Head);
		}
		else
		{
			result = m_BufferSize - m_Tail;
		}
	}

	return result;
}

int CRingBuffer::Enqueue(char *chpData, int iSize)
{
	int enableWriteSize;
	if (GetFreeSize() < iSize)
	{
		return 0;
		iSize = GetFreeSize();
	}

	if (iSize <= 0)
	{
		return 0;
	}

	if (m_Head <= m_Tail)
	{
		enableWriteSize = m_BufferSize - m_Tail;

		if (enableWriteSize >= iSize)
		{
			memcpy(m_Buffer + m_Tail, chpData, iSize);
			m_Tail += iSize;
		}
		else
		{
			memcpy(m_Buffer + m_Tail, chpData, enableWriteSize);
			memcpy(m_Buffer, chpData + enableWriteSize, iSize - enableWriteSize);
			m_Tail = iSize - enableWriteSize;
		}
	}
	else
	{
		memcpy(m_Buffer + m_Tail, chpData, iSize);
		m_Tail += iSize;
	}

	m_Tail = m_Tail == m_BufferSize ? 0 : m_Tail;
	return iSize;
}

int CRingBuffer::Dequeue(char *chpDest, int iSize)
{
	int enableReadSize;

	if (GetUseSize() < iSize)
		iSize = GetUseSize();

	if (iSize <= 0)
	{
		return 0;
	}

	if (m_Head <= m_Tail)
	{
		memcpy(chpDest, m_Buffer + m_Head, iSize);
		m_Head += iSize;
	}
	else
	{
		enableReadSize = m_BufferSize - m_Head;

		if (enableReadSize >= iSize)
		{
			memcpy(chpDest, m_Buffer + m_Head, iSize);
			m_Head += iSize;
		}
		else
		{
			memcpy(chpDest, m_Buffer + m_Head, enableReadSize);
			memcpy(chpDest + enableReadSize, m_Buffer, iSize - enableReadSize);
			m_Head = iSize - enableReadSize;
		}
	}

	return iSize;
}

int CRingBuffer::Peek(char *chpDest, int iSize)
{
	int enableReadSize;

	if (GetUseSize() < iSize)
		iSize = GetUseSize();

	if (iSize <= 0)
	{
		return 0;
	}

	if (m_Head <= m_Tail)
	{
		memcpy(chpDest, m_Buffer + m_Head, iSize);
	}
	else
	{
		enableReadSize = m_BufferSize - m_Head;

		if (enableReadSize >= iSize)
		{
			memcpy(chpDest, m_Buffer + m_Head, iSize);
		}
		else
		{
			memcpy(chpDest, m_Buffer + m_Head, enableReadSize);
			memcpy(chpDest + enableReadSize, m_Buffer, iSize - enableReadSize);
		}
	}

	return iSize;
}

void CRingBuffer::RemoveData(int iSize)
{
	int removableSize;

	if (GetUseSize() < iSize)
	{
		iSize = GetUseSize();
	}

	if (iSize <= 0)
	{
		return;
	}

	if (m_Head < m_Tail)
	{
		m_Head += iSize;
	}
	else
	{
		removableSize = m_BufferSize - m_Head;

		if (removableSize >= iSize)
		{
			m_Head += iSize;
		}
		else
		{
			m_Head = iSize - removableSize;
		}
	}

	m_Head = m_Head == m_BufferSize ? 0 : m_Head;
}

int CRingBuffer::MoveWritePos(int iSize)
{
	m_Tail += iSize;

	return iSize;
}

void CRingBuffer::ClearBuffer()
{
	m_Head = 0;
	m_Tail = 0;
}

char *CRingBuffer::GetBufferPtr()
{
	return m_Buffer;
}

char* CRingBuffer::GetReadBufferPtr()
{
	return m_Buffer + m_Head;
}

char* CRingBuffer::GetWriteBufferPtr()
{
	return m_Buffer + m_Tail;
}
