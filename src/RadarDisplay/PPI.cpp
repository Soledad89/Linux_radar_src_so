
#include "complex.h"
#include "debug.h"
#include "PPI.h"
//#include "../FrameBuffer/FrameBuffer.h"


CPPI::CPPI( CDataPool *dp )
{
    m_dp = dp;
//------------------------------------------------------------------------------
    addr_near_x = m_dp->addr_near_x;
    addr_near_y = m_dp->addr_near_y;
    addr_far_x = m_dp->addr_far_x;
    addr_far_y = m_dp->addr_far_y;

    PPIColorTable = m_dp->PPIColorTable;
    WinColorTable=m_dp->WinColorTable;

    fbpPPI = m_dp->fbpPPI;
    layerPPI = m_dp->layerPPI;
	
    fbpMINI = m_dp->fbpMINI;
    layerMINI = m_dp->layerMINI;
//------------------------------------------------------------------------------
    bDivert = m_dp->dispState.enDivert;
    bSplitDisp = m_dp->dispState.enSplitDisp;
//自己控制，不需要通过datapool传数据来控制
    bWindowDisp = false; // m_dp->dispState.enWindowDisp; // 是否显示小窗口
    bPPIDisplay = false; //m_dp->dispState.enFirstInfo;

    bEchoWiden = m_dp->dispState.enEchoWiden;
    dispMode = m_dp->dispState.dispMode;
    moveMode = m_dp->dispState.moveMode;

    scopeMode = m_dp->dispState.scopeMode;// 0 b 1 ppi 2 E

    xorg = m_dp->dispState.xorg;
    yorg = m_dp->dispState.yorg;

    gain = m_dp->dispState.gain;
    base = m_dp->dispState.base;
	
    range = m_dp->dispState.range;
    benCloseFlag = m_dp->dispState.enCloseFlag;
    northIndex = m_dp->dispState.northIndex;
    prowIndex = m_dp->dispState.prowIndex;
//------------------------------------------------------------------------------
    bScanLine = false;
    prowCorrectIndex = 0;
    distanceCorrectIndex = 0;
    halfwidth = 10;
    distancelen = 5;
    dispRadius = MIN(xorg,yorg);//------------显示半径－－－－－－－－－－－－－－－－－－－－－－－
    scanLineColor = MFB_BLUE;
    hollowNum=0;
//------------------------------------------------------------------------------

    updateRegion();
    updateTransTable();
    
}

CPPI::~CPPI()
{
    TRACE("CPPI::~CPPI()");
}

void CPPI::UpdatePPIDisplay()
{

	updateFbp();
	updateRegion();
}
//设置PPI的中心位置
void CPPI::setPPIOrigin( int mx, int my )
{
    if((mx>PPI_XSTART) && (mx<PPI_XSTART+PPI_WIDTH) && (my>PPI_YSTART) && (my<PPI_YSTART+PPI_HEIGHT))
    {
        xorg = mx;
        yorg = my;
    }
}

void CPPI::resetPPIOrigin()
{
    xorg = PPI_XSTART+PPI_WIDTH/2;
    yorg = PPI_YSTART+PPI_HEIGHT/2;
}
void CPPI::updateWinOrigin() //更新窗口起始点
{
    setWinOrigin(win_xstart,win_ystart);
}


void CPPI::updateRegion(void)
{
	clear();

        PPI_XSTART=0;
        PPI_YSTART=0;
	PPI_WIDTH=m_dp->PPI_WIDTH;
	PPI_HEIGHT=m_dp->PPI_HEIGHT;

        WIN_XSTART=0;
        WIN_YSTART=0;
	WIN_WIDTH=m_dp->WIN_WIDTH;
	WIN_HEIGHT=m_dp->WIN_HEIGHT;

        AR_XSTART=0;
        AR_YSTART=0;
	AR_WIDTH=m_dp->AR_WIDTH;
	AR_HEIGHT=m_dp->AR_HEIGHT;

	resetPPIOrigin();
        setWinOrigin(xorg/2,yorg/2);
	dispRadius = MIN(xorg,yorg);
}
//------------------------------------------------------------------------------
void CPPI::setGain( int g ){gain = g;updateTransTable();}

