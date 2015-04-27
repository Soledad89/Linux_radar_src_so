
#include <string.h>//memset memcpy
#include <stdlib.h>//abs
#include "AR.h"
#include "debug.h"
#include "ConvertColor.h"

CAR::CAR(CDataPool * m_dp)
{
	dp = m_dp;	
	fbpAR = dp->fbpAR;
	layerAR = dp->layerAR;
	updateRegion();



	distance = AR_WIDTH*2;
	gate = 0; //-100;
	videoCount=0;

	isARDispaly=false;
	bSurface=false;
	bRegionLine=false;
	hasLine1=false;
	hasLine2=false;

	ARcolor=MFB_YELLOW;

	foreColor=MFB_GREEN;
	meshColor=MFB_GREEN;
	markNum=5;

	video_enlarge = new int[SAMPLE_NUM];
	memset(video_enlarge,0,SAMPLE_NUM*sizeof(int));

	datbuf = new VIDEODATATYPE[SAMPLE_NUM];

	clear();

	layerS2=new CLayerBase(fbpAR,layerAR,2,true);
	layerR2=new CLayerBase(fbpAR,layerAR,2,false);

	painter2=new CPainter(dp->painterPool,layerS2);
	eraser2=new CPainter(dp->painterPool,layerR2);

	layerS3=new CLayerBase(fbpAR,layerAR,3,true);
	layerR3=new CLayerBase(fbpAR,layerAR,3,false);

	painter3=new CPainter(dp->painterPool,layerS3);
	eraser3=new CPainter(dp->painterPool,layerR3);

	painter3->setColor(MFB_RED);

}

CAR::~CAR(void)
{
	delete video_enlarge;
	delete datbuf;
}
void CAR::UpdateARDispaly()
{
	updateFbp();
	updateRegion();
	clear();
	//updateSurface();
	this->distance = this->AR_WIDTH*2;
	

}
void CAR::setARGate(int g)
{
	clear();
	gate = g;
}

void CAR::setARDistance(int d)
{
	clear();
	distance = d;
}

bool CAR::isValid( int &x, int &y )
{
	return ( (x>=AR_XSTART) && (x<AR_XSTART+AR_WIDTH) && (y>=AR_YSTART) && (y<AR_YSTART+AR_HEIGHT) );
}

void CAR::setFirstPixel(int x, int y, DISPMEMTYPE color)
{

	if(isValid(x,y))
	{
		if(layerAR[y][x].curlayer<2)
		{
			
			//fbpAR[y][x]=color;
			fb_putpixel_int( x,  y, color, fbpAR);
			
		}
	

	}
}

