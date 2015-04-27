
#include   <stdlib.h>  //exit函数 
#include "RadarDis.h"
#include "debug.h"

int CRadarDisplay::ObjectNum = 0;

CRadarDisplay::CRadarDisplay()
{
    //-------------------------------------
        ObjectNum++;
	this->PPI_Surface = NULL;
	this->AR_Surface = NULL;
	this->MINI_WIN_Surface= NULL;
    //------------------------------------------------------------------------------
    	this->dp = new CDataPool();	
    	MEMALLOCCHECK(dp);
	printf("dp -------\n");
    	this->initState();
	fprintf(debugfp,"---------CDataPool class created! -----\n");

	map = new CMap( dp );
	MEMALLOCCHECK(map);
	fprintf(debugfp,"---------CMap class created! -----\n");
	printf("map -------\n");

        ppi = new CPPI( dp );
	MEMALLOCCHECK(ppi);
	fprintf(debugfp,"---------CPPI class created! -----\n");
	printf("ppi -------\n");

    	yh = new CYh( dp );
	MEMALLOCCHECK(yh);
	fprintf(debugfp,"---------CYh class created! -----\n");
	printf("yh -------\n");

	ar = new CAR(dp);
	MEMALLOCCHECK(ar);
	fprintf(debugfp,"---------CAR class created! -----\n");
	printf("yh -------\n");

	mark = new CMark(this->dp,this->ppi);
	MEMALLOCCHECK(mark);
	fprintf(debugfp,"---------CMark class created! -----\n");
	printf("mark -------\n");
	//-------------------------------------------------------
	this->IsPPIOpenFlag = false;
	this->IsAROpenFlag = false;
	this->IsMiniWinOpenFlag = false;
	//-------------------------------------------------------
	this->arStep = 2;

}
CRadarDisplay::~CRadarDisplay()
{
	if(dp) delete dp;
	printf("delete dp ok!\n");
	if(ppi) delete ppi;
	printf("delete ppi ok!\n");
	if(yh) delete yh;
	printf("delete yh ok!\n");
	if(map) delete map;
	printf("delete map ok!\n");
	if(mark) delete mark;
	printf("delete mark ok!\n");
	if(this->PPI_Surface)fb_destroy_surface(PPI_Surface,true);
	if(this->AR_Surface)fb_destroy_surface(AR_Surface,true);
	if(this->MINI_WIN_Surface)fb_destroy_surface(MINI_WIN_Surface,true);
	printf("fb_destroy_surface OK!\n");
}
int CRadarDisplay::LoadMapToPPI(char* filename)
{
	if(!map)return -1;
	if(!map->LoadPPM(filename)) return -2;//"./bmp_ppm/map.ppm"
	if(!map->DrawPPMtolayerPPI0()) return -3;
	return 1;
}
void CRadarDisplay::initmark()
{
	mark->setRange(0,200,50);
	mark->updateAziLine();
	mark->updateFixCircle();
	mark->updateScale();
}
void CRadarDisplay::cleanmark()
{
	mark->clearAziLine();
	mark->clearFixCircle();
	mark->clearScale();

}
void  CRadarDisplay::initState()
{
dp->dispState.northIndex = 0;
dp->dispState.prowIndex = 0;
dp->dispState.xorg = dp->PPI_XSTART+dp->PPI_WIDTH/2;
dp->dispState.yorg = dp->PPI_YSTART+dp->PPI_HEIGHT/2;
dp->dispState.prowCorrect = 0;
dp->dispState.distanceCorrect = 0;
dp->dispState.gain = 100;// gain/100 is the times
dp->dispState.range = 10;
dp->dispState.base = -20;
dp->dispState.dispMode = 0;
dp->dispState.moveMode = 0;
dp->dispState.mileKilo = 0;
dp->dispState.dayNight = 0;
dp->dispState.tailTime = 0;
dp->dispState.cluterControl = 1;
dp->dispState.rainSnowControl = 1;
dp->dispState.cofreqControl = 0;
dp->dispState.vecType = 0;
dp->dispState.vecTime = 1;

dp->dispState.enFirstInfo = 1;
dp->dispState.enDivert = false;
dp->dispState.enSplitDisp = 0;
dp->dispState.enWindowDisp = true;
dp->dispState.enProwLine = 0;
dp->dispState.enFixedScale = 1;
dp->dispState.enEchoWiden = 0;
dp->dispState.scopeMode = 1; // p 显 
dp->dispState.enCloseFlag = false;
}
void  CRadarDisplay::UpdateDisplay(s_ppi_video_for_disp *dispdat)
{
	int a = 0;
	int j = 0;
		if(ppi->isPPIDisplay())
		{
			ppi->ppiDraw(dispdat);
			if(--arStep<=0){
				ar->ARDraw(dispdat);
				arStep=2;
			}
			if(ppi->isScanLine()&&ppi->scopeMode == 1)//ppi 显示
			{
				for(j=0;j< dispdat->znum;j++)
				{
					a=(dispdat->azicnt[j])%4096;
					ppi->ppiClearScanLine(a);

					ppi->ppiDrawScanLine(a+1);
				}

			}
		}
}

//＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊二代函数接口＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊
//------PPI------
 Status CRadarDisplay::ML_GVID_OpenPPI(short theX, short theY , short theWidth, short theHeight)
{
	if(!IsPPIOpenFlag)
	{
		if(this->PPI_Surface)fb_destroy_surface(PPI_Surface,true);
		PPI_Surface=fb_create_sectionsurface(theX,theY,theWidth,theHeight,fb_screen);
                RadarDisplayRect[TypePPI].left_x = theX;
                RadarDisplayRect[TypePPI].left_y = theY;
                RadarDisplayRect[TypePPI].width  = theWidth;
                RadarDisplayRect[TypePPI].height = theHeight;
		dp->SetFbDev( PPI_Surface);	
		/*设置绘图位置和接口*/
		ppi->UpdatePPIDisplay();
		yh->UpdateYHDispaly();	
		ppi->setPPIDisplay(true);
		yh->enableYh();
		this->IsPPIOpenFlag = true;
		return 1;
	}
	else
		return -1;
}

Status CRadarDisplay::ML_GVID_ClosePPI( )
{
	if(IsPPIOpenFlag)
	{
		ppi->setPPIDisplay(false);
                RadarDisplayRect[TypePPI].left_x = 0;
                RadarDisplayRect[TypePPI].left_y = 0;
                RadarDisplayRect[TypePPI].width  = 0;
                RadarDisplayRect[TypePPI].height = 0;
		this->IsPPIOpenFlag = false;
		return 1;
	}
	else
		return -1;	
}
//----------------------------------------------------------------------------------
Status CRadarDisplay::ML_GVID_SetPPIXY(  short theX ,short theY)
{
	if(IsPPIOpenFlag)
	{	
		if(this->PPI_Surface)fb_destroy_surface(PPI_Surface,true);
                PPI_Surface=fb_create_sectionsurface(theX,theY,RadarDisplayRect[TypePPI].width, RadarDisplayRect[TypePPI].height,fb_screen);
                RadarDisplayRect[TypePPI].left_x = theX;
                RadarDisplayRect[TypePPI].left_y = theY;
                dp->SetFbDev( PPI_Surface);
		ppi->UpdatePPIDisplay();
		yh->UpdateYHDispaly();	
		return 1;
	}
	else
		return -1;
}

