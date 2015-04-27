#include <stdio.h>
#include <stdlib.h>  //exit函数
#include   <unistd.h>  //usleep
#include "ML_GVIDMW_sys.h"
#include "ML_GVIDMW.h"
#include "Timer.h"
#include "Tv.h"
//#include "./Timer/include/timer_manager.h"
#include "./RadarDisplay/RadarDis.h"
#include "DataBuffer.h"
/*导出想要导出的函数*/

#if defined(__GNUC__) && \
        ((__GNUC__ >= 4) || (__GNUC__ == 3 && __GNUC_MINOR__ >= 3))
#define EXPORT __attribute__((visibility("default")))
#else
#define EXPORT
#endif

//#define EXPORT

FILE * debugfp =NULL;

CRadarDisplay* radar1 =NULL;
CDataBuffer* pBuffer1 = NULL;
CRadarDisplay* radar2 =NULL;
CDataBuffer* pBuffer2 = NULL;

bool bInit=false; //初始化函数是否调用

bool bInitPPI[2] = {false,false};
bool bInitAR[2] = {false,false};
bool bInitMiniWin[2] = {false,false};
bool bInitTV[2] = {false,false};
int screenMode = 0;// 0--双屏，横排； 1--双屏，竖排； 2--单屏

bool checkXY(int x,int y)
{
        switch(screenMode)
        {
        case 0:
                if(x<0 || x>=3200 || y<0 || y>=1200)
                        return false;
                break;
        case 1:
                if(x<0 || x>=1600 || y<0 || y>=2400)
                        return false;
                break;
        case 2:
                if(x<0 || x>=1600 || y<0 || y>=1200)
                        return false;
                break;
        }
        return true;
}

//-----------------------------暂时放在这里，还需要进一步的调整－－－－－－－－－－－
// void yh1(CTimer * timer, void *data)
//{
//  for(int i =0;i<2;i++)//3
//        {
//                if(radar1)radar1->yh->yh();
//        }
//
//}
// void yh2(CTimer * timer, void *data)
//{
//  for(int i =0;i<2;i++)//3
//        {
//
//                if(radar2)radar2->yh->yh();
//        }
//
//}
void tshTestYh(int a)
{
  for(int i =0;i<1;i++)//3
        {

                if(radar1)radar1->yh->yh();
                if(radar2)radar2->yh->yh();
        }

}
	//－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－