void CAR::ARDraw(s_ppi_video_for_disp* dispdat)
{
	
	int maxd;
	int dotnum;
	VIDEODATATYPE * p;

	if(!isARDispaly)return;
	if(hasLine1)drawVideo(MFB_CLEAR); 

	videoCount += 1;
	if(videoCount<oncevn)return;// 视频数目不够，直接返回

	videoCount %= oncevn;
	int comnum = distance/AR_WIDTH+1;  //计算合并点


	if(comnum*AR_WIDTH>SAMPLE_NUM)comnum = SAMPLE_NUM/AR_WIDTH;

	dotnum = AR_WIDTH;
	p = dispdat->videodata;

	for(int i=0;i<dotnum;i++)
	{
		maxd = 0;
		for(int j=0;j<comnum;j++)
		{
			maxd = *p>maxd ? *p : maxd;
			p++;
		}
		video_data[i]= (maxd+gate)<0 ? 0 : maxd+gate;
	}
	drawVideo(ARcolor);
	DrawARFrame(0xff0000);
	hasLine1 = true;
}
void CAR::DrawARFrame(int color)
{
	int r= (color >> 16) & 0xFF;
	int g= (color >> 8) & 0xFF;
	int b= color & 0xFF;

 	fb_line(AR_XSTART,AR_YSTART, AR_XSTART+AR_WIDTH-1, AR_YSTART,  r,  g,  b, fbpAR);
	fb_line(AR_XSTART,AR_YSTART+AR_HEIGHT-1, AR_XSTART+AR_WIDTH-1, AR_YSTART+AR_HEIGHT-1,  r,  g,  b, fbpAR);

	fb_line(AR_XSTART,AR_YSTART, AR_XSTART, AR_YSTART+AR_HEIGHT-1,  r,  g,  b, fbpAR);	
	fb_line(AR_XSTART+AR_WIDTH-1,AR_YSTART, AR_XSTART+AR_WIDTH-1, AR_YSTART+AR_HEIGHT-1,  r,  g,  b, fbpAR);
	
}
void CAR::ARDraw(unsigned char * dat,int len)
{

	int maxd;
	int dotnum,comnum;
	VIDEODATATYPE * p;


	if(!isARDispaly)return; 
	if(len>1200)return;

	if(hasLine1)drawVideo(MFB_CLEAR);


	comnum = distance/AR_WIDTH +1;
	if(comnum*AR_WIDTH>len)comnum = len/AR_WIDTH;

	dotnum = AR_WIDTH;
	p = dat;


	for(int i=0;i<dotnum;i++)
	{
		maxd = 0;
		for(int j=0;j<comnum;j++)
		{
			maxd = *p>maxd ? *p : maxd;
			p++;
		}
		video_data[i]= (maxd+gate)<0 ? 0 : maxd+gate;
	}


	drawVideo(ARcolor); 
	hasLine1 = true;

	ARDrawEnlarge(dat,len);

	memcpy(datbuf,dat,sizeof(VIDEODATATYPE)*len);
	datbufLen=len;

}

void CAR::ARDrawEnlarge(unsigned char * dat,int len)
{
	int st,ed;
	int n;
	int x1;
	unsigned char *p;

	//static int step=0;

	if(!isARDispaly)return; //¿ØÖÆARÊÇ·ñÏÔÊŸ

	if(hasLine2)drawEnlargeVideo(MFB_CLEAR);//²Á³ýÏß

	st=stR*len/range;
	ed=edR*len/range;
	n=ed-st+1;

	float f;
	p=dat+st;
	for(int i=0;i<AR_WIDTH;i++)
	{
		f=((float)i)*n/((float)AR_WIDTH);
		x1=floorf(f);
		f-=x1;
		if(x1+1>=len)break;
		video_enlarge[i]=p[x1]+(p[x1+1]-p[x1])*f;
	}

	drawEnlargeVideo(ARcolor);//»­Ïß
	hasLine2 = true;

}

void CAR::drawVideo( DISPMEMTYPE color)
{
	int i,j;
	int n,n1;
	int x,y;

	for(i=0;i<AR_WIDTH;i++)
	{
		if(i==0)
		{
			n = video_data[i]>video_data[i+1]? video_data[i]-video_data[i+1]+1 : 1 ;
		}
		else if(i==AR_WIDTH-1)
		{
			n = video_data[i]>video_data[i-1]? video_data[i]-video_data[i-1]+1 : 1 ;
		}
		else
		{
			n = video_data[i]>video_data[i+1]? video_data[i]-video_data[i+1]+1 : 1 ;
			n1 = video_data[i]>video_data[i-1]? video_data[i]-video_data[i-1]+1 : 1 ;
			n = n>n1? n : n1;
		}

		for(j=0;j<n;j++)
		{
			y = AR_YSTART2+AR_HEIGHT-(video_data[i]-j);
			x = i+AR_XSTART;
			
			setFirstPixel(x,y,color);
		}
	}

}


