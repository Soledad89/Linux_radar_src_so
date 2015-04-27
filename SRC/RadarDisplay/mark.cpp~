#include <string.h>
#include "mark.h"
#include "ConvertColor.h"

CMark::CMark(CDataPool *dp,CPPI*ppi)
{
	this->dp  = dp;
	this->ppi = ppi;


	backColor=0x00;//black
	foreColor=0x00ff0000;//green
	echoColor=0xffff0000;//yellow

	boldDisColor=changeBright(foreColor,130);
	thinDisColor=changeBright(foreColor,50);
	boldAziColor=changeBright(foreColor,130);
	thinAziColor=changeBright(foreColor,50);
	scaleColor=changeBright(foreColor,120);

	for(int i=0;i<5;i++)layerFlag[i]=true;

	fixLabelType=0;
	labelNum=0;
	pNum=0;

	boldDisStep=200;// unit pixel 0~dispRadius
	thinDisStep=50;


	boldAziStep=512;// unit aziIndex 0~4095
	thinAziStep=128;

	hasw=true;
	hasp=false;

	prowLen=ADDR_NEAR_SMP_LEN;

	layerS2=new CLayerBase(dp->fbpPPI,dp->layerPPI,2,true);//表示第二层
	layerR2=new CLayerBase(dp->fbpPPI,dp->layerPPI,2,false);

	painter2=new CPainter(dp->painterPool,layerS2);
	eraser2=new CPainter(dp->painterPool,layerR2);

	painter2->setColor(MFB_BLUE);//MFB_GREEN
//-----------------------------------------------------------------------------
	hasc=false;// dis circle
	hasa=false;// azi line
	hass=false;// scale
	UpdateNewRegion();
}


CMark::~CMark()
{
	delete layerR2;
	delete layerS2;
	delete painter2;
	delete eraser2;

}
void CMark::UpdateNewRegion()
{
	this->dispRadius= ppi->dispRadius;
	rangeNum=dispRadius;
	updatePPIRegion();
	xorg=PPI_XSTART+PPI_WIDTH/2;
	yorg=PPI_YSTART+PPI_HEIGHT/2;
	wx=PPI_XSTART+PPI_WIDTH/2+150;
	wy=PPI_YSTART+PPI_HEIGHT/2-200;
	pxorg=PPI_XSTART+PPI_WIDTH/2;
	pyorg=PPI_YSTART+PPI_HEIGHT/2;

}void CMark::UpdateDisplay()
{
	clearAziLine();
	clearFixCircle();
	clearScale();
	clearProw();

	UpdateNewRegion();

	painter2->updatePPIRadius(this->dispRadius);
	eraser2->updatePPIRadius(this->dispRadius);
	setRange(0,200,50);
	updateAziLine();
	updateFixCircle();
	updateScale();
}

void CMark::setOrigin(int theX,int theY)
{
	clearAziLine();
	clearFixCircle();
	clearScale();
	clearProw();

	xorg=theX;
	yorg=theY;

	updateAziLine();
	updateFixCircle();
	updateScale();

}

int CMark::setFixCircleLabel(int type,float*pLabel,int lNum,int pn)
{
	clearFixCircle();
	if(type==0)
	{
		fixLabelType=0;
		labelNum=0;
		pNum=0;
	}
	else
	{
		
		if(lNum>20)lNum=20;

		memcpy(fixLabel,pLabel,sizeof(float)*lNum);

		fixLabelType=1;
		labelNum=lNum;
		pNum=pn;
	}

	updateFixCircle();

	return 0;

}