void CPPI::setBase( int b ){ base = b;updateTransTable();}

void CPPI::updateTransTable()
{
	int v;
	for(int n=0;n<256;n++)
	{
		v = n;
		v = (v+base)*gain/100;
		v = v<0? 0 : v;
		transTable[n]=v & 0x0ff;
	}
}

bool CPPI::isValid( int &x, int &y )
{
	return ( (x>=PPI_XSTART) && (x<PPI_XSTART+PPI_WIDTH) && (y>=PPI_YSTART) && (y<PPI_YSTART+PPI_HEIGHT)  );
}

void CPPI::updateFbp(void)//更新绘图显存区地址
{
	fbpPPI=m_dp->fbpPPI;
	fbpMINI = m_dp->fbpMINI;
}
void CPPI::updateAziBase()
{
//	switch(dispMode)
//	{
//	case 0: //正北向上
//		prowIndex=(int)floorf(m_dp->shipState.prowAzi/AZIDIS)%4096;
//		northIndex=0;
//		break;
//		
//	case 1://舰首向上
//		prowIndex=0;
//		northIndex=(int)floorf((360.0-m_dp->shipState.prowAzi)/AZIDIS)%4096;
//		break;
//		
//	case 2: //航向向上
//		prowIndex=((int)floorf((m_dp->shipState.prowAzi-m_dp->shipState.sailingAzi)/AZIDIS)+4096)%4096;
//		northIndex=((int)floorf((360.0-m_dp->shipState.prowAzi)/AZIDIS)+prowIndex)%4096;
//		
//		break;

//	}

//	prowIndex=(prowIndex+prowCorrectIndex+4096)%4096; //船首校正

//	m_dp->dispState.prowIndex=prowIndex;
//	m_dp->dispState.northIndex=northIndex;
	
}