Status CRadarDisplay::ML_GVID_SetPPIWH(  short theWidth, short theHeight)
{
	if(IsPPIOpenFlag)
	{	
		if(this->PPI_Surface)fb_destroy_surface(PPI_Surface,true);
                PPI_Surface=fb_create_sectionsurface(RadarDisplayRect[TypePPI].left_x,RadarDisplayRect[TypePPI].left_y,theWidth,theHeight,fb_screen);
                RadarDisplayRect[TypePPI].width  = theWidth;
                RadarDisplayRect[TypePPI].height = theHeight;
                dp->SetFbDev( PPI_Surface);
		ppi->UpdatePPIDisplay();
		yh->UpdateYHDispaly();	
		return 1;
	}
	else
		return -1;
}
//-----------------------------------------------------------------------------------
Status CRadarDisplay::ML_GVID_SetPPIMode( short theMode)
{


	if(IsPPIOpenFlag)
	{
            dp->dispState.scopeMode=theMode;
            ppi->setScopeMode(theMode);
		return 1;
	}
	else
		return -1;

}
//need to been modify
Status CRadarDisplay::ML_GVID_SetPPICenter(  short theX ,short theY)
{
	if(IsPPIOpenFlag)
        {
		if( theX==dp->PPI_XSTART+dp->PPI_WIDTH/2 && theX==dp->PPI_YSTART+dp->PPI_HEIGHT/2 )
		   {
		    dp->dispState.enDivert=false;
		    ppi->setDivert(false);
		    }
		else
		    {
		    dp->dispState.enDivert=true;
		    ppi->setDivert(true);
		    }	 
	    	ppi->setPPIOrigin(theX,theY);
		ppi->updateWinOrigin();
	    	ppi->clear();
		return 1;
	}
	else
		return -1;


}
Status CRadarDisplay::ML_GVID_SetPPIShipDir( float theShipDir)
{
	if(IsPPIOpenFlag)
	{
		if(theShipDir>=0 && theShipDir<360){
			dp->shipState.prowAzi=theShipDir;
			ppi->updateAziBase();
		}
		return 1;
	}
	else
		return -1;



}
Status CRadarDisplay::ML_GVID_SetPPIColor( unsigned int theColor)
{
	if(IsPPIOpenFlag)
	{
		dp->SetPPIDisplayColor( (theColor>>24) & 0x0ff,(theColor>>16) & 0x0ff,(theColor>>8) & 0x0ff );
		return 1;
	}
	else
		return -1;
	
}
Status CRadarDisplay::ML_GVID_SetPPIScan( int theScan)
{
	if(IsPPIOpenFlag)
	{
		ppi->setScanLine( (theScan==0) );
		return 1;
	}
	else
		return -1;
		
}
Status CRadarDisplay::ML_GVID_SetPPILineColor( unsigned int theColor)
{
	if(IsPPIOpenFlag)
	{
            ppi->setScanLineColor(  ((theColor & 0xf8000000)>>16 | (theColor & 0x00fc0000)>>13 | (theColor & 0x0000f800)>>11) & 0x0000ffff );
		return 1;
	}
	else
		return -1;

}
Status CRadarDisplay::ML_GVID_FrostPPI( char theFrost)
{
	if(IsPPIOpenFlag)
	{
		if (theFrost == 0|| theFrost ==1)
			{
				ppi->setPPIDisplay( theFrost== 1);
			}
		return 1;
	}
	else
		return -1;

	
}
Status CRadarDisplay::ML_GVID_SetPPIDist( short theDist)
{
	if(IsPPIOpenFlag)
	{
		if( dp->dispState.range != theDist )
		{
		    dp->dispState.range = theDist;
		    ppi->setRange( theDist );
		    ppi->updateCorrectIndex();
		    mark->updateFixCircle();
		}
		return 1;
	}
	else
		return -1;


}
Status CRadarDisplay::ML_GVID_SetPPISwitch( short theSwitch)
{
	if(IsPPIOpenFlag)
	{
		ppi->setPPIDisplay( theSwitch==0 );
		if(theSwitch==1)ppi->clear();
		return 1;
	}
	else
		return -1;

}
Status CRadarDisplay::ML_GVID_SetPPIGate( char theGate)
{
	if(IsPPIOpenFlag)
	{
		ppi->setBase(theGate);
		return 1;
	}
	else
		return -1;
	
}
Status CRadarDisplay::ML_GVID_SetPPITail( int theTail)
{
	if(IsPPIOpenFlag)
	{
		yh->setYhWeight(theTail);// theTail is 1~16, small num refers to the fast yh
		return 1;
	}
	else
		return -1;
	
}