EXPORT Status ML_GVID_Init()
{
        if(bInit == true) return -1;

	int ret = 1;
	//---- create debug file ----
	if( (debugfp=fopen("./save/dlldebug.txt","w"))==NULL ){
		printf("Erroe in create the debug file!\n");return -1;
	}
	fb_init();
	//-------------------------------------------------------------------------
	printf("start to init radar1\n");
	radar1 =new CRadarDisplay();
	if(!radar1){
		printf("radar1 new mem failure!\n");return -2;}
	fprintf(debugfp,"---------radar1 class created! -----\n");

	ret = radar1->LoadMapToPPI("./bmp_ppm/map.ppm");
		if(ret < 0) {printf("LoadMapToPPI failure!\n");return ret;}
        //radar1->initmark();
	pBuffer1 = new CDataBuffer();
        if(!pBuffer1->initFileDataBuffer("./save/radar1"))
	{
		printf("initFileDataBuffer  failure!\n");
	return -3;
	}
	ret = pBuffer1->Init(radar1);
	if(ret < 0){
			printf("pBuffer1 Init failure!\n");return ret;}
 	
	//-------------------------------------------------------------------------
	printf("start to init radar2\n");	
	radar2 =new CRadarDisplay();
	if(!radar2){
		printf("radar2 new mem failure!\n");return -2;}
	fprintf(debugfp,"---------radar2 class created! -----\n");

	ret = radar2->LoadMapToPPI("./bmp_ppm/map.ppm");
		if(ret < 0) {printf("LoadMapToPPI failure!\n");return ret;}
	pBuffer2 = new CDataBuffer(); 
        if(!pBuffer2->initFileDataBuffer("./save/radar2"))
	{
		printf("initFileDataBuffer  failure!\n");
	return -3;
	}
        ret = pBuffer2->Init(radar2);
	if(ret < 0){
			printf("pBuffer2 Init failure!\n");return ret;}

	printf("start to init yh\n");
	//-------------------------------------------------------------------------
//        CTimerManager::instance()->start(20000,0);
//        CTimer a1(40000,yh1,(void *)1,CTimer::TIMER_CIRCLE);
//        CTimer a2(40000,yh2,(void *)2,CTimer::TIMER_CIRCLE);
//        a1.start();
//        a2.start();
        SetTimer(40000,tshTestYh);

        ret =  OpenTVDLL("./libML_GVIDTV.so");
        ret = InitTVFunctionPointer();
        ret = InitTV();

        bInit=true;

	return ret;
}
//----------------------------------PPI----------------------------------------
EXPORT Status ML_GVID_OpenPPI(char VideoChannel,short theX, short theY , short theWidth, short theHeight)
{
	printf("ML_GVID_OpenPPI------\n");

        INIT_CHECK
        VC_CHECK
        XY_CHECK
        VR_CHECK(theWidth,1,1200)
        VR_CHECK(theHeight,1,1200)

	int ret=0;
	switch(VideoChannel)
	{
		case 0:
			ret = radar1->ML_GVID_OpenPPI(theX,theY,theWidth,theHeight);
		break;
		case 1:
			ret = radar2->ML_GVID_OpenPPI(theX,theY,theWidth,theHeight);             
		break;
		default:
		ret = -5;
		break;
	}
        if(ret > 0)bInitPPI[VideoChannel]= true;
	return ret;
}
EXPORT Status ML_GVID_ClosePPI(char VideoChannel)
{
	printf("ML_GVID_ClosePPI------\n");
        INIT_CHECK
        INIT_PPI_CHECK
        VC_CHECK

	int ret=0;
	switch(VideoChannel)
	{
		case 0:
			ret = radar1->ML_GVID_ClosePPI();
		break;
		case 1:
			ret = radar2->ML_GVID_ClosePPI();
		break;
		default:
		ret = -5;
		break;
	}
        if(ret > 0)bInitPPI[VideoChannel] = false;
	return ret;

}
EXPORT Status ML_GVID_SetPPIXY(char VideoChannel ,short theX ,short theY)
{
	printf("ML_GVID_SetPPIXY------\n");

        INIT_CHECK
        INIT_PPI_CHECK
        VC_CHECK
        XY_CHECK

	int ret=0;
	switch(VideoChannel)
	{
		case 0:
			ret = radar1->ML_GVID_SetPPIXY(theX,theY);
		break;
		case 1:
			ret = radar2->ML_GVID_SetPPIXY(theX,theY);
		break;
		default:
		ret = -5;
		break;
	}
	return ret;
}
 EXPORT Status ML_GVID_SetPPIWH(char VideoChannel ,short theWidth, short theHeight){
	printf("ML_GVID_SetPPIWH------\n");

        INIT_CHECK
        INIT_PPI_CHECK
        VC_CHECK
        VR_CHECK(theWidth,1,1200)
        VR_CHECK(theHeight,1,1200)

	int ret=0;
	switch(VideoChannel)
	{
		case 0:
			ret = radar1->ML_GVID_SetPPIWH(theWidth,theHeight);
		break;
		case 1:
			ret = radar2->ML_GVID_SetPPIWH(theWidth,theHeight);
		break;
		default:
		ret = -5;
		break;
	}
	return ret;
}
EXPORT Status ML_GVID_SetPPIMode(char VideoChannel, short theMode){
	printf("ML_GVID_SetPPIMode------\n");

        INIT_CHECK
        INIT_PPI_CHECK
        VC_CHECK
        VR_CHECK(theMode,0,2)

	int ret=0;
	switch(VideoChannel)
	{
		case 0:
			ret = radar1->ML_GVID_SetPPIMode(theMode);
		break;
		case 1:
			ret = radar2->ML_GVID_SetPPIMode(theMode);
		break;
		default:
		ret = -5;
		break;
	}
	return ret;
}
EXPORT Status ML_GVID_SetPPICenter(char VideoChannel ,short theX ,short theY){
	printf("ML_GVID_SetPPICenter------\n");

        INIT_CHECK
        INIT_PPI_CHECK
        VC_CHECK
        VR_CHECK(theX,0,1199)
        VR_CHECK(theY,0,1199)

	int ret=0;
	switch(VideoChannel)
	{
		case 0:
			ret = radar1->ML_GVID_SetPPICenter(theX,theY);
		break;
		case 1:
			ret = radar2->ML_GVID_SetPPICenter(theX,theY);
		break;
		default:
		ret = -5;
		break;
	}
	return ret;
}
EXPORT Status ML_GVID_SetPPIShipDir(char VideoChannel,float theShipDir){
	printf("ML_GVID_SetPPIShipDir------\n");

        INIT_CHECK
        INIT_PPI_CHECK
        VC_CHECK
        while(theShipDir<0)
                        theShipDir+=360.0;
        while(theShipDir>=360)
                theShipDir -= 360.0;

	int ret=0;
	switch(VideoChannel)
	{
		case 0:
			ret = radar1->ML_GVID_SetPPIShipDir(theShipDir);
		break;
		case 1:
			ret = radar2->ML_GVID_SetPPIShipDir(theShipDir);
		break;
		default:
		ret = -5;
		break;
	}
	return ret;
}
EXPORT Status ML_GVID_SetPPIColor(char VideoChannel,unsigned int theColor){
	printf("ML_GVID_SetPPIColor------\n");

        INIT_CHECK
        INIT_PPI_CHECK
        VC_CHECK

	int ret=0;
	switch(VideoChannel)
	{
		case 0:
			ret = radar1->ML_GVID_SetPPIColor(theColor);
		break;
		case 1:
			ret = radar2->ML_GVID_SetPPIColor(theColor);
		break;
		default:
		ret = -5;
		break;
	}
	return ret;
}
EXPORT Status ML_GVID_SetPPIScan(char VideoChannel,int theScan){
	printf("ML_GVID_SetPPIScan------\n");

        INIT_CHECK
        INIT_PPI_CHECK
        VC_CHECK
        VR_CHECK(theScan,0,1)

	int ret=0;
	switch(VideoChannel)
	{
		case 0:
			ret = radar1->ML_GVID_SetPPIScan(theScan);
		break;
		case 1:
			ret = radar2->ML_GVID_SetPPIScan(theScan);
		break;
		default:
		ret = -5;
		break;
	}
	return ret;
}
EXPORT Status ML_GVID_SetPPILineColor(char VideoChannel,unsigned int theColor){
	printf("ML_GVID_SetPPILineColor------\n");

        INIT_CHECK
        INIT_PPI_CHECK
        VC_CHECK

	int ret=0;
	switch(VideoChannel)
	{
		case 0:
			ret = radar1->ML_GVID_SetPPILineColor(theColor);
		break;
		case 1:
			ret = radar2->ML_GVID_SetPPILineColor(theColor);
		break;
		default:
		ret = -5;
		break;
	}
	return ret;
}
EXPORT Status ML_GVID_FrostPPI(char VideoChannel,char theFrost){
	printf("ML_GVID_FrostPPI------\n");

        INIT_CHECK
        INIT_PPI_CHECK
        VC_CHECK
        VR_CHECK(theFrost,0,1)

	int ret=0;
	switch(VideoChannel)
	{
		case 0:
			ret = radar1->ML_GVID_FrostPPI(theFrost);
		break;
		case 1:
			ret = radar2->ML_GVID_FrostPPI(theFrost);
		break;
		default:
		ret = -5;
		break;
	}
	return ret;
}
EXPORT Status ML_GVID_SetPPIDist(char VideoChannel,short theDist){
	printf("ML_GVID_SetPPIDist------\n");

        INIT_CHECK
        INIT_PPI_CHECK
        VC_CHECK
        VR_CHECK(theDist,1,300)

	int ret=0;
	switch(VideoChannel)
	{
		case 0:
			ret = radar1->ML_GVID_SetPPIDist(theDist);
		break;
		case 1:
			ret = radar2->ML_GVID_SetPPIDist(theDist);
		break;
		default:
		ret = -5;
		break;
	}
	return ret;
}
EXPORT Status ML_GVID_SetPPISwitch(char VideoChannel,short theSwitch){
	printf("ML_GVID_SetPPISwitch------\n");

        INIT_CHECK
        INIT_PPI_CHECK
        VC_CHECK
        VR_CHECK(theSwitch,0,1)

	int ret=0;
	switch(VideoChannel)
	{
		case 0:
			ret = radar1->ML_GVID_SetPPISwitch(theSwitch);
		break;
		case 1:
			ret = radar2->ML_GVID_SetPPISwitch(theSwitch);
		break;
		default:
		ret = -5;
		break;
	}
	return ret;
}
EXPORT Status ML_GVID_SetPPIGate(char VideoChannel,char theGate){
	printf("ML_GVID_SetPPIGate------\n");

        INIT_CHECK
        INIT_PPI_CHECK
        VC_CHECK
        if(theGate<1 || theGate >8) return -1;

        theGate -= 4;
        theGate *= 25;//分档

	int ret=0;
	switch(VideoChannel)
	{
		case 0:
			ret = radar1->ML_GVID_SetPPIGate(theGate);
		break;
		case 1:
			ret = radar2->ML_GVID_SetPPIGate(theGate);
		break;
		default:
		ret = -5;
		break;
	}
	return ret;
}
EXPORT Status ML_GVID_SetPPITail(char VideoChannel,int theTail){
	printf("ML_GVID_SetPPITail------\n");

        INIT_CHECK
        INIT_PPI_CHECK
        VC_CHECK

        if (theTail < 0 || theTail >65535 ) return -1;

        theTail = (double)theTail/256;

        theTail = 255 - theTail;

	int ret=0;
	switch(VideoChannel)
	{
		case 0:
			ret = radar1->ML_GVID_SetPPITail(theTail);
		break;
		case 1:
			ret = radar2->ML_GVID_SetPPITail(theTail);
		break;
		default:
		ret = -5;
		break;
	}
	return ret;
}
//----------------------------------AR----------------------------------------
EXPORT Status ML_GVID_OpenAr(char VideoChannel,short theX, short theY , short theWidth, short theHeight)
{
	printf("ML_GVID_OpenAr------\n");
        INIT_CHECK
        INIT_PPI_CHECK
        VC_CHECK
        XY_CHECK
        VR_CHECK(theWidth,1,512)
        VR_CHECK(theHeight,1,512)

	int ret=0;
	switch(VideoChannel)
	{
		case 0:
			ret = radar1->ML_GVID_OpenAr(theX,theY,theWidth,theHeight);
		break;
		case 1:
			ret = radar2->ML_GVID_OpenAr(theX,theY,theWidth,theHeight);
		break;
		default:
		ret = -5;
		break;
	}
    if(ret > 0)bInitAR[VideoChannel]  = true;
    return ret;

}
EXPORT Status ML_GVID_CloseAr(char VideoChannel)
 {
    printf("ML_GVID_CloseAr------\n");
    INIT_CHECK
    INIT_AR_CHECK
    VC_CHECK
    int ret=0;
    switch(VideoChannel)
    {
            case 0:
                    ret = radar1->ML_GVID_CloseAr();
            break;
            case 1:
                    ret = radar2->ML_GVID_CloseAr();
            break;
            default:
            ret = -5;
            break;
    }
    if(ret > 0)bInitAR[VideoChannel]  = false;
return ret;
 }
