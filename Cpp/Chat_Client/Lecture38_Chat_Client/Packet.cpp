#include "stdafx.h"
#include "Packet.h"


CPacket::CPacket()
{
	m_chpBuffer = NULL;
	m_chpDataFieldStart = NULL;
	m_chpDataFieldEnd = NULL;
	m_chpBuffer = NULL;
	m_chpReadPos = NULL;
	m_chpWritePos = NULL;
	m_chpBufferExpansion = NULL;
	Initial();
}

CPacket::CPacket(int iBufferSize)
{
	m_chpBuffer = NULL;
	m_chpDataFieldStart = NULL;
	m_chpDataFieldEnd = NULL;
	m_chpReadPos = NULL;
	m_chpWritePos = NULL;
	m_chpBuffer = NULL;
	m_chpBufferExpansion = NULL;
	Initial(iBufferSize);
}

CPacket::CPacket(const CPacket &cSrCPacket)
{
	m_chpBuffer = NULL;
	m_iBufferSize = 0;
	m_chpDataFieldStart = NULL;
	m_chpDataFieldEnd = NULL;
	m_chpReadPos = NULL;
	m_chpWritePos = NULL;
	m_chpBuffer = NULL;
	m_chpBufferExpansion = NULL;
	Initial(cSrCPacket.m_iBufferSize);

	PutData(cSrCPacket.m_chpReadPos, cSrCPacket.m_iDataSize);
}

CPacket::~CPacket()
{
	Release();
}

void CPacket::Initial(int iBufferSize)
{
	m_iBufferSize = iBufferSize;

	if (NULL == m_chpBuffer)
	{
		if (eBUFFER_DEFAULT < m_iBufferSize)
		{
			m_chpBufferExpansion = new char[m_iBufferSize];
			m_chpBuffer = m_chpBufferExpansion;
		}
		else
		{
			m_chpBufferExpansion = NULL;
			m_chpBuffer = m_chaBufferDefault;
		}
	}

	m_chpDataFieldStart = m_chpBuffer;
	m_chpDataFieldEnd = m_chpBuffer + m_iBufferSize;

	m_chpReadPos = m_chpWritePos = m_chpDataFieldStart;

	m_iDataSize = 0;
}

void CPacket::Release(void)
{
	if (NULL != m_chpBufferExpansion)
		delete[] m_chpBufferExpansion;
}

void CPacket::Clear(void)
{
	assert(m_chpBuffer);

	m_chpDataFieldStart = m_chpBuffer;
	m_chpDataFieldEnd = m_chpBuffer + m_iBufferSize;

	m_chpReadPos = m_chpWritePos = m_chpDataFieldStart;

	m_iDataSize = 0;
}

int CPacket::MoveWritePos(int iSize)
{
	if (0 > iSize) return 0;

	if (m_chpWritePos + iSize > m_chpDataFieldEnd)
		return 0;

	m_chpWritePos += iSize;
	m_iDataSize += iSize;

	return iSize;
}

int CPacket::MoveReadPos(int iSize)
{
	if (0 > iSize) return 0;

	if (iSize > m_iDataSize)
		return 0;

	m_chpReadPos += iSize;
	m_iDataSize -= iSize;

	return iSize;
}

CPacket &CPacket::operator = (CPacket &cSrCPacket)
{
	m_chpReadPos = m_chpBuffer;
	m_chpWritePos = m_chpBuffer;

	PutData(cSrCPacket.m_chpReadPos, cSrCPacket.m_iDataSize);

	return *this;
}

CPacket &CPacket::operator << (BYTE byValue)
{
	PutData(reinterpret_cast<char *>(&byValue), sizeof(BYTE)); //캐스팅 기본(char *)&value
	return *this;
}

CPacket &CPacket::operator << (char chValue)
{
	PutData(&chValue, sizeof(char));
	return *this;
}

CPacket &CPacket::operator << (short shValue)
{
	PutData(reinterpret_cast<char *>(&shValue), sizeof(short));
	return *this;
}