//-------------------------------------------AR-----------------------------------------------
 Status CRadarDisplay::ML_GVID_OpenAr(short theX, short theY , short theWidth, short theHeight)
{
	if(!IsAROpenFlag)
	{
		if(AR_Surface)fb_destroy_surface(AR_Surface,true);
		AR_Surface=fb_create_sectionsurface(theX,theY,theWidth,theHeight,fb_screen);
                RadarDisplayRect[TypeAR].left_x = theX;
                RadarDisplayRect[TypeAR].left_y = theY;
                RadarDisplayRect[TypeAR].width  = theWidth;
                RadarDisplayRect[TypeAR].height = theHeight;
                dp->SetFbDev(NULL,AR_Surface);
		ar->UpdateARDispaly();
		ar->setARDispaly(true);
		IsAROpenFlag = true;
		return 1;
	}
	else
		return -1;
}
 Status CRadarDisplay::ML_GVID_CloseAr()
{
        if(IsAROpenFlag)
        {
            RadarDisplayRect[TypeAR].left_x = 0;
            RadarDisplayRect[TypeAR].left_y = 0;
            RadarDisplayRect[TypeAR].width  = 0;
            RadarDisplayRect[TypeAR].height = 0;
            ar->setARDispaly(false);
            ar->clear();
            this->IsAROpenFlag = false;
            return 1;
        }
        else
                return -1;
}
 Status CRadarDisplay::ML_GVID_SetArXY(short theX, short theY)
{
        if(IsAROpenFlag)
        {
            if(this->AR_Surface)fb_destroy_surface(AR_Surface,true);
            AR_Surface=fb_create_sectionsurface(theX,theY,RadarDisplayRect[TypeAR].width, RadarDisplayRect[TypeAR].height,fb_screen);
            RadarDisplayRect[TypeAR].left_x = theX;
            RadarDisplayRect[TypeAR].left_y = theY;
            dp->SetFbDev(NULL,AR_Surface);
            ar->UpdateARDispaly();
            return 1;
        }
        else
                return -1;
}
 Status CRadarDisplay::ML_GVID_SetArWH(short theWidth, short theHeight)
{
        if(IsAROpenFlag)
        {
            if(this->AR_Surface)fb_destroy_surface(AR_Surface,true);
            AR_Surface=fb_create_sectionsurface(RadarDisplayRect[TypeAR].left_x,RadarDisplayRect[TypeAR].left_y,theWidth,theHeight,fb_screen);
            RadarDisplayRect[TypeAR].width  = theWidth;
            RadarDisplayRect[TypeAR].height = theHeight;
            dp->SetFbDev(NULL,AR_Surface);
            ar->UpdateARDispaly();
                return 1;
        }
        else
                return -1;
}
 Status CRadarDisplay::ML_GVID_SetArGate(char theGate)
{
        if(IsAROpenFlag)
        {
                ar->setARGate(theGate);
                return 1;
        }
        else
                return -1;
}
 Status CRadarDisplay::ML_GVID_SetArDist (short theDist)
 {
     if(IsAROpenFlag)
     {
             ar->setARDistance(theDist);
             return 1;
     }
     else
             return -1;

 }