EXPORT Status ML_GVID_SetArXY(char VideoChannel ,short theX ,short theY)
  {
    printf("ML_GVID_SetArXY------\n");
    INIT_CHECK
    //INIT_PPI_CHECK
    INIT_AR_CHECK
    VC_CHECK
    XY_CHECK
    int ret=0;
    switch(VideoChannel)
    {
            case 0:
                    ret = radar1->ML_GVID_SetArXY(theX,theY);
            break;
            case 1:
                    ret = radar2->ML_GVID_SetArXY(theX,theY);
            break;
            default:
            ret = -5;
            break;
    }

return ret;

  }
EXPORT Status ML_GVID_SetArWH(char VideoChannel ,short theWidth, short theHeight)
 {
    printf("ML_GVID_SetArWH------\n");

    INIT_CHECK
    //INIT_PPI_CHECK
    INIT_AR_CHECK
    VC_CHECK
    VR_CHECK(theWidth,1,512)
    VR_CHECK(theHeight,1,512)

    int ret=0;
    switch(VideoChannel)
    {
            case 0:
                    ret = radar1->ML_GVID_SetArWH(theWidth,theHeight);
            break;
            case 1:
                    ret = radar2->ML_GVID_SetArWH(theWidth,theHeight);
            break;
            default:
            ret = -5;
            break;
    }

return ret;


 }