void CPPI::ppiDraw(s_ppi_video_for_disp* dispdat)
{
	int ta;//绘图方位
	int j;

	if(bPPIDisplay)
	{
		video_data = dispdat->videodata;
		if(hollowNum==0)
		{
			for(j=0;j<CNTVIDEO;j++)//增益及基准调整
				video_data[j]=transTable[video_data[j]];
		}
		else
		{
			for(j=CNTVIDEO-1;j>=0;j--)//增益及基准调整
			{
				if(j<hollowNum)
					video_data[j]=0;
				else
					video_data[j]=transTable[video_data[j-hollowNum]];		
			}
		}
		switch(scopeMode)
		{
		case 0:
			for(j=0;j<dispdat->znum;j++)
			{
				ta=(dispdat->azicnt[j] + prowIndex)%4096;
				BScopeDraw(ta);
			}
			break;
		case 1:
			for(j=0;j<dispdat->znum;j++)
			{
				ta=(dispdat->azicnt[j] + prowIndex)%4096;
				ppiDraw(ta);
			}
			break;
		case 2:

			for(j=0;j<dispdat->znum;j++)
			{
				ta=(dispdat->azicnt[j] + prowIndex)%4096;
				EScopeDraw(ta);
			}
			break;
		default:
			break;
		}


	}

}
//画扫描线
void CPPI::ppiDrawScanLine(int a)
{
	int ta,an;
	int x,y,i;

	if(isScanLine()){
		ta=(a + prowIndex)%4096;

                if(dispRadius<=ADDR_NEAR_SMP_LEN){   //绘图半径小于600
			for(an=0;an<3;an++)
			{
				ta = (ta+an)%4096;

				for(i=0;i<dispRadius;i++)//画扫描线
				{
					x=addr_near_x[ta][i]+xorg;
					y=-addr_near_y[ta][i]+yorg;

					if(isValid(x,y))// && maskTable[y][x]==0)
					{
						if(layerPPI[y][x].curlayer<2)
						{				
							//fbpPPI[y][x]=scanLineColor;
						fb_putpixel_int( x,  y, scanLineColor, fbpPPI);

						}
					}
				}
			}
		}
		else{  //绘图半径大于512
			for(an=0;an<3;an++)
			{
				ta = (ta+an)%4096;

				for(i=0;i<ADDR_NEAR_SMP_LEN;i++)//画近区扫描线
				{
					x=addr_near_x[ta][i]+xorg;
					y=-addr_near_y[ta][i]+yorg;

					if(isValid(x,y))// && maskTable[y][x]==0)
					{
						if(layerPPI[y][x].curlayer<2)
						{				
							//fbpPPI[y][x]=scanLineColor;
							fb_putpixel_int( x,  y, scanLineColor, fbpPPI);
						}
					}
				}

				for(i=0;i<dispRadius-ADDR_NEAR_SMP_LEN;i++)//画远区扫描线
				{
					x=addr_far_x[ta*2][i]+xorg;
					y=-addr_far_y[ta*2][i]+yorg;

					if(isValid(x,y))// && maskTable[y][x]==0)
					{
						if(layerPPI[y][x].curlayer<2)
						{				
							//fbpPPI[y][x]=scanLineColor;
							fb_putpixel_int( x,  y, scanLineColor, fbpPPI);
						}
					}
				}

			}

		}

	}

}
void CPPI::ppiClearScanLine(int a)
{
	int ta,an;
	int x,y,i;

	if(isScanLine()){
		ta=(a + prowIndex)%4096;

		if(dispRadius<=ADDR_NEAR_SMP_LEN){   //绘图半径小于512
			for(an=0;an<3;an++)
			{
				ta = (ta+an)%4096;

				for(i=0;i<dispRadius;i++)//画扫描线
				{
					x=addr_near_x[ta][i]+xorg;
					y=-addr_near_y[ta][i]+yorg;

					if(isValid(x,y))// && maskTable[y][x]==0)
					{
						if(layerPPI[y][x].curlayer<2)
						{				
							//fbpPPI[y][x]=layerPPI[y][x].nfirst;
						fb_putpixel_int( x,  y, layerPPI[y][x].nfirst, fbpPPI);
						}
					}
				}
			}
		}
		else{  //绘图半径大于512
			for(an=0;an<3;an++)
			{
				ta = (ta+an)%4096;

				for(i=0;i<ADDR_NEAR_SMP_LEN;i++)//画近区扫描线
				{
					x=addr_near_x[ta][i]+xorg;
					y=-addr_near_y[ta][i]+yorg;

					if(isValid(x,y))// && maskTable[y][x]==0)
					{
						if(layerPPI[y][x].curlayer<2)
						{				
							//fbpPPI[y][x]=layer[y][x].nfirst;
							fb_putpixel_int( x,  y, layerPPI[y][x].nfirst, fbpPPI);
						}
					}
				}

				for(i=0;i<dispRadius-ADDR_NEAR_SMP_LEN;i++)//画远区扫描线
				{
					x=addr_far_x[ta*2][i]+xorg;
					y=-addr_far_y[ta*2][i]+yorg;

					if(isValid(x,y))// && maskTable[y][x]==0)
					{
						if(layerPPI[y][x].curlayer<2)
						{				
							//fbpPPI[y][x]=layerPPI[y][x].nfirst;
							fb_putpixel_int( x,  y, layerPPI[y][x].nfirst, fbpPPI);
						}
					}
				}

			}

		}

	}

}