void CAR::drawEnlargeVideo( DISPMEMTYPE color)
{

	int i,j;
	int n,n1;
	int x,y;

	for(i=0;i<AR_WIDTH;i++)
	{
		if(i==0)
		{
			n = video_enlarge[i]>video_enlarge[i+1]? video_enlarge[i]-video_enlarge[i+1]+1 : 1 ;
		}
		else if(i==AR_WIDTH-1)
		{
			n = video_enlarge[i]>video_enlarge[i-1]? video_enlarge[i]-video_enlarge[i-1]+1 : 1 ;
		}
		else
		{
			n = video_enlarge[i]>video_enlarge[i+1]? video_enlarge[i]-video_enlarge[i+1]+1 : 1 ;
			n1 = video_enlarge[i]>video_enlarge[i-1]? video_enlarge[i]-video_enlarge[i-1]+1 : 1 ;
			n = n>n1? n : n1;
		}

		for(j=0;j<n;j++)
		{
			y = AR_YSTART2+AR_HEIGHT-(video_enlarge[i]-j);
			x = i+AR_XSTART;
			
			setFirstPixel(x,y,color);
		}
	}

}

void CAR::drawLineP2P(int x1,int y1,int x2,int y2,DISPMEMTYPE color)
{
	int i;

	if(x1==x2)
	{
		
		if(y1<=y2)
		{
			for(i=y1;i<=y2;i++)
				setFirstPixel(x1,i,color);
		}
		else
		{
			for(i=y2;i<=y1;i++)
				setFirstPixel(x1,i,color);
		}

		return;
	}

	
	if(y1==y2)
	{
		if(x1<=x2)
		{
			for(i=x1;i<=x2;i++)
				setFirstPixel(i,y1,color);
		}
		else
		{
			for(i=x2;i<=x1;i++)
				setFirstPixel(i,y1,color);
		}

		return;
	}

	
	float m=(y2-y1)*1.0/(x2-x1);
	float fTemp;

	if(abs(m)<=1)
	{
		if(x1<x2)
		{
			fTemp=y1-m;
			for(i=x1;i<=x2;i++)
				setFirstPixel(i,fTemp+=m,color);
		}
		else
		{
			fTemp=y2-m;
			for(i=x2;i<=x1;i++)
				setFirstPixel(i,fTemp+=m,color);
		}
		return;
	}

	if(y1<y2)
	{
		fTemp=x1-1/m;
		for(i=y1;i<=y2;i++)
			setFirstPixel(fTemp+=1/m,i,color);
	}
	else
	{
		fTemp=x2-1/m;
		for(i=y2;i<=y1;i++)
			setFirstPixel(fTemp+=1/m,i,color);
	}
}

void CAR::updateRegion(void)
{

	clear();
	AR_XSTART=dp->AR_XSTART;
	AR_YSTART=dp->AR_YSTART;
	AR_WIDTH=dp->AR_WIDTH;
	AR_HEIGHT=dp->AR_HEIGHT;

	AR_YSTART2=AR_YSTART+AR_HEIGHT/2;

}


void CAR::clear()
{
	for(int i=AR_XSTART;i<AR_WIDTH+AR_XSTART;i++)
		for(int j=AR_YSTART;j<AR_HEIGHT+AR_YSTART;j++)
			setFirstPixel(i,j,MFB_CLEAR);

	if(bSurface)
	{
		clearSurface(AR_YSTART,0,range);
		clearSurface(AR_YSTART2,stR,edR);
	}

}

void CAR::updateFbp(void) 
{
	fbpAR=dp->fbpAR;
	layerS2->setFbp(fbpAR);
	layerR2->setFbp(fbpAR);
}


void CAR::updateSurface()
{

	if(bSurface)clearSurface(AR_YSTART,0,range);
	drawSurface(AR_YSTART,0,range);

	if(bSurface)clearSurface(AR_YSTART2,stR,edR);
	drawSurface(AR_YSTART2,stR,edR);

	updateRegionLine();
}