CPacket &CPacket::operator << (WORD wValue)
{
	PutData(reinterpret_cast<char *>(&wValue), sizeof(WORD));
	return *this;
}

CPacket &CPacket::operator << (int iValue)
{
	PutData(reinterpret_cast<char *>(&iValue), sizeof(int));

	return *this;
}

CPacket &CPacket::operator << (DWORD dwValue)
{
	PutData(reinterpret_cast<char *>(&dwValue), sizeof(DWORD));
	return *this;
}

CPacket &CPacket::operator << (float fValue)
{
	PutData(reinterpret_cast<char *>(&fValue), sizeof(float));
	return *this;
}

CPacket &CPacket::operator << (__int64 iValue)
{
	PutData(reinterpret_cast<char *>(&iValue), sizeof(__int64));
	return *this;
}

CPacket &CPacket::operator << (double dValue)
{
	PutData(reinterpret_cast<char *>(&dValue), sizeof(double));
	return *this;
}

CPacket &CPacket::operator >> (BYTE &byValue)
{
	GetData(reinterpret_cast<char *>(&byValue), sizeof(BYTE));
	return *this;
}

CPacket &CPacket::operator >> (char &chValue)
{
	GetData(&chValue, sizeof(char));
	return *this;
}

CPacket &CPacket::operator >> (short &shValue)
{
	GetData(reinterpret_cast<char *>(&shValue), sizeof(short));
	return *this;
}

CPacket &CPacket::operator >> (WORD &wValue)
{
	GetData(reinterpret_cast<char *>(&wValue), sizeof(WORD));
	return *this;
}

CPacket &CPacket::operator >> (int &iValue)
{
	GetData(reinterpret_cast<char *>(&iValue), sizeof(int));
	return *this;
}

CPacket &CPacket::operator >> (DWORD &dwValue)
{
	GetData(reinterpret_cast<char *>(&dwValue), sizeof(DWORD));
	return *this;
}

CPacket &CPacket::operator >> (float &fValue)
{
	GetData(reinterpret_cast<char *>(&fValue), sizeof(float));
	return *this;
}

CPacket &CPacket::operator >> (__int64 &iValue)
{
	GetData(reinterpret_cast<char *>(&iValue), sizeof(__int64));
	return *this;
}

CPacket &CPacket::operator >> (double &dValue)
{
	GetData(reinterpret_cast<char *>(&dValue), sizeof(double));
	return *this;
}


int CPacket::GetData(char *chpDest, int iSize)
{
	if (iSize > m_iDataSize)
		return 0;

	memcpy(chpDest, m_chpReadPos, iSize);
	m_chpReadPos += iSize;

	m_iDataSize -= iSize;

	return iSize;

}

int CPacket::GetData(wchar_t *chpDest, int iSize)
{
	if (iSize > m_iDataSize)
		return 0;

	wmemcpy(chpDest, (wchar_t*)m_chpReadPos, iSize);
	m_chpReadPos += iSize;

	m_iDataSize -= iSize;

	return iSize;

}

int CPacket::PutData(char *chpSrc, int iSrcSize)
{
	if (m_chpWritePos + iSrcSize > m_chpDataFieldEnd)
		return 0;

	memcpy(m_chpWritePos, chpSrc, iSrcSize);
	m_chpWritePos += iSrcSize;

	m_iDataSize += iSrcSize;

	return iSrcSize;
}

int CPacket::PutData(wchar_t *chpSrc, int iSrcSize)
{
	if (m_chpWritePos + iSrcSize > m_chpDataFieldEnd)
		return 0;

	wmemcpy((wchar_t *)m_chpWritePos, chpSrc, iSrcSize);
	m_chpWritePos += iSrcSize;

	m_iDataSize += iSrcSize;

	return iSrcSize;
}

int CPacket::GetBufferSize()
{
	return m_iBufferSize;
}

int CPacket::GetDataSize()
{
	return m_iDataSize;
}

char* CPacket::GetBufferPtr()
{
	return m_chpBuffer;
}