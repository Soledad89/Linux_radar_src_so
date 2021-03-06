#ifndef __THU_6BE7D83F_50C1_4007_804D_525118CDCC34__
#define __THU_6BE7D83F_50C1_4007_804D_525118CDCC34__

#ifdef __cplusplus

extern "C"{

#endif

typedef int Status;

//------init------
  Status ML_GVID_Init();
  void  ML_GVID_Exit();
//------PPI------
  Status ML_GVID_OpenPPI(char VideoChannel,short theX, short theY , short theWidth, short theHeight);
  Status ML_GVID_ClosePPI(char VideoChannel);
 Status ML_GVID_SetPPIXY(char VideoChannel ,short theX ,short theY);
 Status ML_GVID_SetPPIWH(char VideoChannel ,short theWidth, short theHeight);
 Status ML_GVID_SetPPIMode(char VideoChannel, short theMode);
 Status ML_GVID_SetPPICenter(char VideoChannel ,short theX ,short theY);
 Status ML_GVID_SetPPIShipDir(char VideoChannel,float theShipDir);
 Status ML_GVID_SetPPIColor(char VideoChannel,unsigned int theColor);
 Status ML_GVID_SetPPIScan(char VideoChannel,int theScan);
 Status ML_GVID_SetPPILineColor(char VideoChannel,unsigned int theColor);
 Status ML_GVID_FrostPPI(char VideoChannel,char theFrost);
 Status ML_GVID_SetPPIDist(char VideoChannel,short theDist);
 Status ML_GVID_SetPPISwitch(char VideoChannel,short theSwitch);
 Status ML_GVID_SetPPIGate(char VideoChannel,char theGate);
 Status ML_GVID_SetPPITail(char VideoChannel,int theTail);

//------AR------
 Status ML_GVID_OpenAr(char VideoChannel,short theX, short theY , short theWidth, short theHeight);
 Status ML_GVID_CloseAr(char VideoChannel);
 Status ML_GVID_SetArXY(char VideoChannel ,short theX ,short theY);
 Status ML_GVID_SetArWH(char VideoChannel ,short theWidth, short theHeight);
 Status ML_GVID_SetArGate (char VideoChannel ,char theGate);
 Status ML_GVID_SetArDist (char VideoChannel ,short theDist);

//------MiniWin------
 Status ML_GVID_OpenMiniWin(char VideoChannel,short theX, short theY , short theWidth, short theHeight);
 Status ML_GVID_CloseMiniWin(char VideoChannel);
 Status ML_GVID_SetMiniWinXY(char VideoChannel ,short theX ,short theY);
 Status ML_GVID_SetMiniWinWH(char VideoChannel ,short theWidth, short theHeight);
 Status ML_GVID_SetMiniWinMode(char VideoChannel, short theMode);
 Status ML_GVID_SetMiniWinColor(char VideoChannel,unsigned int theColor);
 Status ML_GVID_SetMiniWinDistDir(char VideoChannel ,short theDistance ,float theDirection);
 Status ML_GVID_SetMiniWinScale(char VideoChannel,int theScale);

 //------TV------
Status ML_GVID_OpenTV (char VideoChannel ,short theX, short theY,short theWidth,short theHeight);//29
Status ML_GVID_CloseTV (char VideoChannel );//30
Status ML_GVID_SetTVXY(char VideoChannel ,short theX, short theY);//31
Status ML_GVID_SetTVWH(char VideoChannel ,short theWidth, short theHeight);//32
Status ML_GVID_FrostTV(char VideoChannel,char theFrost);//34
Status ML_GVID_SetTVBright(char VideoChannel,unsigned char theBright);
Status ML_GVID_SetTVContrast(char VideoChannel,unsigned char theContrast);
Status ML_GVID_SetTVColor(char VideoChannel,unsigned char theColor);

 //------------------------------------------------------------------------------
typedef struct  interface_pkg
{
	unsigned short	 pkgNum;
	unsigned short	 azi;
	char video[1200];
}interface_pkg;

 Status tshInputRadarPkg(char VideoChannel,interface_pkg *dat);



// d should be 1200 length
 Status tshARUpdate(char VideoChannel,unsigned char *d);
#ifdef __cplusplus

}

#endif

#endif