//------------------------------------------------------------------------------
void CMark::updateFixCircle(int cn)
{
	updateFixCircle();
}
#include <stdio.h>
void CMark::updateFixCircle()
{
	int circleNum;
	circleNum=dispRadius/boldDisStep;

	if(hasc)
	{
		eraser2->drawCircles(xorg,yorg,thinDisStep);
	}

	painter2->setColor(thinDisColor);
	painter2->drawCircles(xorg,yorg,thinDisStep);
	painter2->setColor(boldDisColor);
	if(fixLabelType==0)
	{painter2->drawDistantCircle(xorg,yorg,circleNum,rangeNum);
         printf("circleNum= %d,rangeNum=%d.\n",circleNum,rangeNum);
}
		
	else
	{
		painter2->drawCircles(xorg,yorg,boldDisStep);
		for(int i=0;i<labelNum && i<=circleNum;i++)
		{
			painter2->drawFloat(xorg+4,yorg-boldDisStep*i-16,fixLabel[i],pNum);
			if(i==circleNum)
				painter2->drawFloat(xorg+4,yorg-boldDisStep*i+3,fixLabel[i],pNum);
		}
	}
	hasc=true;

}

void CMark::updateCrossLine()
{
	if(hasl)eraser2->drawCrossLine(xorg,yorg,dispRadius);
	painter2->drawCrossLine(xorg,yorg,dispRadius);
	hasl=true;
}
void CMark::updateScale()
{
	if(hass)eraser2->drawScale(xorg,yorg,0);
	painter2->drawScale(xorg,yorg,0);
	hass=true;
}
//------------------------------------------------------------------------------
void CMark::updateAziLine()
{
	int a;

	if(hasa)
	{
		for(a=0;a<4096;a+=thinAziStep)
			eraser2->drawLine(xorg,yorg,a,dispRadius);
	}

	painter2->setColor(thinAziColor);
	for(a=0;a<4096;a+=thinAziStep)
		painter2->drawLine(xorg,yorg,a,dispRadius);

	painter2->setColor(boldAziColor);
	for(a=0;a<4096;a+=boldAziStep)
		painter2->drawLine(xorg,yorg,a,dispRadius);
	
	hasa=true;
}

void CMark::clearAziLine()
{
	int a;

	if(hasa)
	{
		for(a=0;a<4096;a+=thinAziStep)
			eraser2->drawLine(xorg,yorg,a,dispRadius);
	}
	hasa=false;
}

void CMark::clearProw()
{
	if(hasp)eraser2->drawLine(pxorg,pyorg,prowAzi,prowLen);
	hasp=false;
}

void CMark::clearFixCircle()
{
	int circleNum;
	circleNum=dispRadius/boldDisStep;
	if(hasc)
	{
		if(fixLabelType==0)
			eraser2->drawDistantCircle(xorg,yorg,circleNum,rangeNum);
		else
		{
			eraser2->drawCircles(xorg,yorg,boldDisStep);
			for(int i=0;i<labelNum && i<=circleNum;i++)
			{
				eraser2->drawFloat(xorg+4,yorg-boldDisStep*i-16,fixLabel[i],pNum);
				if(i==circleNum)
					eraser2->drawFloat(xorg+4,yorg-boldDisStep*i+3,fixLabel[i],pNum);
			}
		}

		eraser2->drawCircles(xorg,yorg,thinDisStep);
	}
	hasc=false;
}

void CMark::clearCrossLine()
{
	if(hasl)eraser2->drawCrossLine(xorg,yorg,dispRadius);
	hasl=false;
}
void CMark::clearScale()
{
	if(hass)
{
printf("xorg =%d,yorg=%d.\n",xorg,yorg);
eraser2->drawScale(xorg,yorg,0);

}
	hass=false;
}

void CMark::clearWindow()
{
	//if(hasw)eraser2->drawNoFillRect( wx, wy, wx+WIN_WIDTH/2, wy+WIN_HEIGHT/2 );
	//hasw=false;
}

void CMark::updatePPIRegion(void)
{
	PPI_XSTART= dp->PPI_XSTART;
	PPI_YSTART= dp->PPI_YSTART;
	PPI_WIDTH= dp->PPI_WIDTH;
	PPI_HEIGHT= dp->PPI_HEIGHT;

	WIN_XSTART= dp->WIN_XSTART;
	WIN_YSTART= dp->WIN_YSTART;
	WIN_WIDTH= dp->WIN_WIDTH;
	WIN_HEIGHT= dp->WIN_HEIGHT;

	AR_XSTART= dp->AR_XSTART;
	AR_YSTART= dp->AR_YSTART;
	AR_WIDTH= dp->AR_WIDTH;
	AR_HEIGHT= dp->AR_HEIGHT;
}

