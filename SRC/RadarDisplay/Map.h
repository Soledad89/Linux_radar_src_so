#ifndef _CMAP_H_
#define _CMAP_H_

#include "global.h"
#include "datapool.h"
//把图片加载到某个内存页面
//
//
class CMap
{

public:
	CMap(CDataPool *dp);
	virtual ~CMap();
private:
	CDataPool *dp;
	fb_surface* pmap;
	bool IsDrawFlag; //是否加载地图信息
//记录位置信息
	public:
		int PPI_XSTART;
		int PPI_YSTART;
		int PPI_WIDTH;
		int PPI_HEIGHT;
private:
	s_layer_unit ** layerPPI;//frame buffer layer

public:
	//加载地图注意是ppm格式的图片
	bool LoadPPM(char *filename);
	bool ReloadPPM(char *filename);
	//把地图信息加载到自定义数据结构layerPPI的第0层,只需要加载一次就可以了
	bool DrawPPMtolayerPPI0();
	//停止地图显示
	bool StopMapDispaly();
	//恢复地图显示
	bool ResumeMapDispaly();

};
#endif//_CMAP_H_
