#ifndef _RADAR_DISPALY_H_
#define _RADAR_DISPALY_H_

#include "global.h"
#include "datapool.h"
#include "Map.h"
#include "PPI.h"
#include "yh.h"
#include "AR.h"
#include "mark.h"

typedef struct _RECT
{
    int left_x;
    int left_y;

    int width;
    int height;
}RECT,*pRECT;
typedef int Status;
class CRadarDisplay
{
	public:
		CRadarDisplay();
		virtual ~CRadarDisplay();
	public:
	static int ObjectNum;

	public:
		CDataPool *dp;
	public:
		CMap *map; // 地图
		CPPI *ppi; //雷达ppi显示
		CYh  *yh;  //
		CAR *ar;
		CMark *mark;
	private:
    	fb_surface* PPI_Surface;
    	fb_surface* AR_Surface;
    	fb_surface* MINI_WIN_Surface;
        RECT  RadarDisplayRect[3];//TypePPI=0,TypeAR,TypeWIN
	int arStep;
	public:
	void  initState();
	//加载地图信息
	int  LoadMapToPPI(char* filename);
	//画聚标环
	void  initmark();
	void  cleanmark();
	// 
	void UpdateDisplay(s_ppi_video_for_disp *dispdat);
	private:
	bool IsPPIOpenFlag;
	bool IsAROpenFlag;
	bool IsMiniWinOpenFlag;
	//＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊二代函数接口＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊
	public:
	//------PPI------
	 Status ML_GVID_OpenPPI(short theX, short theY , short theWidth, short theHeight);
	 Status ML_GVID_ClosePPI( );
	 Status ML_GVID_SetPPIXY(  short theX ,short theY);
	 Status ML_GVID_SetPPIWH(  short theWidth, short theHeight);
	 Status ML_GVID_SetPPIMode(  short theMode);
	 Status ML_GVID_SetPPICenter(  short theX ,short theY);
	 Status ML_GVID_SetPPIShipDir( float theShipDir);
	 Status ML_GVID_SetPPIColor( unsigned int theColor);
	 Status ML_GVID_SetPPIScan( int theScan);
	 Status ML_GVID_SetPPILineColor( unsigned int theColor);
	 Status ML_GVID_FrostPPI( char theFrost);
	 Status ML_GVID_SetPPIDist( short theDist);
	 Status ML_GVID_SetPPISwitch( short theSwitch);
	 Status ML_GVID_SetPPIGate( char theGate);
	 Status ML_GVID_SetPPITail( int theTail);

	//------AR------
	 Status ML_GVID_OpenAr( short theX, short theY , short theWidth, short theHeight);
	 Status ML_GVID_CloseAr( );
	 Status ML_GVID_SetArXY( short theX ,short theY);
	 Status ML_GVID_SetArWH( short theWidth, short theHeight);
	 Status ML_GVID_SetArGate ( char theGate);
	 Status ML_GVID_SetArDist ( short theDist);

	//------MiniWin------
	 Status ML_GVID_OpenMiniWin(short theX, short theY , short theWidth, short theHeight);
	 Status ML_GVID_CloseMiniWin( );
	 Status ML_GVID_SetMiniWinXY( short theX ,short theY);
	 Status ML_GVID_SetMiniWinWH( short theWidth, short theHeight);
	 Status ML_GVID_SetMiniWinMode( short theMode);
	 Status ML_GVID_SetMiniWinColor(unsigned int theColor);
	 Status ML_GVID_SetMiniWinDistDir( short theDistance ,float theDirection);
	 Status ML_GVID_SetMiniWinScale( int theScale);
}; 
#endif//_RADAR_DISPALY_H_

