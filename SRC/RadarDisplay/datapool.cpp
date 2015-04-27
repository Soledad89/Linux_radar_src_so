/*
头文件的位置如果不是必要的，最好放在cpp文件中
*/

#include   <stdlib.h>  //exit函数 
#include   <string.h>  //memset函数
#include "datapool.h"
#include "debug.h"
#include "ConvertColor.h"


CDataPool::CDataPool(fb_surface* fbplinearPPI,fb_surface* fbplinearAR, fb_surface* fbplinearMINI )
{
    //从文件中读取地址表
    if(initDataTable()!=0)
    {
        printf("load addr table error\n");
        exit(0);//退出程序
    }
    //init frame buffer
    if(SetFbDev(fbplinearPPI,fbplinearAR,fbplinearMINI)!=0)
    {
        printf("init frame buffer error\n");
        exit(0);//退出程序
    }
    //init multi display layer
    if(initFbLayer()!=0)
    {
        printf("init frame buffer layer error\n");
        exit(0);//退出程序
    }
	PPI_XSTART = -1;
	PPI_YSTART = -1;
	PPI_WIDTH  = -1;
	PPI_HEIGHT = -1;
	fbpPPI=NULL; 

	AR_XSTART = -1;
	AR_YSTART = -1;
	AR_WIDTH  = -1;
	AR_HEIGHT = -1;
	fbpAR=NULL; 

	WIN_XSTART = -1;
	WIN_YSTART = -1;
	WIN_WIDTH  = -1;
	WIN_HEIGHT = -1;
	fbplinearMINI=NULL;

    SetPPIDisplayColor(255,0,0);
    SetWinDisplayColor(255,0,0);
    painterPool = new CPainterPool(this);
    TRACE("CDataPool::CDataPool()");
}
CDataPool::~CDataPool()
{
    freeDataTable();
    freeFbDev();
    freeFbLayer();
	delete painterPool;
    TRACE("CDataPool::~CDataPool()");
}
int CDataPool::initDataTable()
{
    //为近区地址表和远区地址表分配空间-----------------------
    //近区
    addr_near_x = new int*[ADDR_NEAR_AZI_LEN];
    MEMALLOCCHECK(addr_near_x)
    addr_near_y = new int*[ADDR_NEAR_AZI_LEN];
    MEMALLOCCHECK(addr_near_y)
    for( int i=0; i<ADDR_NEAR_AZI_LEN; i++ )
    {
        addr_near_x[i] = new int[ADDR_NEAR_SMP_LEN];
        MEMALLOCCHECK(addr_near_x[i])
        addr_near_y[i] = new int[ADDR_NEAR_SMP_LEN];
        MEMALLOCCHECK(addr_near_y[i])
    }

    //远区
    addr_far_x = new int*[ADDR_FAR_AZI_LEN];
    MEMALLOCCHECK(addr_far_x)
    addr_far_y = new int*[ADDR_FAR_AZI_LEN];
    MEMALLOCCHECK(addr_far_y)
    for( int i=0; i<ADDR_FAR_AZI_LEN; i++ )
    {
        addr_far_x[i] = new int[ADDR_FAR_SMP_LEN];
        MEMALLOCCHECK(addr_far_x[i])
        addr_far_y[i] = new int[ADDR_FAR_SMP_LEN];
        MEMALLOCCHECK(addr_far_y[i])
    }

    //余辉随机数表
    randomx = new int[RND_LEN];
    MEMALLOCCHECK(randomx)
    randomy = new int[RND_LEN];
    MEMALLOCCHECK(randomy)

    PPIColorTable = new DISPMEMTYPE[256];
    MEMALLOCCHECK(PPIColorTable)
    
    WinColorTable = new DISPMEMTYPE[256];
    MEMALLOCCHECK(WinColorTable)

    //从文件中读取----------------------------------------
    FILE *fp;
    int i;
    if( (fp=fopen("./dat/sysdat","rb"))==NULL )
    {
        printf("read ./dat/sysdat error!\n");
        return -1;
    }
    else
    {
        fseek(fp,datPos[6],SEEK_SET);
        for(  i=0; i<ADDR_NEAR_AZI_LEN; i++ )
            fread(addr_near_x[i],sizeof(int),ADDR_NEAR_SMP_LEN,fp);

        fseek(fp,datPos[7],SEEK_SET);
        for(  i=0; i<ADDR_NEAR_AZI_LEN; i++ )
            fread(addr_near_y[i],sizeof(int),ADDR_NEAR_SMP_LEN,fp);

        fseek(fp,datPos[8],SEEK_SET);
        for(  i=0; i<ADDR_FAR_AZI_LEN; i++ )
            fread(addr_far_x[i],sizeof(int),ADDR_FAR_SMP_LEN,fp);

        fseek(fp,datPos[9],SEEK_SET);
        for(  i=0; i<ADDR_FAR_AZI_LEN; i++ )
            fread(addr_far_y[i],sizeof(int),ADDR_FAR_SMP_LEN,fp);

        fseek(fp,datPos[4],SEEK_SET);
        fread(randomx,sizeof(int),RND_LEN,fp);

        fseek(fp,datPos[5],SEEK_SET);
        fread(randomy,sizeof(int),RND_LEN,fp);

        fseek(fp,datPos[0],SEEK_SET);
        fread(PPIColorTable,sizeof(DISPMEMTYPE),256,fp);

        fclose(fp);
    }
    TRACE("--load data table done");
    return 0;
}