void CAR::clearSurface(int y,int r1,int r2)
{

	int i;
	int n,num,step;

	//eraser2->setColor(meshColor);
	for(i=32;i<256;i+=32)
	{
		eraser2->drawLine(AR_XSTART,y+i,1024,AR_WIDTH);
	}

	n=AR_WIDTH/markNum;

	for(i=0;i<AR_WIDTH;i+=n)
	{
		eraser2->drawLine(AR_XSTART+i,y,2048,AR_HEIGHT);
	}

	//eraser2->setColor(foreColor);
	eraser2->drawNoFillRect(AR_XSTART,y,AR_XSTART+AR_WIDTH-1,y+AR_HEIGHT-1);

	step=(r2-r1)/markNum;
	num=r1;
	for(i=0;i<=AR_WIDTH;i+=n)
	{
		eraser2->drawNum(AR_XSTART+i,y+AR_HEIGHT+4,num);
		num+=step;
	}

	bSurface=false;

}
void CAR::drawSurface(int y,int r1,int r2)
{

	int i;
	int n,num,step;

	painter2->setColor(meshColor);
	for(i=32;i<256;i+=32)
	{
		painter2->drawLine(AR_XSTART,y+i,1024,AR_WIDTH);
	}

	n=AR_WIDTH/markNum;

	for(i=0;i<AR_WIDTH;i+=n)
	{
		painter2->drawLine(AR_XSTART+i,y,2048,AR_HEIGHT);
	}

	painter2->setColor(foreColor);
	painter2->drawNoFillRect(AR_XSTART,y,AR_XSTART+AR_WIDTH-1,y+AR_HEIGHT-1);

	step=(r2-r1)/markNum;
	num=r1;
	for(i=0;i<=AR_WIDTH;i+=n)
	{
		painter2->drawNum(AR_XSTART+i,y+AR_HEIGHT+4,num);
		num+=step;
	}

	bSurface=true;

}

void CAR::updateRegionLine()
{
	int x1,x2;

	if(bRegionLine)clearRegionLine();

	x1=stR*600/range;
	x2=edR*600/range;
	painter3->drawLine(x1+AR_XSTART,AR_YSTART+1,2048,AR_HEIGHT-1);
	painter3->drawLine(x2+AR_XSTART,AR_YSTART+1,2048,AR_HEIGHT-1);

	bRegionLine=true;

}
void CAR::clearRegionLine()
{
	int x1,x2;

	x1=stR*600/range;
	x2=edR*600/range;
	eraser3->drawLine(x1+AR_XSTART,AR_YSTART+1,2048,AR_HEIGHT-1);
	eraser3->drawLine(x2+AR_XSTART,AR_YSTART+1,2048,AR_HEIGHT-1);

	bRegionLine=false;
}

void CAR::setARColor(unsigned int c)
{
	ARcolor=csectocmem(c);
	drawVideo(MFB_CLEAR);
	drawVideo(ARcolor);
}
void CAR::setForeColor(unsigned int c)
{
	foreColor=csectocmem(c);
	setMeshBright(getBright(meshColor));
}
void CAR::setMeshBright(unsigned char b)
{
	meshColor=changeBright(cmemtocsec(foreColor),b);
	updateSurface();
}

void CAR::setARRange(int r)
{
	clearSurface(AR_YSTART,0,range);
	if(bRegionLine)clearRegionLine();
	range = r;
	drawSurface(AR_YSTART,0,range);

	updateRegionLine();
	if(hasLine1 && hasLine2)ARDrawEnlarge(datbuf,datbufLen);
}

void CAR::setEnlargeRegion(int st,int ed)//unit as range--km
{
	
	if(bSurface)clearSurface(AR_YSTART2,stR,edR);
	if(bRegionLine)clearRegionLine();

	stR=st;
	edR=ed;

	drawSurface(AR_YSTART2,stR,edR);

	updateRegionLine();
	if(hasLine2)ARDrawEnlarge(datbuf,datbufLen);

}
