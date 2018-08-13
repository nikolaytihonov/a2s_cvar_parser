#include "net.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

struct sockaddr_in g_maddr;

class WSAInit
{
public:
	WSAInit(){
		WSAStartup(MAKEWORD(2,2),&m_WSA);
		srand((unsigned int)time(NULL));
	}

	~WSAInit(){
		WSACleanup();
	}

	WSADATA m_WSA;
} g_WSA;

CSocket::CSocket()
{
	s = INVALID_SOCKET;
	socklen = sizeof(struct sockaddr_in);
}

CSocket::CSocket(struct sockaddr_in& addr)
{
	s = INVALID_SOCKET;
	a = addr;
	socklen = sizeof(struct sockaddr_in);
}

CSocket::~CSocket()
{
}

bool CSocket::InitSocket()
{
	return ((s = socket(a.sin_family,SOCK_DGRAM,
		IPPROTO_UDP)) != INVALID_SOCKET);
}

bool CSocket::Bind()
{
	return (bind(s,(const sockaddr*)&a,
		socklen)!= SOCKET_ERROR);
}

void CSocket::Close()
{
	closesocket(s);
}

int CSocket::Recv(CSocket& cl,void* pBuf,size_t uSize,
	DWORD dwTimeOut)
{
	if(dwTimeOut)
	{
		setsockopt(cl.s,SOL_SOCKET,SO_RCVTIMEO,
			(const char*)&dwTimeOut,sizeof(DWORD));
	}

	return recvfrom(s,(char*)pBuf,(int)uSize,0,
		(sockaddr*)&cl.a,&cl.socklen);
}

int CSocket::Send(CSocket& cl,const void* pBuf,size_t uSize,
	DWORD dwTimeOut)
{
	if(dwTimeOut)
	{
		setsockopt(cl.s,SOL_SOCKET,SO_SNDTIMEO,
			(const char*)&dwTimeOut,sizeof(DWORD));
	}

	return sendto(s,(const char*)pBuf,(int)uSize,0,
		(const sockaddr*)&cl.a,cl.socklen);
}

void CSocket::SetNonBlocking(bool bEnable)
{
	u_long uBlock = (u_long)bEnable;
	ioctlsocket(s,FIONBIO,&uBlock);
}