void CPPI::BScopeDraw( int azicnt )
{
	int i,x,y,readytobreak;
	int v;

	x = azicnt/4;
	if(x>=1200)return;

	for(  i=0;i<600;i++ )
	{
		v = video_data[i];
		if(v==0)
			continue;
		else{
			v += base;
			if(v<=0)continue;
		}

		y = i+300;

		if( isValid(x,y) )
		{
			setFirstPixel( x, y, v);
		}
		//else
		//{
		//	if(readytobreak==5)
		//	{
		//		//printf("break when i=%d\n",i);
		//		break;
		//	}
		//	else
		//		readytobreak++;
		//}
	}

	bool dowin;
	if(bWindowDisp)//bWindowDisp
	{

		dowin=false;
		if(azicnt>=win_aziIndex-340 && azicnt<win_aziIndex+340)dowin=true;

		if(dowin)
		{    
			int vn;
			readytobreak = 0;
			x = azicnt/4;
			x =(x-(win_aziIndex/4-85))*2+WIN_XSTART;
			y = WIN_YSTART;   
			//fprintf(debugfp,"x=%d\n",x);
			for(  i=win_disIndex-85;i<win_disIndex+85;i++ )//画近区
			{	
				vn=video_data[i];
				//if( isValid(x,y) )
				//if(x>=1260&&x<1599&&y>=0&&y<399)
				if(winIsValid(x,y))
				{
					winSetFirstPixel( x, y, vn);
					winSetFirstPixel( x, y+1, vn);
					winSetFirstPixel( x+1, y, vn);
					winSetFirstPixel( x+1, y+1, vn);			
				//	fprintf(debugfp,"y=%d\n",y);

				}
				y+=2;
			}
		}        
	} //end window


}
void CPPI::EScopeDraw(int azicnt)
{
	int i,x,y,readytobreak;
	int v;

	y = azicnt/4 +90;
	if(y>=1200)return;

	for(  i=0;i<600;i++ )//画近区
	{
		v = video_data[i];
		if(v==0)
			continue;
		else{
			v += base;
			if(v<=0)continue;
		}

		x = i+300;// note this is minus

		if( isValid(x,y) )
		{

			setFirstPixel( x, y, v);
		}
		else
		{
			//填充死区的地址表不是完全顺序的,一个越界地址的后面可能还有另一个不越界的地址
			if(readytobreak==5)
			{
				//printf("break when i=%d\n",i);
				break;
			}
			else
				readytobreak++;
		}
	}
}
//-------------------------------------------------------------------------------
void CPPI::ppiDraw( int azicnt )
{
        int x, y;
        int readytobreak;
	int v;
	int i;

	int n;

        if(dispRadius>ADDR_NEAR_SMP_LEN)//>512
	{
		if( isDivert() || moveMode==1)//偏心显示模式(真运动或偏心)
				n=ADDR_FAR_SMP_LEN+ADDR_NEAR_SMP_LEN;
			else
                                n=dispRadius;

                readytobreak = 0;

                for(  i=0;i<ADDR_NEAR_SMP_LEN;i++ )//画512
		{
                    v = video_data[i];
                    if(v==0)continue;
			
		    x = xorg+addr_near_x[azicnt][i];
		    y = yorg-addr_near_y[azicnt][i];// note this is minus

		    if( isValid(x,y) )
		    {
                         //printf("x = %d,y = %d",x,y);
		        setFirstPixel( x, y, v);
		    }
		    else
		    {
		        //填充死区的地址表不是完全顺序的,一个越界地址的后面可能还有另一个不越界的地址
		        if(readytobreak==5)
		        {
		            break;
		        }
		        else
		            readytobreak++;
		    }
		}

        readytobreak = 0;
		for(  i=ADDR_NEAR_SMP_LEN;i<n;i++ )//画远区
		{
				v = video_data[i];
				if(v==0)continue;
		
		    x = xorg+addr_far_x[2*(azicnt)][i-ADDR_NEAR_SMP_LEN];
		    y = yorg-addr_far_y[2*(azicnt)][i-ADDR_NEAR_SMP_LEN];

		    if( isValid(x,y) )
		    {      
		        setFirstPixel( x, y, v);
		    }
		    else
		    {
		        if(readytobreak==10)
		        {
		            //printf("break when i=%d\n",i);
		            break;
		        }
		        else
		            readytobreak++;
		    }

		    x = xorg+addr_far_x[2*(azicnt)+1][i-ADDR_NEAR_SMP_LEN];
		    y = yorg-addr_far_y[2*(azicnt)+1][i-ADDR_NEAR_SMP_LEN];

		    if( isValid(x,y) )
		    {
		        setFirstPixel( x, y, v );
		    }
		    else
		    {
		        if(readytobreak==10)
		        {
		            //printf("break when i=%d\n",i);
		            break;
		        }
		        else
		            readytobreak++;
		    }
		}
	}
        
    else //<= 512
    {
        readytobreak = 0;
        for( i=0;i<dispRadius;i++ )//画近区
        {
            v = video_data[i];
            if(v==0)continue;

            x = xorg+addr_near_x[azicnt][i];
            y = yorg-addr_near_y[azicnt][i];
	
            if( isValid(x,y) )
            {
                
                setFirstPixel( x, y, v );
            }
            else
            {
                if(readytobreak==5)
                {
                    //printf("break when i=%d\n",i);
                    break;
                }
                else
                    readytobreak++;
            }
        }
    }
   
bool dowin;
	if(bWindowDisp)//bWindowDisp
	{
       
		dowin=false;
		if( win_astart < win_aend )
		{
		    if( azicnt>=win_astart-5 && azicnt<=win_aend+5 )dowin=true;
		}
		else
		{
		    if( azicnt>=win_aend-5 || azicnt<=win_astart+5 )dowin=true;
		}

        	

        if(dowin)
        {
		    if( isDivert() || moveMode==1)//偏心显示模式(真运动或偏心)
		    {
		        readytobreak = 0;
		        for(  i=0;i<ADDR_NEAR_SMP_LEN;i++ )//画近区
		        {
						/*if(video_data[p]==0 && video_data[p+1]==0)
							continue;
						else*/
		

		            x = xorg+addr_near_x[azicnt][i];
		            y = yorg-addr_near_y[azicnt][i];// note this is minus
		            

		            if( isValid(x,y) )
		            {
		                winDraw(azicnt, x,y,i);
		            }
		            else
		            {
		            //填充死区的地址表不是完全顺序的,一个越界地址的后面可能还有另一个不越界的地址
		                if(readytobreak==5)
		                {
		                //printf("break when i=%d\n",i);
		                    break;
		                }
		                else
		                    readytobreak++;
		            }
		        }

		        readytobreak = 0;
		        for(  i=ADDR_NEAR_SMP_LEN;i<ADDR_FAR_SMP_LEN+ADDR_NEAR_SMP_LEN;i++ )//画远区
		        {
						/*if(video_data[p]==0 && video_data[p+1]==0)
							continue;
						else*/
		

		            x = xorg+addr_far_x[2*(azicnt)][i-ADDR_NEAR_SMP_LEN];
		            y = yorg-addr_far_y[2*(azicnt)][i-ADDR_NEAR_SMP_LEN];

		            if( isValid(x,y) )
		            {      
		                winDraw( azicnt, x, y, i);
		            }
		            else
		            {
		                if(readytobreak==10)
		                {
		                //printf("break when i=%d\n",i);
		                    break;
		                }
		                else
		                    readytobreak++;
		            }

		            x = xorg+addr_far_x[2*(azicnt)+1][i-ADDR_NEAR_SMP_LEN];
		            y = yorg-addr_far_y[2*(azicnt)+1][i-ADDR_NEAR_SMP_LEN];

		            if( isValid(x,y) )
		            {
		                winDraw( azicnt, x, y, i );
		            }
		            else
		            {
		                if(readytobreak==10)
		                {
		                //printf("break when i=%d\n",i);
		                    break;
		                }
		                else
		                    readytobreak++;
		            }
		        }
		        
		    }//if( isDivert() || moveMode==1)//偏心显示模式(真运动或偏心)
            else//正常显示模式
            {
                //printf("normal\n");

                readytobreak = 0;
                for(  i=0;i<dispRadius;i++ )//ADDR_NEAR_SMP_LEN//画近区
                {			
		 
                    x = xorg+addr_near_x[azicnt][i];//azicnt
                    y = yorg-addr_near_y[azicnt][i];//azicnt

                    if( isValid(x,y) )
                    {
                        winDraw( azicnt, x, y, i );
			
                    }
                    else
                    {
                        if(readytobreak==5)
                        {

                            break;
                        }
                        else
                            readytobreak++;
                    }
                }
            }
            
        }
    } //end window
}


