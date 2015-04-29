#include <stdio.h>
#include <stdlib.h>
#include "ML_GVIDMW.h"
#include <string.h>
#include <unistd.h>

int main()
{
	int  ret = 0;
	ret = ML_GVID_Init();

        if(ret < 0) 
	{
		printf("error in ML_GVID_Init!\n");
		return 1;
	}

 	ret = ML_GVID_OpenPPI(0, 0, 0 , 1024, 1024);
        if(ret < 0) 
	{
		printf("error in ML_GVID_OpenPPI!\n");
		return 1;
	}
	
	ret = ML_GVID_OpenAr(0,1024,300,200,200);
        if(ret < 0) 
	{
		printf("error in ML_GVID_OpenAr!\n");
		return 1;
	}

// Status ML_GVID_SetPPIXY(char VideoChannel ,short theX ,short theY);
// Status ML_GVID_SetPPIWH(char VideoChannel ,short theWidth, short theHeight);
// Status ML_GVID_SetPPIMode(char VideoChannel, short theMode);
// Status ML_GVID_SetPPICenter(char VideoChannel ,short theX ,short theY);
// Status ML_GVID_SetPPIShipDir(char VideoChannel,float theShipDir);
// Status ML_GVID_SetPPIColor(char VideoChannel,unsigned int theColor);
// Status ML_GVID_SetPPIScan(char VideoChannel,int theScan);
// Status ML_GVID_SetPPILineColor(char VideoChannel,unsigned int theColor);
// Status ML_GVID_FrostPPI(char VideoChannel,char theFrost);
// Status ML_GVID_SetPPIDist(char VideoChannel,short theDist);
// Status ML_GVID_SetPPISwitch(char VideoChannel,short theSwitch);
// Status ML_GVID_SetPPIGate(char VideoChannel,char theGate);
// Status ML_GVID_SetPPITail(char VideoChannel,int theTail);

// Status ML_GVID_ClosePPI(char VideoChannel);

// 	ret =ML_GVID_OpenPPI(1,1600, 0 , 1024, 1024);
//        if(ret < 0) 
//	{
//		printf("error in ML_GVID_OpenPPI!\n");
//		return 1;
//	}
//	
//	ret = ML_GVID_OpenAr(1,1600+1024,300,200,200);
//        if(ret < 0) 
//	{
//		printf("error in ML_GVID_OpenAr!\n");
//		return 1;
//	}


    char s[50];

    while(fgets(s,50,stdin)!=NULL)
    {
        if(strstr(s,"bye"))
        {
            break;
        }
    }
    
    usleep(8000);
	ret = ML_GVID_SetPPIXY(0,500,500);
        if(ret < 0) 
	{
		printf("error in ML_GVID_SetPPIXY!\n");
		return 1;
	}

//	ret = ML_GVID_SetPPIWH(0,500,500);
//        if(ret < 0) 
//	{
//		printf("error in ML_GVID_SetPPIWH!\n");
//		return 1;
//	}

//	ret = ML_GVID_ClosePPI(0);
//        if(ret < 0) 
//	{
//		printf("error in ML_GVID_ClosePPI!\n");
//		return 1;
//	}

	ML_GVID_Exit();
	return 0;
}
