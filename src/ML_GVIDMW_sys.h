

#ifndef __THU_6BE7D83F_50C1_4007__
#define __THU_6BE7D83F_50C1_4007__

//---------------------------------------------------------------------------
//-------------- 用于参数有效性判定的宏定义 --------------------------
// valid region(low up)
#define VR_CHECK(n,l,u) if(n<l || n>u)return -6;
// axis
#define XY_CHECK   if(!checkXY(theX,theY))return -3;
//if(theX<0 || theX>1599 || theY<0 || theY>2399)return -3;
// valid channel
#define VC_CHECK   if(VideoChannel!=1 && VideoChannel!=2)return -4;else VideoChannel--;
// is init
#define INIT_CHECK if(!bInit)return -5;//-3
#define INIT_PPI_CHECK if(!bInitPPI[VideoChannel-1])return -5;
#define INIT_AR_CHECK if(!bInitAR[VideoChannel-1])return -5;
#define INIT_MiniWin_CHECK if(!bInitMiniWin[VideoChannel-1])return -5;
#define INIT_TV_CHECK if(!bInitTV[VideoChannel-1])return -5;


#endif

