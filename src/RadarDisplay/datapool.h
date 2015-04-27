#ifndef _DATAPOOL_H_
#define _DATAPOOL_H_

#include   <stdlib.h> #include "global.h"
#include "../FrameBuffer/FrameBuffer.h"
#include "painter.h"

class CPainterPool;

class CDataPool
{
public:
   	CDataPool( fb_surface *fbplinearPPI=NULL, fb_surface *fbplinearAR=NULL,fb_surface *fbplinearMINI=NULL);
 	~CDataPool();
	int SetFbDev( fb_surface *fbplinearPPI=NULL, fb_surface *fbplinearAR=NULL,fb_surface *fbplinearMINI=NULL);
private:
	//--------------------------------------    	
	int initDataTable();
    	void freeDataTable();
 	//--------------------------------------    
	
    	void freeFbDev();
	//-------------------------------------- 
    	int initFbLayer();
    	void freeFbLayer();
	//-------------------------------------- 
    	void ClearLayer(int t = 0);
	//-------------------------------------- 
public:
	void SetPPIDisplayColor(int r,int g,int b);
	void SetWinDisplayColor(int r,int g,int b);

	
public:
	int PPI_XSTART;
	int PPI_YSTART;
	int PPI_WIDTH;
	int PPI_HEIGHT;

	int WIN_XSTART;
	int WIN_YSTART;
	int WIN_WIDTH;
	int WIN_HEIGHT;

	int AR_XSTART;
	int AR_YSTART;
	int AR_WIDTH;
	int AR_HEIGHT;

	s_disp_state dispState;
public:
    //从文件中读取的作图地址表
    	int ** addr_near_x;
    	int ** addr_near_y;
    	int ** addr_far_x;
   	 int ** addr_far_y;
    //余辉表
    	int * randomx;
    	int * randomy;
    //一次色彩表
    	DISPMEMTYPE *PPIColorTable;
    	DISPMEMTYPE *WinColorTable;

    	fb_surface* fbpPPI; 
    	fb_surface* fbpAR; 
   	fb_surface* fbpMINI; 

    //frame buffer layer
    	s_layer_unit ** layerPPI;
    	s_layer_unit ** layerAR;
    	s_layer_unit ** layerMINI;
    	CPainterPool *painterPool;	
        s_ship_state shipState;
};

#endif

