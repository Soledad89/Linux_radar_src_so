#ifndef _FRAMEBUFFER_COLOR_H_
#define _FRAMEBUFFER_COLOR_H_
#include "FrameBuffer.h"
unsigned long fb_make_color_8(int r, int g, int b);
unsigned long fb_make_color_15(int r, int g, int b);
unsigned long fb_make_color_16(int r, int g, int b);
unsigned long fb_make_color_24(int r, int g, int b);
unsigned long fb_make_color_unknown(int r, int g, int b);
void fb_get_color(unsigned int hwcolor, int *r, int *g, int *b);
void fb_get_textcolor(int *fg, int *bg);
void fb_sget_textcolor(fb_surface *surface, int *fg, int *bg);
void fb_set_textcolor(int fg, int bg);
void fb_sset_textcolor(fb_surface *surface, int fg, int bg);
#endif//_FRAMEBUFFER_COLOR_H_
