#ifndef _DATA_BUFFER_H_
#define _DATA_BUFFER_H_

#include <pthread.h>
#include <deque>
#include "./RadarDisplay/global.h"
#include "./RadarDisplay/RadarDis.h"

typedef std::deque<s_ppi_video_for_disp*> DeqDispPointer;
typedef DeqDispPointer::iterator DeqDispPointerIte;

#define DISP_BUF_NUM 3600*7

class CDataBuffer{

public:
	CDataBuffer();
 	virtual ~CDataBuffer();
public:
	static int BufferNum;
	int OwnBufferID;
	int CurrentLine;

private:
	pthread_t thread;
	CRadarDisplay* RadarDis;
	int PACKAGE_NUM_ONCE;
	//数据缓存区
	//s_ppi_video_for_disp dispNetPkg[DISP_BUF_NUM];
	s_ppi_video_for_disp dispFilePkg[DISP_BUF_NUM];

	//操纵数据缓存区指针
	DeqDispPointer pDeqEmpty;    	//维护缓冲区，空闲区段的指针队列
	DeqDispPointer pDeqDisp;	//维护缓冲区，已存数据区段的指针队列
public:
	int Init(CRadarDisplay* radar);
	bool initFileDataBuffer(char*filename);
	void SetDisplayPackageOnce(int Oncepackage){PACKAGE_NUM_ONCE = Oncepackage;}
	void ExitThread();
private:
	static void* recv_data(void *);
	void ReadFileLoop();
	
	
private:
	FILE * rpkgfp;
};

#endif//_DATA_BUFFER_H_