void CDataPool::freeDataTable()
{
    for( int i=0; i<ADDR_NEAR_AZI_LEN; i++ )
    {
        delete[] addr_near_x[i];
        delete[] addr_near_y[i];
    }
    delete[] addr_near_x;
    delete[] addr_near_y;

    for( int i=0; i<ADDR_FAR_AZI_LEN; i++ )
    {
        delete[] addr_far_x[i];
        delete[] addr_far_y[i];
    }
    delete[] addr_far_x;
    delete[] addr_far_y;

    if(randomx)delete[] randomx;
    if(randomy)delete[] randomy;

    if(PPIColorTable)delete[] PPIColorTable;
    if(WinColorTable)delete[] WinColorTable;
    TRACE("--data table fread");
}  
 //从一维线性地址中分离出需要作图的区域,转换为二维地址空间
 //要注意这样映射后,fbp[x][y]在屏幕上的位置水平为y,竖直为x
int CDataPool::SetFbDev( fb_surface *fbplinearPPI,fb_surface*fbplinearAR,fb_surface *fbplinearMINI)
{
if(fbplinearPPI){
        PPI_XSTART =0;//fbplinearPPI->x;
        PPI_YSTART = 0;//fbplinearPPI->y;
	PPI_WIDTH  = fbplinearPPI->width;
	PPI_HEIGHT = fbplinearPPI->height;
	fbpPPI=fbplinearPPI; 
}
	
if(fbplinearAR){
        AR_XSTART = 0;//fbplinearAR->x;
        AR_YSTART = 0;//fbplinearAR->y;
	AR_WIDTH  = fbplinearAR->width;
	AR_HEIGHT = fbplinearAR->height;
	fbpAR=fbplinearAR; }
if(fbplinearMINI){
        WIN_XSTART = 0;//fbplinearMINI->x;
        WIN_YSTART = 0;//fbplinearMINI->y;
	WIN_WIDTH  = fbplinearMINI->width;
	WIN_HEIGHT = fbplinearMINI->height;
	fbpMINI=fbplinearMINI;}
    return 0;
}

