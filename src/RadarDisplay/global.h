#ifndef _GLOBAL_H_
#define _GLOBAL_H_
/*显示类型*/
enum  DisplayType{ 
TypePPI=0,TypeAR,TypeWIN,TypeTV,TypeOther
};
/*常量*/
const float AZIDIS = 0.08789;//360.0/4096.0;
const int TOP=4; //top layer number
const int SAMPLE_NUM=512*4;//一个视频量程合并后的采样点数
const int AZI_NUM=4096;//雷达旋转一周的角度计数

const int ADDR_NEAR_AZI_LEN=AZI_NUM;//PPI显示不偏心时的一周显示角度总数
const int ADDR_NEAR_SMP_LEN=SAMPLE_NUM/4;//PPI显示不偏心时的一根视频显示点数

const int ADDR_FAR_AZI_LEN=AZI_NUM*2;//PPI显示偏心时的一周显示角度总数
const int ADDR_FAR_SMP_LEN=SAMPLE_NUM/4;//PPI显示偏心时的一根视频显示点数

const int RND_LEN=1200*1200;//余辉随机数表的长度,等于作图区域的分辨率
/**/
typedef unsigned char VIDEODATATYPE;//雷达视频数据类型,位宽8, 256级灰度
typedef unsigned int DISPMEMTYPE;   //雷达视频数据显示类型,位宽32
typedef unsigned int MFB_COLORTYPE;

/*用于显示的雷达视频数据包，一条线的数据*/
const int max_azi_num_per_video = 24;
#define CNTVIDEO 4*512
//PPI显示数据接口定义
typedef struct s_ppi_video_for_disp
{
    //CNTVIDEO defined in ib_pkg.h, the num of video sapmle point
    VIDEODATATYPE videodata[CNTVIDEO];//原始视频数据
    int znum;//对应角度的个数
    int azicnt[max_azi_num_per_video];//(lsd) (方位代码)角度增量
}s_ppi_video_for_disp;



//每个像素点所需保留信息
typedef struct s_layer_unit
{
    DISPMEMTYPE nfirst;  //该像素点的一次雷达视频数据对应的颜色值
    DISPMEMTYPE nsecond;
    DISPMEMTYPE nthird;
    DISPMEMTYPE nmap;

    VIDEODATATYPE first;//一次视频
    
    //反映当前状态
    bool bsecond;//是否有二次信息
    bool bthird;
    bool bmap;//是否有地图信息，地图在一次之上，其他层之下，具有透明叠加功能
    int curlayer;//判断该像素点现在属于第几层
}s_layer_unit;

typedef struct s_disp_state
{
int northIndex;
int prowIndex;
int xorg;
int yorg;
float prowCorrect;//舰首校正值，单位：度
float distanceCorrect;//距离校正值，单位：米
	
unsigned char  gain;
int  range;// 0~300
int base;
unsigned char  dispMode;//0: 正北向上  1: 舰首向上  2: 航向向上
unsigned char moveMode; //0: 相对运动  1:真运动
unsigned char mileKilo;// mile:0  ; kilo : 11    //range + milekilo 对应常量数组索引
unsigned char dayNight;
unsigned char tailTime;
unsigned char cluterControl;//设置抗海浪杂波  0：自动 ； 其他：级数  其中 1 为不抑制
unsigned char rainSnowControl;//设置抗雨雪杂波  0：自动 ； 其他：级数  其中 1 为不抑制
unsigned char cofreqControl;//设置抗同频异步干扰： 0：关闭；1：打开

unsigned char vecType; //矢量类型: 0  真矢量；1  相对矢量
unsigned char vecTime;// 单位：分钟； 范围：1～60

bool enFirstInfo;
bool enDivert;
bool enSplitDisp;
bool enWindowDisp;
bool enProwLine;
bool enFixedScale;
bool enEchoWiden;
bool enCloseFlag;

unsigned char scopeMode;//1: PPI; 0: B; 2: E

}s_disp_state;

enum PPIScopeMode{
BMODE=0,PPIMODE,EMODE
};
typedef struct s_ship_state
{
float speed;
float sailingAzi;  //unit is degree
float prowAzi;

}s_ship_state;
//------------------------------------------------------------------------------
const int MAX_PPI_WIDTH = 1200;
const int MAX_PPI_HEIGHT = 1200;

const int MAX_MINIWIN_WIDTH = 1200;
const int MAX_MINIWIN_HEIGHT = 1200;

const int MAX_AR_WIDTH = 512;
const int MAX_AR_HEIGHT = 512;

//clear color
const unsigned int MFB_CLEAR=0x0;
const unsigned int MFB_RED = 0xff0000;
const unsigned int MFB_GREEN = 0x00ff00;
const unsigned int MFB_BLUE = 0x0000ff;
const unsigned int MFB_YELLOW =0xffff00;
const unsigned int MFB_WHITE = 0xffffff;
const unsigned int MFB_BLACK = 0x0;
// 加密数据文件的文件位置
const int datPos[10]={45075968,14149632,14149120,8388608,22538752,8389120,0,14150144,45076480,28298752};

#endif // _GLOBAL_H_
