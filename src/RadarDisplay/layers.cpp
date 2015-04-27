#include "layers.h"

bool CLayerBase::isValid(int x,int y)
{
 if( (x>=0) && (x<MAX_PPI_WIDTH) && (y>=0) && (y<MAX_PPI_HEIGHT) )
	 return true;

 return false;

}

void CLayerBase::clear(void)
{
  int x,y;
  for(y=0;y<MAX_PPI_HEIGHT;y++)
    for(x=0;x<MAX_PPI_WIDTH;x++)
      setPixel(x,y,MFB_CLEAR);
}
//#include"stdio.h"
void CLayerBase::setPixel(int x,int y,DISPMEMTYPE d )
{
	//printf("x=%d,y=%d,ly=%d,isd=%d.\n",x,y,ly,isd);
  if(isValid(x,y))
  {
    switch(ly)
    {
    case 1:
        if(isd)//画图
        {
          if(layer[y][x].curlayer<2)
          {
            layer[y][x].nfirst=d;
            layer[y][x].curlayer=1;

			if(layer[y][x].bmap)
//				fbp[y][x]=d ^ layer[y][x].nmap;
				fb_putpixel_int( x,  y, d ^ layer[y][x].nmap, fbp);				
			else
//				fbp[y][x]=d;
				fb_putpixel_int( x,  y, d, fbp);	

          }
          else if(layer[y][x].curlayer>TOP)
          {

          }
          else
          {
            layer[y][x].nfirst=d;
          }
        }
        else//恢复
        {
          if(layer[y][x].curlayer<2)
          {   
            layer[y][x].nfirst=MFB_CLEAR;

			if(layer[y][x].bmap)
//				fbp[y][x]=layer[y][x].nmap;
				fb_putpixel_int( x,  y, layer[y][x].nmap, fbp);
			else
				//fbp[y][x]=MFB_CLEAR;
				fb_putpixel_int( x,  y, MFB_CLEAR, fbp);
          }
          else if(layer[y][x].curlayer>TOP)
          {
          }
          else
          {
            layer[y][x].nfirst=MFB_CLEAR;
          }
        }
        
        break;
    case 2://－－－－－－－－聚标环－－－－－－－－－－
        if(isd)//画图
        {
          if(layer[y][x].curlayer<3)
          {
            layer[y][x].nsecond=d;
            layer[y][x].bsecond=true;
            layer[y][x].curlayer=2;
            //fbp[y][x]=d;
	   fb_putpixel_int( x,  y, d, fbp);

          }
          else if(layer[y][x].curlayer>TOP)
          {

          }
          else
          {
            layer[y][x].nsecond=d;
            layer[y][x].bsecond=true;
          }
	//printf("ok!\n");
        }
        else//恢复
        {
          layer[y][x].bsecond=false;
        
          if(layer[y][x].curlayer<3)
          {
           // fbp[y][x]=layer[y][x].nfirst;
	    fb_putpixel_int( x,  y, layer[y][x].nfirst, fbp);
            layer[y][x].curlayer=1;
          }
        }
        break;
    case 3:
        if(isd)//画图
        {
          if(layer[y][x].curlayer<4)
          {
            layer[y][x].nthird=d;
            layer[y][x].bthird=true;
            layer[y][x].curlayer=3;
//            fbp[y][x]=d;
		fb_putpixel_int( x,  y, d, fbp);
          }
          else if(layer[y][x].curlayer>TOP)
          {

          }
          else
          {
            layer[y][x].nthird=d;
            layer[y][x].bthird=true;
          }
        }
        else//恢复
        {
          layer[y][x].bthird=false;

          if(layer[y][x].curlayer<4)
          {
            if(layer[y][x].bsecond)
            {
              //fbp[y][x]=layer[y][x].nsecond;
		fb_putpixel_int( x,  y, layer[y][x].nsecond, fbp);
              layer[y][x].curlayer=2;
            }
            else
            {
              //fbp[y][x]=layer[y][x].nfirst;
		fb_putpixel_int( x,  y, layer[y][x].nfirst, fbp);
              layer[y][x].curlayer=1;
            }
          }
        }
        break;
    case 4:
        if(isd)//画图
        {
          if(layer[y][x].curlayer<5)
          {
            layer[y][x].curlayer=4;
            //fbp[y][x]=d;
	fb_putpixel_int( x,  y, d, fbp);
          }
          else
          {
        
          }
        }
        else//恢复
        {
			if(layer[y][x].curlayer > TOP)
			{
				//fbp[y][x]=MFB_CLEAR;
				fb_putpixel_int( x,  y, MFB_CLEAR, fbp);
			}
          if(layer[y][x].curlayer<5)
          {
            if(layer[y][x].bthird)
            {
              //fbp[y][x]=layer[y][x].nthird;
		fb_putpixel_int( x,  y, layer[y][x].nthird, fbp);
              layer[y][x].curlayer=3;
            }
            else if(layer[y][x].bsecond)
            {
              //fbp[y][x]=layer[y][x].nsecond;
		fb_putpixel_int( x,  y, layer[y][x].nsecond, fbp);
              layer[y][x].curlayer=2;
            }
            else
            {
              //fbp[y][x]=layer[y][x].nfirst;
		fb_putpixel_int( x,  y,layer[y][x].nfirst, fbp);
              layer[y][x].curlayer=1;
            }
          }
        }
        break;
        
    case 0:
        break;

	case 0x10:// map layer
		if(isd)//画图
		{
			if(layer[y][x].curlayer<2)
			{
				layer[y][x].nmap=d;
				layer[y][x].curlayer=1;
				layer[y][x].bmap=true;
				//fbp[y][x]=d ^ layer[y][x].nfirst;//暂时用异或，以后可用查找表
				fb_putpixel_int( x,  y,d ^ layer[y][x].nfirst, fbp);
			}
			else if(layer[y][x].curlayer>TOP)
			{

			}
			else
			{
				layer[y][x].nmap=d;
				layer[y][x].bmap=true;
			}
		}
		else//恢复
		{
			if(layer[y][x].curlayer<2)
			{   
				layer[y][x].bmap=false;
				//fbp[y][x]=layer[y][x].nfirst;
				fb_putpixel_int( x,  y,layer[y][x].nfirst, fbp);
			}
			else if(layer[y][x].curlayer>TOP)
			{
			}
			else
			{
				layer[y][x].bmap=false;
			}
		}

		break;
        
    default:
        if(isd)//画图
        {
            if(layer[y][x].curlayer > TOP)
            {
                //fbp[y][x]=d;
		fb_putpixel_int( x,  y,d, fbp);
            }
        }
        else//恢复
        {
            if(layer[y][x].curlayer > TOP)
            {
                //fbp[y][x]=MFB_CLEAR;
		fb_putpixel_int( x,  y,MFB_CLEAR, fbp);
            }
        }

    }
  }

}
