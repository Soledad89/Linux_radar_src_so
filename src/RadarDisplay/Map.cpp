
#include   <stdlib.h>  //exit函数 
#include <stdio.h>
#include "Map.h"
//#include "../FrameBuffer/FrameBuffer.h"

CMap::CMap(CDataPool *dp )
{
	this->dp = dp;
	this->layerPPI = dp->layerPPI;
	pmap = NULL;
	IsDrawFlag = false; //是否显示地图信息

}
CMap::~CMap()
{
	if(pmap)
	{
		fb_destroy_surface(pmap);
		pmap = NULL;
	}

}
//加载图片到指定的缓存区
bool CMap::LoadPPM(char *filename)
{
	
	this->pmap =  fb_load_ppm(filename);
	if(pmap) 
		return true;
	else 
		return false;

}
bool CMap::ReloadPPM(char *filename)
{
	if(pmap)
	{
		fb_destroy_surface(pmap);
		pmap = NULL;
	}
	pmap =  fb_load_ppm(filename);
	if(pmap) 
		return true;
	else 
		return false;

}
//把地图信息加载到自定义数据结构layerPPI的第0层,只需要加载一次就可以了
bool CMap::DrawPPMtolayerPPI0()
{
	int i=0,j=0;
	int jmin = MIN(MAX_PPI_HEIGHT,pmap->height);
	int imin = MIN(MAX_PPI_WIDTH,pmap->width);
        //printf("x = %d,y =%d!\n",imin,jmin);
	if(pmap)
	{	this->IsDrawFlag = true;		
			for(j=0;j<jmin;j++)//this->dp->PPI_HEIGHT
				{
					i= 0;
				for(i=0;i<imin;i++)//this->dp->PPI_WIDTH
				{	
						
					layerPPI[j][i].bmap = true; //表明该点有地图信息
//					//然后把图片加载到指定的位置，注意这里起始位置都是0,0
	 				fb_getpixel_int( i,  j,&layerPPI[j][i].nmap,pmap);
				}
			}
	return true;
	}
	else
	return false;
}
bool CMap::StopMapDispaly()
{
   	int x=0,y=0;	
	int jmin = MIN(MAX_PPI_HEIGHT,pmap->height);
	int imin = MIN(MAX_PPI_WIDTH,pmap->width);
	if(pmap&&IsDrawFlag)
	{
			for(y=0;y<jmin;y++)
				for(x=0;x<imin ;x++)						
					layerPPI[y][x].bmap = false;
	
		IsDrawFlag = false;		
	return true;
	}
	else
	return false;

}
bool CMap::ResumeMapDispaly()
{

   	int x=0,y=0;	
	int jmin = MIN(MAX_PPI_HEIGHT,pmap->height);
	int imin = MIN(MAX_PPI_WIDTH,pmap->width);
	if(pmap&&IsDrawFlag)
	{
			for(y=0;y<jmin;y++)
				for(x=0;x<imin ;x++)						
					layerPPI[y][x].bmap = true;
	
		IsDrawFlag = true;		
	return true;
	}
	else
	return false;

}




