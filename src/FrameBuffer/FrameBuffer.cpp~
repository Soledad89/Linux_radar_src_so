#include  <stdlib.h> //gedtdev setdev函数
#include <linux/fb.h>
#include <sys/ioctl.h> //ioctl 函数

#include <unistd.h>  //open
#include <fcntl.h>   //O_RDWR

#include <sys/mman.h>

#include <signal.h>
#include <sys/user.h>//PAGE_MASK
#include "FrameBuffer.h"
#include "FrameBufferColor.h"
#include "FbAsm.h"

int fbfd, fbptr_off;
fb_surface *fb_screen = NULL;
struct fb_var_screeninfo old_vinfo;
char *fbptr, *fb_device = NULL;
int fb_setdev(char *setdev)
{
	if (!setdev || setdev == NULL) 
		return -1;
	if (access(setdev, F_OK) < 0)
		return -1;

	fb_device = setdev;
	return 0;
}

/*
 * Function:    fb_finddev()
 * Arguments:	None
 * Returns:     Full path to framebuffer device
 * Description: Looks for the location of the framebuffer device
 * 		in the FRAMEBUFFER environment variable, and the
 * 		relative path in /dev (DFLT_FB).
 */
char *fb_finddev()
{
	char *dev;

	if (fb_device != NULL)
		return fb_device;

	if ((dev = getenv("FRAMEBUFFER")) == NULL) {
		if (access(DFLT_FB,F_OK) == 0) {
			dev = DFLT_FB;
			setenv("FRAMEBUFFER", DFLT_FB, 1);
		} else if (access(DFLT_DEVFS_FB, F_OK) == 0) {
			dev = DFLT_DEVFS_FB;
			setenv("FRAMEBUFFER", DFLT_DEVFS_FB, 1);
		}
	}

	return dev;
}

/*
 * Function:	fb_init()
 * Arguments:	None
 * Returns:	An fb_surface structure representing the screen.
 * Description:	Locates and initializes the fb device.
 */
