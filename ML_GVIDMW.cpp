#include <stdio.h>
#include <stdlib.h>  //exit函数
#include <unistd.h>  //usleep
#include "ML_GVIDMW.h"
#include "Timer.h"
//#include "./Timer/include/timer_manager.h"
#include "./RadarDisplay/RadarDis.h"
#include "DataBuffer.h"

FILE * debugfp =NULL;

CRadarDisplay* radar1 =NULL;
CDataBuffer* pBuffer1 = NULL;
CRadarDisplay* radar2 =NULL;
CDataBuffer* pBuffer2 = NULL;

//-----------------------------暂时放在这里，还需要进一步的调整－－－－－－－－－－－
// void yh1(CTimer * timer, void *data)
//{
//  for(int i =0;i<1;i++)//3
//	{
//		if(radar1)radar1->yh->yh();
//	}
//	
//}
// void yh2(CTimer * timer, void *data)
//{
//  for(int i =0;i<1;i++)//3
//	{
//		
//		if(radar2)radar2->yh->yh();
//	}
//	
//}
void tshTestYh(int a)
{
  for(int i =0;i<2;i++)//3
	{
		
		if(radar1)radar1->yh->yh();
	}

}
	//－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－
 Status ML_GVID_Init()
{
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
//	CTimerManager::instance()->start(20000,0);
//	CTimer a1(10000,yh1,(void *)1,CTimer::TIMER_CIRCLE);
//	CTimer a2(20000,yh2,(void *)2,CTimer::TIMER_CIRCLE);
//	a1.start();
//	a2.start();
	SetTimer(30000,tshTestYh);
	return ret;
}
//----------------------------------PPI---------------------------------------- Status ML_GVID_OpenPPI(char VideoChannel,short theX, short theY , short theWidth, short theHeight)
{
	printf("ML_GVID_OpenPPI------\n");
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
	return ret;
}
 Status ML_GVID_ClosePPI(char VideoChannel)
{
	printf("ML_GVID_ClosePPI------\n");
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
	return ret;

}
 Status ML_GVID_SetPPIXY(char VideoChannel ,short theX ,short theY)
{
	printf("ML_GVID_SetPPIXY------\n");
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
 Status ML_GVID_SetPPIWH(char VideoChannel ,short theWidth, short theHeight){
	printf("ML_GVID_SetPPIWH------\n");
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
 Status ML_GVID_SetPPIMode(char VideoChannel, short theMode){
	printf("ML_GVID_SetPPIMode------\n");
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
 Status ML_GVID_SetPPICenter(char VideoChannel ,short theX ,short theY){
	printf("ML_GVID_SetPPICenter------\n");
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
 Status ML_GVID_SetPPIShipDir(char VideoChannel,float theShipDir){
	printf("ML_GVID_SetPPIShipDir------\n");
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
 Status ML_GVID_SetPPIColor(char VideoChannel,unsigned int theColor){
	printf("ML_GVID_SetPPIColor------\n");
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
 Status ML_GVID_SetPPIScan(char VideoChannel,int theScan){
	printf("ML_GVID_SetPPIScan------\n");
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
 Status ML_GVID_SetPPILineColor(char VideoChannel,unsigned int theColor){
	printf("ML_GVID_SetPPILineColor------\n");
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
 Status ML_GVID_FrostPPI(char VideoChannel,char theFrost){
	printf("ML_GVID_FrostPPI------\n");
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
 Status ML_GVID_SetPPIDist(char VideoChannel,short theDist){
	printf("ML_GVID_SetPPIDist------\n");
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
 Status ML_GVID_SetPPISwitch(char VideoChannel,short theSwitch){
	printf("ML_GVID_SetPPISwitch------\n");
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
 Status ML_GVID_SetPPIGate(char VideoChannel,char theGate){
	printf("ML_GVID_SetPPIGate------\n");
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
 Status ML_GVID_SetPPITail(char VideoChannel,int theTail){
	printf("ML_GVID_SetPPITail------\n");
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
Status ML_GVID_OpenAr(char VideoChannel,short theX, short theY , short theWidth, short theHeight)
{
	printf("ML_GVID_OpenAr------\n");
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

return ret;

}
//---------------------------------------------------------------------------------------------------------
 void  ML_GVID_Exit()
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



