

#ifndef _YH_H_
#define _YH_H_

#include "global.h"
#include "datapool.h"

const int YH_NUM=600*50;
const int CNUM = 4; //连点数

class CYh
{


public:
    CYh( CDataPool *dp );
    ~CYh();

public:
    	void setYhWeight( int yhmul );
	void setYhLen(int len){yhLen=len;}
	int getYhWeight(){return yhDecWeight;}

    	void enableYh(){ bYh = true; }
    	void disableYh(){ bYh = false; }
    	bool isYh(){ return bYh; }
	void setYhWin(bool b){bWin=b;}

public: 
    void yh();
	void updateRegion(void);//更新作图区位置，即从DataPool中获取区域位置数据
	void updateFbp(void);//更新绘图显存区地址

	void updateTransTable();
	void UpdateYHDispaly();

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

private:
	DISPMEMTYPE *WinColorTable;
    	DISPMEMTYPE *PPIColorTable;
public:


    int yhDecWeight;
    int yhLen;
    int yh_index;
    bool bYh;

	bool bWin;

    CDataPool *m_dp;
    int * randomx;
    int * randomy;


    	fb_surface* fbpPPI;//pointer to framebuffer mem
    	s_layer_unit ** layerPPI;//frame buffer layer

  	fb_surface* fbpMINI;//pointer to framebuffer mem
 	s_layer_unit ** layerMINI;//frame buffer layer
	
  	VIDEODATATYPE transTable[256];
};

#endif
