#ifndef _AR_H_
#define _AR_H_

#include "global.h"
#include "datapool.h"


class CAR
{
public:
	CAR(CDataPool * m_dp);
public:
	~CAR(void);

public:
	void ARDraw(s_ppi_video_for_disp*);
	void ARDraw(unsigned char * dat,int len);
	void ARDrawEnlarge(unsigned char * dat,int len);

	void updateRegion(void);
	void clear();
	void setARDispaly(bool b){isARDispaly = b;}
	bool IsARDisplay(){return isARDispaly;}
	void setARGate(int g);
	int getARGate(){return gate;}
	void setARDistance(int d);
	int getARDistance(){return distance;}

	void updateSurface();
	void clearSurface(int y,int r1,int r2);
	void drawSurface(int y,int r1,int r2);
	void updateRegionLine();
	void clearRegionLine();
	void setARColor(unsigned int c);
	void setForeColor(unsigned int c);
	void setMeshBright(unsigned char b);
	void setARRange(int r);
	void setEnlargeRegion(int st,int ed);//unit as range--km

	void updateFbp(void);
public:
	void UpdateARDispaly();
	void  DrawARFrame(int color);
private:
	inline bool isValid( int &x, int &y );
	inline void setFirstPixel(int x, int y, DISPMEMTYPE color);
	void drawVideo( DISPMEMTYPE color);
	void drawEnlargeVideo( DISPMEMTYPE color);
	void drawLineP2P(int x1,int y1,int x2,int y2,DISPMEMTYPE color);


private:
	int AR_XSTART;
	int AR_YSTART;
	int AR_WIDTH;
	int AR_HEIGHT;

	int AR_YSTART2;

public:
	
	CDataPool *dp;
	fb_surface* fbpAR;//pointer to framebuffer mem
	s_layer_unit ** layerAR;//frame buffer layer
//------------------------------------------------------------------------------
	DISPMEMTYPE ARcolor;

	bool isARDispaly;
	static const int oncevn=10;// once every video number, 
	int videoCount;// count for video passed

	int gate;
	int distance;// 



	int video_data[SAMPLE_NUM];// 
	int *video_enlarge;// 
	VIDEODATATYPE *datbuf;// 
	int datbufLen;

	//for surface
	CLayerBase *layerS2;
	CLayerBase *layerR2;

	CPainter *painter2;
	CPainter *eraser2;

	// for enlarge line
	CLayerBase *layerS3;
	CLayerBase *layerR3;

	CPainter *painter3;
	CPainter *eraser3;

	bool bSurface;
	bool bRegionLine;
	bool hasLine1;
	bool hasLine2;
	int range;// unit km
	int stR,edR;// unit km

	DISPMEMTYPE foreColor;
	DISPMEMTYPE meshColor;
	int markNum;

};#endif//_AR_H_
