#ifndef _FRAMEBUFFER_H_
#define _FRAMEBUFFER_H_



#ifndef MIN
#define MIN(x, y) ((x) < (y)) ? (x) : (y)
#define MAX(x, y) ((x) > (y)) ? (x) : (y)
#endif /* MIN */

#define TRUE  -1
#define FALSE  0

#define PI      3.141592653589792
#define LEN     255
#define sqr(x)  (x * x)
#define DFLT_FB "/dev/fb0"
#define DFLT_DEVFS_FB "/dev/fb/0"
#define WIDTH   fb_screen->vinfo->xres
#define HEIGHT  fb_screen->vinfo->yres
#define VWIDTH  fb_screen->vinfo->xres_virtual
#define VHEIGHT fb_screen->vinfo->yres_virtual


typedef struct fb_text {
        int color;
        int bgcolor;
        int mask;
        int tab_size;
} fb_text;

typedef struct fb_font {
        int height, width;
        unsigned char *data;
} fb_font;
typedef struct fb_surface {
	int x;
	int y;
	int width;
	int height;
	int pixel_size;// bpp/8
	int bpp;
	int line_size; // finfo->line_length;
	int buffer_size;
	unsigned long mem_offset;
	char *mem_pointer;
	unsigned long (*make_color)(int r, int g, int b);
	int red_size, red_offset;
	int green_size, green_offset;
	int blue_size, blue_offset;
	fb_text text;
	fb_font font;
	struct fb_fix_screeninfo *finfo;
	struct fb_var_screeninfo *vinfo;
} fb_surface;
extern fb_surface *fb_screen;//对应屏幕的整个显示区域
/*
	fb_surface* fb;
	ptr = (unsigned char *)(fb->mem_pointer + fb->mem_offset + y*fb->line_size + x*fb->pixel_size);
*/

//--------------------------------------------------------------------------------------------
int  fb_init();
void fb_cleanup();
void fb_handle_signals();
/*创建块页面，就是在源页面上的一块矩形区域*/
fb_surface* fb_create_sectionsurface(int x,int y,int w,int h,fb_surface *fb_s);
/*页面清黑，IsSec用来指示是否是区域*/
void   fb_clean_surface(fb_surface *surface,bool IsSec=false);
void   fb_destroy_surface(fb_surface *surface,bool IsSec=false);
fb_surface *fb_create_surface(int width, int height);
fb_surface *fb_create_surface_depth(int width, int height, int depth);
//--------------------------------------------------------------------------------------------
/*FbPixel.cpp*/
void fb_putpixel(int x, int y, int r, int g, int b, fb_surface *surface);
void fb_putpixel_int(int x, int y, unsigned int color, fb_surface *surface);
void fb_getpixel(int x, int y, int *r, int *g, int *b, fb_surface *surface);
void fb_getpixel_int(int x, int y,unsigned int *color,fb_surface *surface);
void fb_putpixel_alpha(int alpha, int x, int y, int r1, int g1, int b1, fb_surface *surface);
void fb_putpixel_aa(int alpha, float x, float y, int r, int g, int b, fb_surface *surface);
//--------------------------------------------------------------------------------------------
/*FbPPM.cpp*/
fb_surface* fb_load_ppm(char *filename);
void        fb_draw_image(fb_surface *dest,fb_surface *src);
int         fb_save_ppm(char *filename, fb_surface *surface, int grayscale);
//--------------------------------------------------------------------------------------------
/* FbLine.cpp */
void fb_line(int x1, int y1, int x2, int y2, int r, int g, int b, fb_surface *surface);

/* 
 * Description:	Clears the screen
 */
inline void fb_cls()
{
	fb_clean_surface(fb_screen);
}
#endif//_FRAMEBUFFER_H_