int fb_init()
{
	char *dev, errstr[LEN];
	
	if (fb_screen == NULL) {
		fb_screen         = (fb_surface*)malloc(sizeof(fb_surface));
		fb_screen->vinfo  = (struct fb_var_screeninfo *)malloc(sizeof(struct fb_var_screeninfo));
		fb_screen->finfo  = (struct fb_fix_screeninfo *)malloc(sizeof(struct fb_fix_screeninfo));
	} else {
		/* Screen is already initialized */
		return 1;
	}

	if ((dev = fb_finddev()) == NULL) {
		fprintf(stderr,"Cannot open %s, make sure it exists.\n",
			DFLT_FB);
		exit(1);
	}

	if ((fbfd = open(dev, O_RDWR)) < 0) {
		snprintf(errstr, LEN, "Error opening %s", dev);
		perror(errstr);
		exit(1);
	}

	if (ioctl(fbfd, FBIOGET_FSCREENINFO, fb_screen->finfo) < 0) {
		perror("Error reading fixed information");
		exit(1);
	}

	if (ioctl(fbfd, FBIOGET_VSCREENINFO, fb_screen->vinfo) < 0) {
		perror("Error reading variable information");
		exit(1);
	}
	
	printf("The mem is :%d bit.\n",fb_screen->finfo->smem_len);
	printf("The line_length is :%d.\n",fb_screen->finfo->line_length);
	printf("The xres is :%d.\n", fb_screen->vinfo->xres);
	printf("The yres is :%d.\n", fb_screen->vinfo->yres);
	printf("bits_per_pixel is :%d.\n", fb_screen->vinfo->bits_per_pixel);
	printf("fb_screen->finfo->id is :%s.\n", fb_screen->finfo->id);
		
	fbptr = (char *)mmap(0, fb_screen->finfo->smem_len, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);
	fbptr_off = (unsigned long)fb_screen->finfo->smem_start & ~PAGE_MASK;
	fb_screen->width  = fb_screen->vinfo->xres;
	fb_screen->height = fb_screen->vinfo->yres;


	switch (fb_screen->vinfo->bits_per_pixel) {
		case 8:
			fb_screen->pixel_size = 1;
			fb_screen->make_color = fb_make_color_8;
			break;
		case 15:
			fb_screen->pixel_size          = 2;
			fb_screen->make_color          = fb_make_color_15;
			fb_screen->vinfo->red.offset   = 10;
			fb_screen->vinfo->red.length   = 5;
			fb_screen->vinfo->green.offset = 5;
			fb_screen->vinfo->green.length = 5;
			fb_screen->vinfo->blue.offset  = 0;
			fb_screen->vinfo->blue.length  = 5;
			break;
		case 16:
			fb_screen->pixel_size          = 2;
			fb_screen->make_color          = fb_make_color_16;
			fb_screen->vinfo->red.offset   = 11;
			fb_screen->vinfo->red.length   = 5;
			fb_screen->vinfo->green.offset = 5;
			fb_screen->vinfo->green.length = 6;
			fb_screen->vinfo->blue.offset  = 0;
			fb_screen->vinfo->blue.length  = 5;
			break;
		case 24:
			fb_screen->pixel_size          = 3;
			fb_screen->make_color          = fb_make_color_24;
			fb_screen->vinfo->red.offset   = 16;
			fb_screen->vinfo->red.length   = 8;
			fb_screen->vinfo->green.offset = 8;
			fb_screen->vinfo->green.length = 8;
			fb_screen->vinfo->blue.offset  = 0;
			fb_screen->vinfo->blue.length  = 8;
			break;
		case 32:
			fb_screen->pixel_size          = 4;
			fb_screen->make_color          = fb_make_color_24;
			fb_screen->vinfo->red.offset   = 16;
			fb_screen->vinfo->red.length   = 8;
			fb_screen->vinfo->green.offset = 8;
			fb_screen->vinfo->green.length = 8;
			fb_screen->vinfo->blue.offset  = 0;
			fb_screen->vinfo->blue.length  = 8;
			break;
		default:
			fprintf(stderr, "Unknown color depth %d.\n"
					"Attempting to generate color.\n", 
					fb_screen->vinfo->bits_per_pixel);

			fb_screen->pixel_size = 1;
			fb_screen->make_color = fb_make_color_unknown;
			break;
	}
	
	if (ioctl(fbfd, FBIOPUT_VSCREENINFO, fb_screen->vinfo) < 0) {
		perror("Couldn't set ideal mode at FBIOPUT_VSCREENINFO");
		exit(1);
	}
	
	if (ioctl(fbfd, FBIOGET_VSCREENINFO, fb_screen->vinfo) < 0) {
		perror("ioctl FBIOGET_VSCREENINFO");
		fb_cleanup();
		exit(1);
	}

	fb_screen->bpp         = fb_screen->vinfo->bits_per_pixel;
	fb_screen->line_size   = fb_screen->finfo->line_length;
	fb_screen->buffer_size = fb_screen->finfo->smem_len;
	fb_screen->mem_offset  = fbptr_off;
	fb_screen->mem_pointer = fbptr;
	
	fb_screen->red_size     = fb_screen->vinfo->red.length;
	fb_screen->red_offset   = fb_screen->vinfo->red.offset;
	fb_screen->green_size   = fb_screen->vinfo->green.length;
	fb_screen->green_offset = fb_screen->vinfo->green.offset;
	fb_screen->blue_size    = fb_screen->vinfo->blue.length;
	fb_screen->blue_offset  = fb_screen->vinfo->blue.offset;
		
	fb_screen->text.tab_size = 8;
	fb_screen->font.width    = 8;
	fb_screen->x = 0;
	fb_screen->y = 0;

	fb_set_textcolor(fb_screen->make_color(0xff, 0xff, 0xff), 0x00);

	fb_handle_signals();

	return 0;
}