//----------------------------------MiniWin-------------------------------
 Status CRadarDisplay::ML_GVID_OpenMiniWin(short theX, short theY , short theWidth, short theHeight)
 {
        if(!IsMiniWinOpenFlag)
        {
            if(MINI_WIN_Surface)fb_destroy_surface(MINI_WIN_Surface,true);
            MINI_WIN_Surface=fb_create_sectionsurface(theX,theY,theWidth,theHeight,fb_screen);
            RadarDisplayRect[TypeWIN].left_x = theX;
            RadarDisplayRect[TypeWIN].left_y = theY;
            RadarDisplayRect[TypeWIN].width  = theWidth;
            RadarDisplayRect[TypeWIN].height = theHeight;
            dp->SetFbDev(NULL,NULL,MINI_WIN_Surface);
            ppi->UpdatePPIDisplay();
            yh->UpdateYHDispaly();
            dp->dispState.enWindowDisp=(true);
            ppi->setWindowDisplay(true);
            yh->setYhWin(true);
            IsMiniWinOpenFlag = true;      
            return 1;
        }
        else
                return -1;
 }
 Status CRadarDisplay::ML_GVID_CloseMiniWin( )
 {
     if(IsMiniWinOpenFlag)
     {
         RadarDisplayRect[TypeWIN].left_x = 0;
         RadarDisplayRect[TypeWIN].left_y = 0;
         RadarDisplayRect[TypeWIN].width  = 0;
         RadarDisplayRect[TypeWIN].height = 0;
         dp->dispState.enWindowDisp=(false);
         ppi->setWindowDisplay(false);
         yh->setYhWin(false);
             return 1;
     }
     else
             return -1;

 }
 Status CRadarDisplay::ML_GVID_SetMiniWinXY( short theX ,short theY)
 {
     if(IsMiniWinOpenFlag)
     {
         if(MINI_WIN_Surface)fb_destroy_surface(MINI_WIN_Surface,true);
         MINI_WIN_Surface=fb_create_sectionsurface(theX,theY,RadarDisplayRect[TypeWIN].width,RadarDisplayRect[TypeWIN].height,fb_screen);
         dp->SetFbDev(NULL,NULL,MINI_WIN_Surface);
         RadarDisplayRect[TypeWIN].left_x = theX;
         RadarDisplayRect[TypeWIN].left_y = theY;
         ppi->UpdatePPIDisplay();
         yh->UpdateYHDispaly();

             return 1;
     }
     else
             return -1;

 }
 Status CRadarDisplay::ML_GVID_SetMiniWinWH( short theWidth, short theHeight)
 {
     if(IsMiniWinOpenFlag)
     {
         if(MINI_WIN_Surface)fb_destroy_surface(MINI_WIN_Surface,true);
         MINI_WIN_Surface=fb_create_sectionsurface(RadarDisplayRect[TypeWIN].left_x,RadarDisplayRect[TypeWIN].left_y,theWidth,theHeight,fb_screen);
         dp->SetFbDev(NULL,NULL,MINI_WIN_Surface);
         RadarDisplayRect[TypeWIN].width  = theWidth;
         RadarDisplayRect[TypeWIN].height = theHeight;
         ppi->UpdatePPIDisplay();
         yh->UpdateYHDispaly();

             return 1;
     }
     else
             return -1;

 }
 Status CRadarDisplay::ML_GVID_SetMiniWinMode( short theMode)
 {
     if(IsMiniWinOpenFlag)
     {
             ML_GVID_SetPPIMode(theMode);
             return 1;
     }
     else
             return -1;

 }
 Status CRadarDisplay::ML_GVID_SetMiniWinColor(unsigned int theColor)
 {
     if(IsMiniWinOpenFlag)
     {
             dp->SetWinDisplayColor((theColor>>24) & 0x0ff,(theColor>>16) & 0x0ff,(theColor>>8) & 0x0ff);
             return 1;
     }
     else
             return -1;

 }
 Status CRadarDisplay::ML_GVID_SetMiniWinDistDir( short theDistance ,float theDirection)
 {
     if(IsMiniWinOpenFlag)
     {
         int dis  = ppi->dispRadius *((double)theDistance /(double)dp->dispState.range);
         if(dis > 511) dis = 511;

         int azi = (int)(floorf(theDirection/AZIDIS)+dp->dispState.northIndex)%4096;//角度相对正北

         int x=dp->addr_near_x[azi][dis]+dp->dispState.xorg;
         int y=-dp->addr_near_y[azi][dis]+dp->dispState.yorg;

         x -= 85;//小窗口长宽170x170，输入参数为窗口中心位置，需换算为窗口左上角坐标
         y -= 85;

         ppi->setWinOrigin(x,y);
         return 1;
     }
     else
         return -1;

 }
 Status CRadarDisplay::ML_GVID_SetMiniWinScale( int theScale)
 {

     if(IsMiniWinOpenFlag)
     {
             return 1;
     }
     else
             return -1;

 }
//----------------------------------------------------------------------

