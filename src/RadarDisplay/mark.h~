

#ifndef __CMARK__
#define __CMARK__

#include "global.h"
#include "datapool.h"
#include "PPI.h"
#include "layers.h"


class CMark
{

public:
	CMark(CDataPool *dp,CPPI*ppi);
	~CMark();
public:
	int dispRadius;

public:
	void updateFixCircle(int cn);
	void updateFixCircle();
	void updateCrossLine();
	void updateScale();
	void updateAziLine();

	void clearProw();
	void clearFixCircle();
	void clearCrossLine();
	void clearScale();
	void clearWindow();
	void clearAziLine();

	void updatePPIRegion(void);//
	void updateFbp(void);//

	int setLayerDisplay(int theLayer,int flag);
	int setBackColor(unsigned int color);
	int setForeColor(unsigned int color);
	int setRange(float range,float bstep,float tstep);// units are all kilometer
	int setAziLine(float bstep,float tstep);// units are all degree
	int setBoldAziLineBright(unsigned char bright);
	int setThinAziLineBright(unsigned char bright);
	int setBoldDisLineBright(unsigned char bright);
	int setThinDisLineBright(unsigned char bright);
	int setScaleBright(unsigned char bright);
	int setBackBright(unsigned char bright);

	int setFixCircleLabel(int type,float*pLabel,int lNum,int pn);

	int getRecord(int theX,int theY,float *azi,float *dis);

	void setOrigin(int theX,int theY);
	void UpdateNewRegion();
	void UpdateDisplay();
public:
	int PPI_XSTART;
	int PPI_YSTART;
	int PPI_WIDTH;
	int PPI_HEIGHT;

	int WIN_XSTART;
	int WIN_YSTART;
	int WIN_WIDTH;
	int WIN_HEIGHT;

	int AR_XSTART;
	int AR_YSTART;
	int AR_WIDTH;
	int AR_HEIGHT;

private:
	CDataPool *dp;
	CPPI*ppi;

	int xorg,yorg;
	// SecGen color format
	unsigned int backColor;
	unsigned int foreColor;
	unsigned int echoColor;

	// Mem color format
	DISPMEMTYPE boldDisColor;
	DISPMEMTYPE thinDisColor;
	DISPMEMTYPE boldAziColor;
	DISPMEMTYPE thinAziColor;
	DISPMEMTYPE scaleColor;

	bool layerFlag[5];

	int fixLabelType;
	float fixLabel[20];
	int labelNum;
	int pNum;

	int boldDisStep;// unit pixel 0~600
	int thinDisStep;
	int boldAziStep;// unit aziIndex 0~4095
	int thinAziStep;

	int rangeNum;

	bool hasc;// dis circle
	bool hasa;// azi line
	bool hass;// scale
	bool hasl;// cross line

	bool hasw;// window
	int wx,wy;

	bool hasp;//prow
	int pxorg,pyorg;
	int prowAzi;
	int prowLen;

	CLayerBase *layerS2;
	CLayerBase *layerR2;

	CPainter *painter2;
	CPainter *eraser2;


};

#endif