void CMark::updateFbp(void)
{
	layerS2->setFbp(dp->fbpPPI);
	layerR2->setFbp(dp->fbpPPI);
}

//------------------------------------------------------

int CMark::setLayerDisplay(int theLayer,int flag)
{

	switch(theLayer)
	{
	case 1:
		if(flag)
		{
			this->ppi->setPPIDisplay(true);
		}
		else
		{
			this->ppi->setPPIDisplay(false);
			this->ppi->clear();
		}
		break;
	case 2:
		if(flag)// 1 is show
		{
			updateAziLine();
			updateFixCircle();
			updateScale();

		}
		else
		{
			clearAziLine();
			clearFixCircle();
			clearProw();
			clearScale();
		}
		break;

	//case 3:
	//	if(flag)
	//	{
	//		this->track->updateAllDots();
	//	}
	//	else
	//	{
	//		this->track->clearAllDots();
	//	}
	//	break;
	//case 4:
	//	if(flag)
	//	{
	//		this->track->updateAllTraces();
	//	}
	//	else
	//	{
	//		this->track->clearAllTraces();
	//	}
	//	break;
	case 5:
		break;
	default:
		break;
	}
	return 0;
}

int CMark::setBackColor(unsigned int color)
{
	backColor=color;
	dp->PPIColorTable[0]=csectocmem(backColor);

	this->ppi->clear();

	return 0;
}

int CMark::setForeColor(unsigned int color)
{
	foreColor=color;

	setBoldAziLineBright(getBright( cmemtocsec(boldAziColor) ));
	setThinAziLineBright(getBright( cmemtocsec(thinAziColor) ));
	setBoldDisLineBright(getBright( cmemtocsec(boldDisColor) ));
	setThinDisLineBright(getBright( cmemtocsec(thinDisColor) ));
	setScaleBright(getBright( cmemtocsec(scaleColor) ));
	

	return 0;
}

int CMark::setRange(float range,float bstep,float tstep)// units are all kilometer
{
//	int bnum=range/bstep;
//	int tnum=range/tstep;
	int bnum=rangeNum/bstep;
	int tnum=rangeNum/tstep;
	//clearFixCircle();
//	setLayerDisplay(2,0);

	rangeNum=range;

	boldDisStep=dispRadius/bnum;
	thinDisStep=dispRadius/tnum;

	//updateFixCircle();
//	setLayerDisplay(2,1);

	return 0;
}

int CMark::setAziLine(float bstep,float tstep)// units are all degree
{
	int bnum=360.0/bstep;
	int tnum=360.0/tstep;

	//clearAziLine();
	setLayerDisplay(2,0);

	boldAziStep=4096/bnum;
	thinAziStep=4096/tnum;

	//updateAziLine();
	setLayerDisplay(2,1);

	return 0;
}

int CMark::setBoldAziLineBright(unsigned char bright)
{
	boldAziColor=changeBright(foreColor,bright);
	updateAziLine();

	return 0;
}

int CMark::setThinAziLineBright(unsigned char bright)
{
	thinAziColor=changeBright(foreColor,bright);
	updateAziLine();

	return 0;
}

int CMark::setBoldDisLineBright(unsigned char bright)
{
	boldDisColor=changeBright(foreColor,bright);
	updateFixCircle();

	return 0;
}

int CMark::setThinDisLineBright(unsigned char bright)
{
	thinDisColor=changeBright(foreColor,bright);
	updateFixCircle();

	return 0;
}

int CMark::setScaleBright(unsigned char bright)
{
	scaleColor=changeBright(foreColor,bright);
	updateScale();

	return 0;
}

int CMark::setBackBright(unsigned char bright)
{
	backColor=changeBright(backColor,bright);//c16toc32(changeBright(backColor,bright));

	for(int i=0;i<bright;i++)
		dp->PPIColorTable[i]=backColor;

	this->ppi->clear();

	backColor=cmemtocsec(backColor);
	return 0;
}

int CMark::getRecord(int theX,int theY,float *azi,float *dis)
{

	return 0;
}

