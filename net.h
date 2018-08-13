#ifndef __NET_H
#define __NET_H

#include <WinSock2.h>

#define MASTER_DOMAIN "hl2master.steampowered.com"
#define MASTER_PORT 27011
#define MVHDR 0x31

typedef struct {
	SOCKET s;
	struct sockaddr_in a;
	int len;
} socket_t;

class CSocket
{
public:
	CSocket();
	CSocket(struct sockaddr_in& addr);
	~CSocket();

	bool InitSocket();
	bool Bind();
	void Close();

	int Recv(CSocket& cl,void* pBuf,size_t uLen,
		DWORD dwTimeOut = 0);
	int Send(CSocket& cl,const void* pBuf,size_t uLen,
		DWORD dwTimeOut = 0);
	void SetNonBlocking(bool bEnabled);

	bool operator==(const CSocket& Other){
		return (a.sin_addr.s_addr == Other.a.sin_addr.s_addr
			&& a.sin_port == Other.a.sin_port);
	}

	SOCKET s;
	struct sockaddr_in a;
	int socklen;
};

class CMutex
{
public:
	CMutex(){
		m_hMutex = CreateMutex(0,0,0);
	}
	~CMutex(){
		CloseHandle(m_hMutex);
	}

	void Lock(){/*WaitForSingleObject(m_hMutex,INFINITE);*/}
	void Unlock(){/*ReleaseMutex(m_hMutex);*/}

	HANDLE m_hMutex;
};

extern struct sockaddr_in g_maddr;

#endif