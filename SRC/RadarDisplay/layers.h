#ifndef _CLayers_h__
#define _CLayers_h__


#include "global.h"
#include "../FrameBuffer/FrameBuffer.h"

class CLayerBase
{
public:
  CLayerBase(fb_surface*  mfbp , s_layer_unit ** mlayer ,  int l, bool d)
			{fbp=mfbp;layer=mlayer;ly=l;isd=d;}
	virtual ~CLayerBase(){}

public:
	fb_surface* fbp;
	s_layer_unit ** layer;
    	int ly;
    	bool isd;
    
    	inline bool isValid(int x,int y);
   	 void setPixel(int x,int y,DISPMEMTYPE d );
    	void clear(void);
	void setFbp(fb_surface* fbpnew){fbp=fbpnew;}

};

#endif
