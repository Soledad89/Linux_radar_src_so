#include <stdio.h>
#include "DataBuffer.h"
#include   <unistd.h>  //usleep


int CDataBuffer::BufferNum = 0;

CDataBuffer::CDataBuffer()
{
  	rpkgfp = NULL;
        PACKAGE_NUM_ONCE = 30;
	RadarDis = NULL;
	thread = NULL;
        OwnBufferID = ++BufferNum;
        CurrentLine = 0;
}
CDataBuffer::~CDataBuffer()
{

	if(rpkgfp) fclose(rpkgfp);

}

bool CDataBuffer::initFileDataBuffer(char*filename)//./save/radar1
{

	if( (rpkgfp=fopen(filename,"rb"))==NULL ){
		printf("error open pkg file!\n");return false;	
		}

		s_ppi_video_for_disp * dispbuf = dispFilePkg;
		if(!dispbuf)return false;

	size_t result = fread(dispbuf,sizeof(s_ppi_video_for_disp),DISP_BUF_NUM,rpkgfp);
	if(result != DISP_BUF_NUM){
		printf ("Reading error!\n"); return false;
		}

	return true;
}

int CDataBuffer::Init(CRadarDisplay* radar)
{
	this->RadarDis = radar;
//	for(int i=0;i<DISP_BUF_NUM;i++)
//	{
//		pDeqEmpty.push_back(&dispNetPkg[i]);
//	}

	// create thread

	if ( pthread_create( &thread, NULL, recv_data, this ) != 0 )
	{
		printf("create thread failure in Init!\n");
		return -1;
	}

	return 1;
}
void CDataBuffer::ExitThread()
{
	if(thread)
	{
            pthread_cancel (thread);
            pthread_join (thread, NULL);
	}

}
void* CDataBuffer::recv_data(void * arg)
{
	CDataBuffer * pointer = (CDataBuffer *) arg;
   
	if (pointer)
	{ 
		pointer->ReadFileLoop();
	}

}
#define NEWPKGNUM 80

void CDataBuffer::ReadFileLoop()
{
	int i=0;
	s_ppi_video_for_disp * dispbuf = dispFilePkg;

	while(true){
            if(dispbuf&&RadarDis)
                {

                   for( i=0;i<PACKAGE_NUM_ONCE;i++)RadarDis->UpdateDisplay(dispbuf+CurrentLine+i);
                   CurrentLine += PACKAGE_NUM_ONCE;
                   if(CurrentLine+PACKAGE_NUM_ONCE>DISP_BUF_NUM)
                    {
                        //dispbuf = dispFilePkg;
                        CurrentLine =0;
                        rewind(rpkgfp);
                        fread(dispbuf,sizeof(s_ppi_video_for_disp),DISP_BUF_NUM,rpkgfp);
                    }
             }
                usleep(40000);
        }//while

    }        

//void CNetRadar::OnReceive(unsigned char id, short len,char* pData,int pos)
//{
//	static int lastAzi1=0;
//	static int lastAzi2=0;

//	int lastAzi;
//	int curAzi;
//	int i;


//		if(pDeqEmpty.size()>0)
//		{
//			dispbuf=*pDeqEmpty.begin();
//			pDeqEmpty.pop_front();
//		}
//		else return;
//		

//	lastAzi = lastAzi1;
//	curAzi = *(unsigned short*)(pData+2);


//	if(curAzi>lastAzi && curAzi<lastAzi+20)
//	{
//		dispbuf->znum=curAzi-lastAzi;
//		for(i=0;i<dispbuf->znum && i<max_azi_num_per_video;i++)
//			dispbuf->azicnt[i]=lastAzi+i;
//	}
//	else if(curAzi<lastAzi && curAzi<20 && lastAzi > 4096-20)
//	{
//		dispbuf->znum=4096+curAzi-lastAzi;
//		for(i=0;i<dispbuf->znum && i<max_azi_num_per_video;i++)
//			dispbuf->azicnt[i]=(lastAzi+i)%4096;
//	}
//	else
//	{
//		dispbuf->znum=1;
//		dispbuf->azicnt[0]=curAzi;
//	}

//	lastAzi = curAzi;

//	memcpy(dispbuf->videodata,pData+4,len-4);
//	lastAzi1=lastAzi;
//	pDeqDisp.push_back(dispbuf);
//}