void CDataPool::freeFbDev()
{


    TRACE("--fbdev close");
}
//
//
int CDataPool::initFbLayer()
{
	//--------------------------------------------------------------
	layerPPI = new s_layer_unit*[MAX_PPI_HEIGHT];
	MEMALLOCCHECK(layerPPI)

	    for( int i=0; i<MAX_PPI_HEIGHT; i++ )
	    {
		layerPPI[i] = new s_layer_unit [MAX_PPI_WIDTH];
		MEMALLOCCHECK(layerPPI[i])
		memset(layerPPI[i],0,MAX_PPI_WIDTH*sizeof(s_layer_unit));
	    }
	//--------------------------------------------------------------
	layerAR = new s_layer_unit*[MAX_AR_HEIGHT];
	MEMALLOCCHECK(layerAR)

	for( int i=0; i<MAX_AR_HEIGHT; i++ )
	{
		layerAR[i] = new s_layer_unit [MAX_AR_WIDTH ];
		MEMALLOCCHECK(layerAR[i])
			memset(layerAR[i],0,MAX_AR_WIDTH *sizeof(s_layer_unit));
	}

	//--------------------------------------------------------------
	layerMINI = new s_layer_unit*[MAX_MINIWIN_HEIGHT];
	MEMALLOCCHECK(layerMINI)

	for( int i=0; i<MAX_MINIWIN_HEIGHT; i++ )
	{
		layerMINI[i] = new s_layer_unit [MAX_MINIWIN_WIDTH];
		MEMALLOCCHECK(layerMINI[i])
			memset(layerMINI[i],0,MAX_MINIWIN_WIDTH*sizeof(s_layer_unit));
	}
	//--------------------------------------------------------------
    	TRACE("--fb layer inited");
    	return 0;
}

void CDataPool::freeFbLayer()
{
	if(layerPPI)
	{
		for( int i=0; i<MAX_PPI_HEIGHT; i++ )
		{
		   if(layerPPI[i]) delete[] layerPPI[i];
		}
		delete[] layerPPI;
	}

	//-------------------------------------
	if(layerAR)
	{
		for( int i=0; i<MAX_AR_HEIGHT; i++ )
		{
			if(layerAR[i])delete[] layerAR[i];
		}
		delete[] layerAR;
	}


	//-------------------------------------
	if(layerMINI)
	{
		for( int i=0; i<MAX_MINIWIN_HEIGHT; i++ )
		{
			if(layerMINI[i])delete[] layerMINI[i];
		}
		delete[] layerMINI;
	}


    TRACE("--fb layer freed");
}
/**/
void CDataPool::ClearLayer(int t)//分类清屏，不清遮蔽区
{
	int i;

	switch(t){
		case 0: // 默认为重置全部
			for(i=0;i<MAX_PPI_HEIGHT;++i)
			{
				memset( layerPPI[i],0,MAX_PPI_WIDTH*sizeof(s_layer_unit));
			}
			for(i=0;i<MAX_AR_HEIGHT;++i)
			{
				memset( layerAR[i],0,MAX_AR_WIDTH*sizeof(s_layer_unit));
			}
			for(i=0;i<MAX_MINIWIN_HEIGHT;++i)
			{
				memset( layerMINI[i],0,MAX_MINIWIN_WIDTH*sizeof(s_layer_unit));
			}
			break;

		case 1: // 重置开窗区
			for(i=0;i<MAX_MINIWIN_HEIGHT;++i)
			{
				memset( layerMINI[i],0,MAX_MINIWIN_WIDTH*sizeof(s_layer_unit));
			}
			break;

		case 2://重置AR区
			for(i=0;i<MAX_AR_HEIGHT;++i)
			{
				memset( layerAR[i],0,MAX_AR_WIDTH*sizeof(s_layer_unit));
			}
			break;

		case 3://重置PPI区
			for(i=0;i<MAX_PPI_HEIGHT;++i)
			{
				memset( layerPPI[i],0,MAX_PPI_WIDTH*sizeof(s_layer_unit));
			}
			break;

		default:

			break;
	}


}
/*下面两个函数是建立PPI和WIN显示的颜色表*/
void CDataPool::SetPPIDisplayColor(int r,int g,int b)
{
	float h,s,l;
	rgb_to_hsv(r,g,b,&h,&s,&l);

	for(int i=0;i<256;i++)
	{
		l = i/255.0;
		hsv_to_rgb(h,s,l,&r,&g,&b);
		PPIColorTable[i] = REFC(r,g,b);
	}

}
void CDataPool::SetWinDisplayColor(int r,int g,int b)
{
	float h,s,l;
	rgb_to_hsv(r,g,b,&h,&s,&l);

	for(int i=0;i<256;i++)
	{
		l = i/255.0;
		hsv_to_rgb(h,s,l,&r,&g,&b);
		WinColorTable[i] = REFC(r,g,b);
	}

}