void CPPI::winDraw(int azicnt, int x, int y, int p)// x,y is the position of the 4 pixels group's left-top point
{
	int vn,vf;
    if( x>= win_xstart && x<win_xstart+WIN_WIDTH/2 && y>=win_ystart && y<win_ystart+WIN_HEIGHT/2 )
    {
                x = (x-win_xstart)*2+WIN_XSTART;//获得开窗绘图区域的首点位置
                y = (y-win_ystart)*2+WIN_YSTART;
		vn=video_data[p];
		vf=(video_data[p]+video_data[p+1])/2;
                            
		if( azicnt>=511 && azicnt<1535 )
		{
			winSetFirstPixel( x, y, vn);
			winSetFirstPixel( x, y+1, vn);
			winSetFirstPixel( x+1, y, vf);
			winSetFirstPixel( x+1, y+1, vf);
		}

		else if( azicnt>=1535 && azicnt<2559 )
		{
			winSetFirstPixel( x, y, vn);
			winSetFirstPixel( x+1, y, vn);
			winSetFirstPixel( x, y+1, vf);
			winSetFirstPixel( x+1, y+1, vf);
		}
		else if( azicnt>=2559 && azicnt<3583 )
		{
			winSetFirstPixel( x+1, y, vn);
			winSetFirstPixel( x+1, y+1, vn);
			winSetFirstPixel( x, y, vf);
			winSetFirstPixel( x, y+1, vf);
		}
		else
		{
			winSetFirstPixel( x, y+1, vn);
			winSetFirstPixel( x+1, y+1, vn);
			winSetFirstPixel( x, y, vf);
			winSetFirstPixel( x+1, y, vf);
		}
    }
}


