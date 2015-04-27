
//******************************
//
//  author : lishidan
//
//  create time: 2007-10-23
//
//  last modify time:2007-10-23
//
//  Version: 0.1
//
//******************************
#include "yh.h"
#include "debug.h"
CYh::CYh( CDataPool *dp )
{


    	m_dp = dp;
//-------------------------------------------------------------
    	randomx = m_dp->randomx;
    	randomy = m_dp->randomy;
//-------------------------------------------------------------
   	PPIColorTable = m_dp->PPIColorTable;
	WinColorTable=m_dp->WinColorTable;
//-------------------------------------------------------------
   	fbpPPI = m_dp->fbpPPI;
    	layerPPI = m_dp->layerPPI;
//-------------------------------------------------------------
	fbpMINI = m_dp->fbpMINI;
	layerMINI = m_dp->layerMINI;
//-------------------------------------------------------------
	bWin = m_dp->dispState.enWindowDisp;
//-------------------------------------------------------------
    	bYh = false;
    	yh_index = 0;
    	yhLen=YH_NUM;
	updateRegion();
    	setYhWeight( 60 );
}
void CYh::UpdateYHDispaly()
{
	updateFbp();
	updateRegion();
}
CYh::~CYh()
{
    TRACE("CYh::~CYh()");
}

void CYh::setYhWeight( int yhmul )
{
	yhDecWeight = yhmul;

	updateTransTable();
}

void CYh::updateTransTable()
{
	int v;
	for(int n=0;n<256;n++)
	{
		v=n;
		v -= yhDecWeight;
		v = v<0 ? 0 : v;
		transTable[n]=v & 0x0ff;
	}
}

void CYh::yh()
{
    int x,y;

    if(bYh)
    {
        for(int i=0;i<yhLen;i++)
        {
            x = randomx[ (yh_index+i) ];
            y = randomy[ (yh_index+i) ]; //调整余晖位置

            if((x<PPI_WIDTH)&&(y<PPI_HEIGHT))
            {
				x += PPI_XSTART;
				y += PPI_YSTART; 

				//if(maskTable[y][x]>0)goto winyh;

				if (layerPPI[y][x].first == 0)goto winyh;


                layerPPI[y][x].first = transTable[layerPPI[y][x].first];
                
                if(layerPPI[y][x].curlayer<2)
                {
                    layerPPI[y][x].nfirst=PPIColorTable[layerPPI[y][x].first];
                    //fbpPPI[y][x] = layerPPI[y][x].nfirst;

					if(layerPPI[y][x].bmap)
						//fbpPPI[y][x]=layerPPI[y][x].nfirst ^ layerPPI[y][x].nmap;
						fb_putpixel_int( x,  y, layerPPI[y][x].nfirst ^ layerPPI[y][x].nmap, fbpPPI);
					else
						//fbpPPI[y][x]=layerPPI[y][x].nfirst;
						fb_putpixel_int( x,  y, layerPPI[y][x].nfirst, fbpPPI);
                }

		winyh:
				x -= PPI_XSTART;
				y -= PPI_YSTART;//调整余晖位置
			}


			if((x<WIN_WIDTH)&&(y<WIN_HEIGHT))
			{
				if(!bWin)continue;

                    x += WIN_XSTART;
                    y += WIN_YSTART;

					//if(maskTable[y][x]>0)continue;

					if (layerMINI[y][x].first == 0)continue;

                    layerMINI[y][x].first = transTable[layerMINI[y][x].first];
                    
                    if(layerMINI[y][x].curlayer<2)
                    {
                        layerMINI[y][x].nfirst=WinColorTable[layerMINI[y][x].first];
                        //fbpMINI[y][x] = layerMINI[y][x].nfirst;
			fb_putpixel_int( x,  y, layerPPI[y][x].nfirst, fbpMINI);
                    }
            }
        }

        yh_index = yh_index+yhLen;
		if(yh_index>=RND_LEN)yh_index=0;
    }
}

void CYh::updateRegion(void)
{
	PPI_XSTART=m_dp->PPI_XSTART;
	PPI_YSTART=m_dp->PPI_YSTART;
	PPI_WIDTH=m_dp->PPI_WIDTH;
	PPI_HEIGHT=m_dp->PPI_HEIGHT;

	WIN_XSTART=m_dp->WIN_XSTART;
	WIN_YSTART=m_dp->WIN_YSTART;
	WIN_WIDTH=m_dp->WIN_WIDTH;
	WIN_HEIGHT=m_dp->WIN_HEIGHT;

	AR_XSTART=m_dp->AR_XSTART;
	AR_YSTART=m_dp->AR_YSTART;
	AR_WIDTH=m_dp->AR_WIDTH;
	AR_HEIGHT=m_dp->AR_HEIGHT;
}


void CYh::updateFbp(void)//更新绘图显存区地址
{
	fbpPPI=m_dp->fbpPPI;
	fbpMINI=m_dp->fbpMINI;
}

