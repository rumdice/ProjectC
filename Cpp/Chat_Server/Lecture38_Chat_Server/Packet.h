#ifndef PACKET_H
#define PACKET_H

#include "stdafx.h"

class CPacket
{
public:
	enum en_PACKET
	{
		eBUFFER_DEFAULT = 512	
	};

	CPacket();
	CPacket(int iBufferSize);
	CPacket(const CPacket &cPacket);

	virtual ~CPacket();

	void Initial(int iBufferSize = eBUFFER_DEFAULT);
	void Release();

	void Clear();

	int GetBufferSize();
	int GetDataSize();
	char *GetBufferPtr();

	int MoveWritePos(int iSize);
	int MoveReadPos(int iSize);

	CPacket &operator = (CPacket &cPacket);

	CPacket &operator << (BYTE bValue);
	CPacket &operator << (char cValue);
	CPacket &operator << (short sValue);
	CPacket &operator << (WORD wValue);
	CPacket &operator << (int iValue);
	CPacket &operator << (DWORD dwValue);
	CPacket &operator << (float fValue);
	CPacket &operator << (__int64 iValue);
	CPacket &operator << (double dValue);

	CPacket &operator >> (BYTE &bValue);
	CPacket &operator >> (char &cValue);
	CPacket &operator >> (short &sValue);
	CPacket &operator >> (WORD &wValue);
	CPacket &operator >> (int &iValue);
	CPacket &operator >> (DWORD &dwValue);
	CPacket &operator >> (float &fValue);
	CPacket &operator >> (__int64 &iValue);
	CPacket &operator >> (double &dValue);

	int GetData(char *chpDest, int iSize);
	int GetData(wchar_t *chpDest, int iSize);

	int PutData(char *chpSrc, int iSrcSize);
	int PutData(wchar_t *chpSrc, int iSrcSize);

protected:

	char	m_chaBufferDefault[eBUFFER_DEFAULT];
	char	*m_chpBufferExpansion;

	char	*m_chpBuffer;
	int		m_iBufferSize;

	char	*m_chpDataFieldStart;
	char	*m_chpDataFieldEnd;

	char	*m_chpReadPos;
	char	*m_chpWritePos;

	wchar_t	*m_wchpReadPos;

	int		m_iDataSize;

};


#endif