fb_surface* fb_create_sectionsurface(int x,int y,int w,int h,fb_surface *fb_s)
{
	fb_surface *new_surface = (fb_surface *)malloc(sizeof(fb_surface));
	int d = 0;
	int s = 0;
	int rl = 0, ro = 0;
	int gl = 0, go = 0;
	int bl = 0, bo = 0;
	unsigned long (*make_color)(int r, int g, int b);
	
	switch(fb_s->bpp) {
		case 8:
			d = 8;
			s = 1;
			rl = 3;
			gl = 3;
			bl = 2;
			ro = 5;
			go = 2;
			bo = 0;
			make_color = fb_make_color_8;
			break;
		case 15:
			d = 15;
			s = 2;
			rl = 5;
			gl = 5;
			bl = 5;
			ro = 10;
			go = 5;
			bo = 0;
			make_color = fb_make_color_15;
			break;
		case 16:
			d = 16;
			s = 2;
			rl = 5;
			gl = 6;
			bl = 5;
			ro = 11;
			go = 5;
			bo = 0;
			make_color = fb_make_color_16;
			break;
		case 24:
			d = 24;
			s = 3;
			rl = 8;
			gl = 8;
			bl = 8;
			ro = 16;
			go = 8;
			bo = 0;
			make_color = fb_make_color_24;
			break;
		case 32:
			d = 32;
			s = 4;
			rl = 8;
			gl = 8;
			bl = 8;
			ro = 16;
			go = 8;
			bo = 0;
			make_color = fb_make_color_24;
			break;
		default:
			/* Unsupported depth */
			return NULL;
	}
  char *_ptr = (char *)(fb_s->mem_pointer + fb_s->mem_offset + y*fb_s->line_size + x*fb_s->pixel_size);
	if (new_surface != NULL) {
		new_surface->finfo        = (struct fb_fix_screeninfo *)malloc(sizeof(struct fb_fix_screeninfo));
		new_surface->vinfo        = (struct fb_var_screeninfo *)malloc(sizeof(struct fb_var_screeninfo));
		new_surface->x = x;
		new_surface->y = y;
		new_surface->vinfo->xres  = w;
		new_surface->vinfo->yres  = h;
		new_surface->width        = w;
		new_surface->height       = h;
		new_surface->pixel_size   = s;
		new_surface->bpp          = d;
		new_surface->line_size    = fb_s->line_size;//width * s;
		new_surface->buffer_size  = w * h * s;
		new_surface->mem_offset   = 0;
		new_surface->mem_pointer  = _ptr;
		new_surface->make_color   = make_color;
		new_surface->red_size     = rl;
		new_surface->red_offset   = ro;
		new_surface->green_size   = gl;
		new_surface->green_offset = go;
		new_surface->blue_size    = bl;
		new_surface->blue_offset  = bo;
		new_surface->text.color   = make_color(255, 255, 255);
		new_surface->text.mask	  = FALSE;
		new_surface->font	  = fb_screen->font;
	}

	return new_surface;

}
fb_surface *fb_create_surface(int width, int height)
{
	return fb_create_surface_depth(width, height, fb_screen->bpp);
}

