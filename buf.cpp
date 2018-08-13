#include "buf.h"

CBuf::CBuf(size_t uSize)
{
	m_bAlloc = true;
	m_pBuf = (char*)calloc(uSize,1);
	m_pSp = m_pBuf;
}

CBuf::CBuf(char* pBuf)
{
	m_bAlloc = false;
	m_pBuf = pBuf;
	m_pSp = m_pBuf;
}

CBuf::~CBuf()
{
	if(m_bAlloc)
		free(m_pBuf);
}

void CBuf::WriteBuf(const void* pBuf,size_t uSize)
{
	if(pBuf)
		memcpy(m_pSp,pBuf,uSize);
	m_pSp += uSize;
}

void CBuf::ReadBuf(void* pDst,size_t uSize)
{
	if(pDst)
		memcpy(pDst,m_pSp,uSize);
	m_pSp += uSize;
}