EXPORT Status ML_GVID_SetArGate (char VideoChannel ,char theGate)
{
    printf("ML_GVID_SetArGate------\n");

    INIT_CHECK
    INIT_PPI_CHECK
    INIT_AR_CHECK
    VC_CHECK
    VR_CHECK(theGate,0,7)
    theGate -= 4;
    theGate *= 25;

    int ret=0;
    switch(VideoChannel)
    {
            case 0:
                    ret = radar1->ML_GVID_SetArGate(theGate);
            break;
            case 1:
                    ret = radar2->ML_GVID_SetArGate(theGate);
            break;
            default:
            ret = -5;
            break;
    }

return ret;
}
EXPORT Status ML_GVID_SetArDist (char VideoChannel ,short theDist)
{
    printf("ML_GVID_SetArDist------\n");

    INIT_CHECK
    INIT_PPI_CHECK
    INIT_AR_CHECK
    VC_CHECK
    VR_CHECK(theDist,1,300)

    int ret=0;
    switch(VideoChannel)
    {
            case 0:
                    ret = radar1->ML_GVID_SetArDist(theDist);
            break;
            case 1:
                    ret = radar2->ML_GVID_SetArDist(theDist);
            break;
            default:
            ret = -5;
            break;
    }

return ret;

}
//----------------------------------------------MiniWin------
EXPORT Status ML_GVID_OpenMiniWin(char VideoChannel,short theX, short theY , short theWidth, short theHeight)
{
    printf("ML_GVID_OpenMiniWin------\n");

    INIT_CHECK
    INIT_PPI_CHECK
    VC_CHECK
    XY_CHECK
    VR_CHECK(theWidth,1,1200)
    VR_CHECK(theHeight,1,1200)

    int ret=0;
    switch(VideoChannel)
    {
            case 0:
                    ret = radar1->ML_GVID_OpenMiniWin(theX,theY,theWidth,theHeight);
            break;
            case 1:
                    ret = radar2->ML_GVID_OpenMiniWin(theX,theY,theWidth,theHeight);
            break;
            default:
            ret = -5;
            break;
    }
    if(ret > 0)bInitMiniWin[VideoChannel]  = true;
return ret;
}
EXPORT Status ML_GVID_CloseMiniWin(char VideoChannel)
{
    printf("ML_GVID_CloseMiniWin------\n");

    INIT_CHECK
    //INIT_PPI_CHECK
    INIT_MiniWin_CHECK
    VC_CHECK

    int ret=0;
    switch(VideoChannel)
    {
            case 0:
                    ret = radar1->ML_GVID_CloseMiniWin();
            break;
            case 1:
                    ret = radar2->ML_GVID_CloseMiniWin();
            break;
            default:
            ret = -5;
            break;
    }
    if(ret > 0)bInitMiniWin[VideoChannel]  = false;
return ret;
}
EXPORT Status ML_GVID_SetMiniWinXY(char VideoChannel ,short theX ,short theY)
{
    printf("ML_GVID_SetMiniWinXY------\n");

    INIT_CHECK
    //INIT_PPI_CHECK
    INIT_MiniWin_CHECK
    VC_CHECK
    XY_CHECK

    int ret=0;
    switch(VideoChannel)
    {
            case 0:
                    ret = radar1->ML_GVID_SetMiniWinXY(theX,theY);
            break;
            case 1:
                    ret = radar2->ML_GVID_SetMiniWinXY(theX,theY);
            break;
            default:
            ret = -5;
            break;
    }

return ret;
}
EXPORT Status ML_GVID_SetMiniWinWH(char VideoChannel ,short theWidth, short theHeight)
{
    printf("ML_GVID_SetMiniWinWH------\n");

    INIT_CHECK
    //INIT_PPI_CHECK
    INIT_MiniWin_CHECK
    VC_CHECK
    VR_CHECK(theWidth,1,1200)
    VR_CHECK(theHeight,1,1200)

    int ret=0;
    switch(VideoChannel)
    {
            case 0:
                    ret = radar1->ML_GVID_SetMiniWinWH(theWidth,theHeight);
            break;
            case 1:
                    ret = radar2->ML_GVID_SetMiniWinWH(theWidth,theHeight);
            break;
            default:
            ret = -5;
            break;
    }

return ret;
}
EXPORT Status ML_GVID_SetMiniWinMode(char VideoChannel, short theMode)
{
    printf("ML_GVID_SetMiniWinMode------\n");

    INIT_CHECK
    INIT_PPI_CHECK
    INIT_MiniWin_CHECK
    VC_CHECK
    VR_CHECK(theMode,0,1)

    int ret=0;
    switch(VideoChannel)
    {
            case 0:
                    ret = radar1->ML_GVID_SetMiniWinMode(theMode);
            break;
            case 1:
                    ret = radar2->ML_GVID_SetMiniWinMode(theMode);
            break;
            default:
            ret = -5;
            break;
    }

return ret;

}
EXPORT Status ML_GVID_SetMiniWinColor(char VideoChannel,unsigned int theColor)
{
    printf("ML_GVID_CloseMiniWin------\n");

    INIT_CHECK
    INIT_PPI_CHECK
    INIT_MiniWin_CHECK
    VC_CHECK

    int ret=0;
    switch(VideoChannel)
    {
            case 0:
                    ret = radar1->ML_GVID_SetMiniWinColor(theColor);
            break;
            case 1:
                    ret = radar2->ML_GVID_SetMiniWinColor(theColor);
            break;
            default:
            ret = -5;
            break;
    }

return ret;
}
EXPORT Status ML_GVID_SetMiniWinDistDir(char VideoChannel ,short theDistance ,float theDirection)
{
    printf("ML_GVID_CloseMiniWin------\n");
    INIT_CHECK
    INIT_PPI_CHECK
    INIT_MiniWin_CHECK
    VC_CHECK
    VR_CHECK(theDistance,0,300)
    while(theDirection<0)
    theDirection += 360.0;
    while(theDirection>=360)
    theDirection -= 360.0;

    int ret=0;
    switch(VideoChannel)
    {
            case 0:
                    ret = radar1->ML_GVID_SetMiniWinDistDir(theDistance,theDirection);
            break;
            case 1:
                    ret = radar2->ML_GVID_SetMiniWinDistDir(theDistance,theDirection);
            break;
            default:
            ret = -5;
            break;
    }

return ret;

}
EXPORT Status ML_GVID_SetMiniWinScale(char VideoChannel,int theScale)
{
    printf("ML_GVID_CloseMiniWin------\n");

    INIT_CHECK
    INIT_PPI_CHECK
    INIT_MiniWin_CHECK
    VR_CHECK(theScale,1,8)
    VC_CHECK

    int ret=0;
    switch(VideoChannel)
    {
            case 0:
                    ret = radar1->ML_GVID_SetMiniWinScale(theScale);
            break;
            case 1:
                    ret = radar2->ML_GVID_SetMiniWinScale(theScale);
            break;
            default:
            ret = -5;
            break;
    }

return ret;

}
//-------------------------------tv-------------------------------------------
EXPORT status ML_GVID_OpenTV(char VideoChannel, short theX, short theY, short theWidth, short theHeight){

    INIT_CHECK
    VC_CHECK
    XY_CHECK
    VR_CHECK(theWidth,1,720);
    VR_CHECK(theHeight,1,576);
    OpenTV(VideoChannel,theX,theY,theWidth, theHeight);
    bInitTV[VideoChannel]  = true;

}
EXPORT status ML_GVID_CloseTV(char VideoChannel){

    INIT_CHECK
    INIT_TV_CHECK
    VC_CHECK
    CloseTV( VideoChannel);
    bInitTV[VideoChannel]  = false;
}
EXPORT status ML_GVID_SetTVXY(char VideoChannel, short theX, short theY){
    INIT_CHECK
    INIT_TV_CHECK
    VC_CHECK
    XY_CHECK
    SetTVXY( VideoChannel,  theX,  theY);

}
EXPORT status ML_GVID_SetTVWH(char VideoChannel, short theWidth, short theHeight){
    INIT_CHECK
    INIT_TV_CHECK
    VC_CHECK
    VR_CHECK(theWidth,1,720)
    VR_CHECK(theHeight,1,576)
    SetTVWH( VideoChannel,  theWidth,  theHeight);
}
EXPORT status ML_GVID_FrostTV(char VideoChannel, char theForst){
    INIT_CHECK
    INIT_TV_CHECK
    VC_CHECK
    SetTVFrost( VideoChannel,  theForst);
}
EXPORT status ML_GVID_SetTVBright(char VideoChannel, unsigned char theBright){
    INIT_CHECK
    INIT_TV_CHECK
    VC_CHECK
    SetTVBright( VideoChannel, theBright);
}
EXPORT status ML_GVID_SetTVContrast(char VideoChannel, unsigned char theContrast){
    INIT_CHECK
    INIT_TV_CHECK
    VC_CHECK
    SetTVContrast( VideoChannel,theContrast);
}
EXPORT status ML_GVID_SetTVColor(char VideoChannel, unsigned char theColor){

    INIT_CHECK
    INIT_TV_CHECK
    VC_CHECK
    SetTVColor( VideoChannel,   theColor);
}
//---------------------------------------------------------------------------------------------------------
EXPORT void  ML_GVID_Exit()
{

	if(pBuffer1) {
	pBuffer1->ExitThread();
	delete pBuffer1;
	pBuffer1 = NULL;
	}

	if(radar1) 
	{
	delete radar1;
	radar1 = NULL;

	}

	if(pBuffer2) {
	pBuffer2->ExitThread();
	delete pBuffer2;
	pBuffer2 = NULL;
	}

	if(radar2)
	{	delete radar2;
		radar2 = NULL;
	}

	fb_cleanup();
	if(debugfp)fclose(debugfp);
	printf("ML_GVID_Exit!\n");
}