void CPPI::clear()
{
	int i,j;
	for(i=PPI_XSTART;i<PPI_WIDTH+PPI_XSTART;i++)
    	for(j=PPI_YSTART;j<PPI_HEIGHT+PPI_YSTART;j++)
      	{
        	layerPPI[j][i].first=0;
        	if(layerPPI[j][i].curlayer<2)
        	{
        		layerPPI[j][i].nfirst=PPIColorTable[0];
			//fbpPPI[j][i]=PPIColorTable[0];
			fb_putpixel_int( i,  j, PPIColorTable[0], fbpPPI);
        }
      }

	if(bWindowDisp){
		for(i=WIN_XSTART;i<WIN_WIDTH+WIN_XSTART;i++)
			for(j=WIN_YSTART;j<WIN_HEIGHT+WIN_YSTART;j++)
			{
				layerMINI[j][i].first=0;
				if(layerMINI[j][i].curlayer<2)
				{
					layerMINI[j][i].nfirst=WinColorTable[0];
						//fbpMINI[j][i]=WinColorTable[0];
 					fb_putpixel_int( i,  j, WinColorTable[0], fbpMINI);
				}
			}
	}

}


void CPPI::setWinOrigin(int x, int y) //设置窗口起始点
{
    int a,b,c,d;
    //   a______b
    //    |       |
    //    |____|
    //   d      c
    int e1,e2;
    
    if(x<0||y<0) return;
    complex t;
    if( x>=PPI_XSTART && x<PPI_XSTART+PPI_WIDTH && y>=PPI_YSTART && y<PPI_YSTART+PPI_HEIGHT )
		;
	else
		return;

    win_xstart = x;
    win_ystart = y;

    //printf("win_xstart =%d,x = %d,win_ystart=%d,y=%d.\n",win_xstart,x,win_ystart,y);
    
    t = complex(x-xorg,-(y-yorg));
    a = (int)floorf(t.getMyThetaDeg());
    
    t = complex(WIN_WIDTH/2+x-xorg,-(y-yorg));
    b = (int)floorf(t.getMyThetaDeg());
    
    t = complex(WIN_WIDTH/2+x-xorg,-(WIN_HEIGHT/2+y-yorg));
    c = (int)floorf(t.getMyThetaDeg());
    
    t = complex(x-xorg,-(WIN_HEIGHT/2+y-yorg));
    d = (int)floorf(t.getMyThetaDeg());
    
    if( a>=0 && a<=180 )//窗口全在右半面
    {
        e1 = a<=90 ? a : b;
        e2 = d<=90 ? c : d;
    }
    else if( b>=180 )//窗口全在左半面
    {
        e2 = b>=270 ? b : a;
        e1 = c>=270 ? d : c;
    }
    else// Y轴穿过窗口
    {
        if( WIN_HEIGHT/2+y <= yorg )//窗口在扫描中心上方
        {
            e2=c;
            e1=d;
        }
        else if( y >= yorg )//窗口在扫描中心下方
        {
            e1=b;
            e2=a;
        }
        else//扫描中心在窗口内
        {
            e1=0;
            e2=360;
        }
    }
    
    win_astart = (int)floorf(e1/AZIDIS);//转换为绘图方位
    win_aend  =  (int)floorf(e2/AZIDIS);
    
    //注意 ： 当窗口跨越0度绘图方位线时，win_aend < win_astart
    if( win_astart>=4096 )win_astart=4095;//处理临界问题
    if( win_aend>=4096 )win_aend=4095; 
  	if(scopeMode==0)setScopeMode(scopeMode);  
    
}