/* 
* Function:	fb_create_surface_depth()
* Arguments:	width, height, and color depth of surface
* Returns:	A pointer to the newly created surface
* Description:	Creates a new fb_surface in system memory 
* 		with the specified width, height, and depth.
*/
fb_surface *fb_create_surface_depth(int width, int height, int depth)
{
	char *new_bitmap = (char *)malloc(width * height * fb_screen->pixel_size);
	fb_surface *new_surface = (fb_surface *)malloc(sizeof(fb_surface));
	int d = 0;
	int s = 0;
	int rl = 0, ro = 0;
	int gl = 0, go = 0;
	int bl = 0, bo = 0;
	unsigned long (*make_color)(int r, int g, int b);
	
	switch(depth) {
		case 8:
			d = 8;
			s = 1;
			rl = 3;
			gl = 3;
			bl = 2;
			ro = 5;
			go = 2;
			bo = 0;
			make_color = fb_make_color_8;
			break;
		case 15:
			d = 15;
			s = 2;
			rl = 5;
			gl = 5;
			bl = 5;
			ro = 10;
			go = 5;
			bo = 0;
			make_color = fb_make_color_15;
			break;
		case 16:
			d = 16;
			s = 2;
			rl = 5;
			gl = 6;
			bl = 5;
			ro = 11;
			go = 5;
			bo = 0;
			make_color = fb_make_color_16;
			break;
		case 24:
			d = 24;
			s = 3;
			rl = 8;
			gl = 8;
			bl = 8;
			ro = 16;
			go = 8;
			bo = 0;
			make_color = fb_make_color_24;
			break;
		case 32:
			d = 32;
			s = 4;
			rl = 8;
			gl = 8;
			bl = 8;
			ro = 16;
			go = 8;
			bo = 0;
			make_color = fb_make_color_24;
			break;
		default:
			/* Unsupported depth */
			return NULL;
	}
	
	if (new_surface != NULL && new_bitmap != NULL) {
		new_surface->finfo        = (struct fb_fix_screeninfo *)malloc(sizeof(struct fb_fix_screeninfo));
		new_surface->vinfo        = (struct fb_var_screeninfo *)malloc(sizeof(struct fb_var_screeninfo));
		new_surface->x = 0;
		new_surface->y = 0;
		new_surface->vinfo->xres  = width;
		new_surface->vinfo->yres  = height;
		new_surface->width        = width;
		new_surface->height       = height;
		new_surface->pixel_size   = s;
		new_surface->bpp          = d;
		new_surface->line_size    = width * s;
		new_surface->buffer_size  = width * height * s;
		new_surface->mem_offset   = 0;
		new_surface->mem_pointer  = new_bitmap;
		new_surface->make_color   = make_color;
		new_surface->red_size     = rl;
		new_surface->red_offset   = ro;
		new_surface->green_size   = gl;
		new_surface->green_offset = go;
		new_surface->blue_size    = bl;
		new_surface->blue_offset  = bo;
		new_surface->text.color   = make_color(255, 255, 255);
		new_surface->text.mask	  = FALSE;
		new_surface->font	  = fb_screen->font;
	}

	return new_surface;
}
void fb_clean_surface(fb_surface *surface,bool IsSec)
{
	char* _ptr = surface->mem_pointer+surface->mem_offset;
	if (surface != NULL && _ptr!= NULL)
	{  
	      	if(IsSec == false)
		fb_memset((void *)(surface->mem_pointer + 
					   surface->mem_offset), 
					   0, surface->buffer_size);
		else
		   {
			int i =0;
			int w = surface->width;
			int h = surface->height;
			for(i = 0;i <h;i++)
					{
					_ptr  +=surface->line_size;
					fb_memset((void *)(_ptr),0, w*surface->pixel_size ); 							   					
					}
			}
	}
}

void fb_destroy_surface(fb_surface *surface,bool IsSec)
{
	if (surface != NULL) {
		if (surface->mem_pointer != NULL&&!IsSec)
			free(surface->mem_pointer);
		if (surface->finfo != NULL)
			free(surface->finfo);
		if (surface->vinfo != NULL)
			free(surface->vinfo);
		free(surface);
	}
}
/*
 * Function:    fb_cleanup()
 * Arguments:	None
 * Returns:     None
 * Description: unmaps memory and closes the
 *              file descriptor.
 */
void fb_cleanup()
{
	if (fb_screen != NULL) {
//		if (ioctl(fbfd, FBIOPUT_VSCREENINFO, &old_vinfo) < 0)
//			perror("ioctl FBIOPUT_VSCREENINFO");

//		if (ioctl(fbfd, FBIOGET_FSCREENINFO, fb_screen->finfo) < 0)
//			perror("ioctl FBIOGET_FSCREENINFO");
		
		munmap(fb_screen->mem_pointer, 
		       fb_screen->buffer_size);
		fbptr = NULL;
		fbptr_off = (int)NULL; /* shut up gcc */
		close(fbfd);
		free(fb_screen->finfo);
		free(fb_screen->vinfo);
		free(fb_screen);
	}
}
//------------------------------------------------------------------------------
void (*old_handlers[2])(int sig);
/*
 * Function:    quit()
 * Arguments:   signal number
 * Returns:     None
 * Description: Function for use with signal() to
 *              exit cleanly out of fb land.
 */
void quit(int signum)
{
	fb_cleanup();
        fprintf(stderr,"Recieved fatal signal %d, exiting...\n", signum);
        exit(1);
}

/*
 * Function:    fb_handle_signals()
 * Arguments:   None
 * Returns:     None
 * Description: Handles any signals we recieve.
 */
void fb_handle_signals()
{
        int i, signals[] = { SIGINT, SIGSEGV };

        for (i = 0; i < (int)(sizeof(signals) / sizeof(int)); i++)
                if ((old_handlers[i] = signal(signals[i], quit)) == SIG_ERR) {
                        fprintf(stderr,"Unable to set handler "
                                       "for signal %d.\n",
                                       signals[i]);

                        exit(1);
                }
}
