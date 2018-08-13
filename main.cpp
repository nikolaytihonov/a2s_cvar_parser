#include "net.h"
#include "buf.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct {
	u32 id;
	u8 total;
	u8 current;
	u16 size;
} sp_t;

void writebuf(CBuf& bf,size_t sz,const char* pFile)
{
	FILE* fout = fopen(pFile,"wb");
	fwrite(bf.m_pBuf,sz,1,fout);
	fclose(fout);
}

void readsplit(CBuf& bf,sp_t& sp)
{
	bf.Read<u32>();
	sp.id = bf.Read<u32>();
	sp.total = bf.Read<u8>();
	sp.current = bf.Read<u8>();
	sp.size = bf.Read<u16>();
}

int main(int argc,char** argv)
{
	CSocket sd;
	CBuf tt(1024*1024); //UP to 1 MB
	CBuf rd(4096);
	CBuf wd(4096);

	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr(argv[1]);
	addr.sin_port = htons(atoi(argv[2]));
	sd = CSocket(addr);
	sd.InitSocket();

	wd.Write<u32>(-1);
	wd.Write<u8>(0x56);
	wd.Write<u32>(-1);
	sd.Send(sd,wd.m_pBuf,wd.GetSize());
	wd.Reset();
	sd.Recv(sd,rd.m_pBuf,4096);

	rd.Read<u32>();
	rd.Read<u8>();
	u32 token = rd.Read<u32>();
	rd.Reset();
	printf("Received A2S_RULES token %p\n",token);

	wd.Write<u32>(-1);
	wd.Write<u8>(0x56);
	wd.Write<u32>(token);
	sd.Send(sd,wd.m_pBuf,wd.GetSize());
	wd.Reset();

	int psize = sd.Recv(sd,rd.m_pBuf,4096);

	sp_t sp;
	sp.total = 2;
	u16 num = 0;
	int totalsize = 0;
	while(sp.current+1 != sp.total)
	{
		int xsize = 12;
		psize -= 12;
		readsplit(rd,sp);
		if(!sp.current) //First packet
		{
			psize -= 7;
			xsize += 7;
			rd.Read<u32>();
			rd.Read<u8>();
			num = rd.Read<u16>();
			printf("%d cvars\n",num);
		}

		tt.WriteBuf((char*)rd.m_pBuf+xsize,psize);
		totalsize += psize;
		rd.Reset();
		printf("%d/%d packet\n",sp.current+1,sp.total);
		if(sp.current+1 == sp.total)
			break;
		psize = sd.Recv(sd,rd.m_pBuf,4096);
	}

	char szName[MAX_PATH];
	_snprintf(szName,MAX_PATH,"%s_%s.txt",argv[1],argv[2]);
	FILE* fdump = fopen(szName,"w");
	tt.Reset();
	for(int i = 0; i < num; i++)
	{
		char szVarName[MAX_PATH];
		char szVarValue[MAX_PATH];
		tt.ReadString(szVarName,MAX_PATH);
		tt.ReadString(szVarValue,MAX_PATH);
		fprintf(fdump,"%s = %s\n",szVarName,szVarValue);
	}
	fclose(fdump);
	return 0;
}