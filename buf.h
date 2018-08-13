#ifndef __BUF_H
#define __BUF_H

#include <stdlib.h>
#include <string.h>
#include <stddef.h>

#define MIN(x, y) (((x) < (y)) ? (x) : (y))

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long u64;

class CBuf
{
public:
	CBuf(size_t uSize);
	CBuf(char* pBuf);
	~CBuf();

	void WriteBuf(const void* pSrc,size_t uSize);
	inline void WriteStr(const char* pStr){
		WriteBuf(pStr,strlen(pStr)+1);
	}

	template<typename T>
	void Write(T Obj){
		*(T*)m_pSp = Obj;
		m_pSp += sizeof(T);
	}

	void ReadBuf(void* pDst,size_t uSize);
	inline size_t ReadString(char* pDst,size_t uMaxLen){
		size_t uLen = strlen(m_pSp)+1;
		if(pDst)
			strncpy(pDst,m_pSp,uMaxLen);
		m_pSp += uLen;
		return uLen;
	}
	template<typename T>
	T Read(){
		T Obj = *(T*)m_pSp;
		m_pSp += sizeof(T);
		return Obj;
	}

	inline size_t GetSize(){
		return (size_t)(m_pSp-m_pBuf);
	}

	inline void Reset(){
		m_pSp = m_pBuf;
	}

	char* m_pBuf;
	char* m_pSp;
	bool m_bAlloc;
};

#endif