void CPPI::setScopeMode(unsigned char n)
{
	scopeMode=n;
	if(scopeMode==0)//B mode update param
	{
		if(win_aend-win_astart>2048)
			win_aziIndex=(win_astart+4096+win_aend)/2;
		else
			win_aziIndex=(win_astart+win_aend)/2;
		win_aziIndex %= 4096;
		//fprintf(debugfp,"win_aziIndex = %d \n",win_aziIndex);
		win_disIndex=sqrt((float)(((win_xstart+85)-xorg)*((win_xstart+85)-xorg)+((win_ystart+85)-yorg)*((win_ystart+85)-yorg)));
		//fprintf(debugfp,"win_disIndex = %d \n",win_disIndex);
	}
}
void CPPI::setFirstPixel(int x, int y, VIDEODATATYPE depth)
{

    if(isValid(x,y))
	{

		if(layerPPI[y][x].curlayer<2)
		{
			if(layerPPI[y][x].first<depth)//比大写入
				{
				layerPPI[y][x].nfirst=PPIColorTable[depth];
				layerPPI[y][x].first=depth;

				if(layerPPI[y][x].bmap&&layerPPI[y][x].first==0)
					{
                                       // printf("和地图疑惑!\n");
					//fbpPPI[y][x]=PPIColorTable[depth] ^ layerPPI[y][x].nmap;PPIColorTable[depth] ^
					fb_putpixel_int( x,  y,  layerPPI[y][x].nmap, fbpPPI);}
				else
					{
                                       // printf("和地图不疑惑!\n");
					//fbpPPI[y][x]=PPIColorTable[depth];
					fb_putpixel_int( x,  y, PPIColorTable[depth], fbpPPI);
					}			
				}
			else
				;
		}
		else if(layerPPI[y][x].curlayer>TOP)//top =4
		{

		}
		else
		{
			if(layerPPI[y][x].first<depth)
				{
				layerPPI[y][x].nfirst=PPIColorTable[depth];
				layerPPI[y][x].first=depth;
				}
		}
	//printf("setFirstPixel\n");
	}

//fb_putpixel_int( x,  y, PPIColorTable[depth], fbpPPI);

}
//-------- 加入小窗口颜色表，以便和PPI大窗相独立 --------
//--------------- 修改日期：2008-09-01 ----------------------

bool CPPI::winIsValid( int &x, int &y )
{
	 return ( (x>=WIN_XSTART) && (x<WIN_XSTART+WIN_WIDTH) && (y>=WIN_YSTART) && (y<WIN_YSTART+WIN_HEIGHT) );
}

void CPPI::winSetFirstPixel(int x, int y, VIDEODATATYPE depth)
{
	if(winIsValid(x,y))
	{
		if(layerMINI[y][x].curlayer<2)
		{
			if(layerMINI[y][x].first<depth)//比大写入
			{
				layerMINI[y][x].nfirst=WinColorTable[depth];
				layerMINI[y][x].first=depth;

				if(layerMINI[y][x].bmap)
					//fbpMINI[y][x]=WinColorTable[depth] ^ layerMINI[y][x].nmap;
	fb_putpixel_int( x,  y, WinColorTable[depth] ^ layerMINI[y][x].nmap, fbpMINI);
				else
					//fbpMINI[y][x]=WinColorTable[depth];
	fb_putpixel_int( x,  y, WinColorTable[depth], fbpMINI);
			}
			else
				;
		}
		else if(layerMINI[y][x].curlayer>TOP)
		{

		}
		else
		{
			if(layerMINI[y][x].first<depth)
			{
				layerMINI[y][x].nfirst=WinColorTable[depth];
				layerMINI[y][x].first=depth;
			}
		}
	}
}





