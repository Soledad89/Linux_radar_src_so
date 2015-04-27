#include <stdio.h>
#include <math.h>
#include "FrameBuffer.h"
/*
 * Function:	fb_putpixel()
 * Arguments:	x and y positioners, rgb values, and pointer to surface
 * Returns:	None
 * Description: Draws a pixel to the surface at x, y position with rgb
 * 		value.
 */
void fb_putpixel(int x, int y, int r, int g, int b, fb_surface *surface)
{
	register unsigned char *ptr_char;
	register unsigned short *ptr_short;
	register unsigned int *ptr_int;
	unsigned int c = 0;

ptr_int  = (unsigned int *)(surface->mem_pointer + 
				 surface->mem_offset + 
				 y * surface->line_size + 
				 x * surface->pixel_size);

ptr_short = (unsigned short *)ptr_int;	
ptr_char  = (unsigned char *)ptr_short;


	if (x >= surface->width || x < 0 || 
            y >= surface->height || y < 0)
		return;

	switch(surface->bpp) {
		case 8:
			c = surface->make_color(r, g, b);
			*ptr_char = c & 0xFF;
		case 15:
		case 16:
			c = surface->make_color(r, g, b);
			*ptr_short = c & 0xFFFF;
			return;
		case 24:
			ptr_int = (unsigned int *)(ptr_char - 1);
			
			r >>= 8 - surface->red_size;
			g >>= 8 - surface->green_size;
			b >>= 8 - surface->blue_size;
			c = fb_screen->make_color(r, g, b);
			
			*(ptr_char + 2) = (c >> 16) & 0xFF;
			*(ptr_char + 1) = (c >> 8) & 0xFF;
			*(ptr_char + 0) = c & 0xFF;
			
			return;
		case 32:
			c = surface->make_color(r, g, b);
			*(ptr_int) = c;
			return;
		default:
			r >>= 8 - surface->red_size;
			g >>= 8 - surface->green_size;
			b >>= 8 - surface->blue_size;
			c = r << surface->red_offset | 
			    g << surface->green_offset | 
			    b << surface->blue_offset;
	}

}
/*
 * Function:	fb_putpixel()
 * Arguments:	x and y positioners, rgb values, and pointer to surface
 * Returns:	None
 * Description: Draws a pixel to the surface at x, y position with rgb
 * 		value.
 */
void fb_putpixel_int(int x, int y,unsigned int color, fb_surface *surface)
{
	register unsigned char *ptr_char;
	register unsigned short *ptr_short;
	register unsigned int *ptr_int;
	unsigned int c = color;

	ptr_int  = (unsigned int *)(surface->mem_pointer + 
					 surface->mem_offset + 
					 y * surface->line_size + 
					 x * surface->pixel_size);

	ptr_short = (unsigned short *)ptr_int;	
	ptr_char  = (unsigned char *)ptr_short;


	if (x >= surface->width || x < 0 || 
            y >= surface->height || y < 0)
		return;

	switch(surface->bpp) {
		case 8:
			*ptr_char = c & 0xFF;
		case 15:
		case 16:
			*ptr_short = c & 0xFFFF;
			return;
		case 24:			
			*(ptr_char + 2) = (c >> 16) & 0xFF;
			*(ptr_char + 1) = (c >> 8) & 0xFF;
			*(ptr_char + 0) = c & 0xFF;
			
			return;
		case 32:
			*(ptr_int) = c;
			return;
	}

}


/*
 * Function:	fb_getpixel()
 * Arguments:	x, y positioners, rgb values, pointer to surface
 * Returns:	None
 * Description:	Gets a pixel on surface at specified x, y position
 */
void fb_getpixel_int(int x, int y, 
			       unsigned int *color, 
			       fb_surface *surface)
{
	register unsigned int *ptr;
	ptr = (unsigned int *)(surface->mem_pointer + 
                               surface->mem_offset + 
                               y * surface->line_size + 
                               x * surface->pixel_size);

	if (x >= surface->width || x < 0 || 
            y >= surface->height || y < 0) {
		*color = 0;
		return;
	}

	*color = *ptr;
}

void fb_getpixel(int x, int y, 
			       int *r, int *g, int *b, 
			       fb_surface *surface)
{
	register unsigned int c, *ptr;
	ptr = (unsigned int *)(surface->mem_pointer + 
                               surface->mem_offset + 
                               y * surface->line_size + 
                               x * surface->pixel_size);

	if (x >= surface->width || x < 0 || 
            y >= surface->height || y < 0) {
		*r = *g = *b = 0;
		return;
	}

	c = *ptr;

	switch(surface->bpp) {
		case 16:
			*r = (c & (31 << 11)) >> 8;
			*g = (c & (63 << 5)) >> 3;
			*b = (c & 31) << 3;
			break;
		case 24:
		case 32:
			*r = (c & (0xFF << fb_screen->red_offset)) 
                                        >> fb_screen->red_offset;
			*g = (c & (0xFF << fb_screen->green_offset)) 
                                        >> fb_screen->green_offset;
			*b = (c & (0xFF << fb_screen->blue_offset)) 
                                        >> fb_screen->blue_offset;
			break;
		default:
			fprintf(stderr, "Color depth %i is not yet"
					"supported in fb_getpixel\n", 
					surface->bpp);
			break;
	}
}
/*
 * Function:    fb_blend()
 * Arguments:	alpha, r1, g1, b1, r2, g2, b2,
 *              *r, *g, *b (gets blended value)
 * Returns:     None.
 * Description: blends first rgb values with second rgb values
 *              according to the value of alpha
 *              
 *              with alpha going from 0-255, 0 starts out as
 *              the first rgb values. 255 ends with the second
 *              rgb values.
 */
void fb_blend(int alpha, int r1, int g1, int b1,
              int r2, int g2, int b2,
              int *r, int *g, int *b)
{
	int alpha2;

	if (alpha == 127) {
		*r = (r1 + r2) >> 1;
		*g = (g1 + g2) >> 1;
		*b = (b1 + b2) >> 1;
	} else if (alpha == 255) {
		*r = r2;
		*g = g2;
		*b = b2;
	} else if (!alpha) {
		*r = r1;
		*g = g1;
		*b = b1;
	} else {
		alpha2 = 255 - alpha;
		*r = floor((((r1 * alpha2) + (r2 * alpha)) >> 8));
		*g = floor((((g1 * alpha2) + (g2 * alpha)) >> 8));
		*b = floor((((b1 * alpha2) + (b2 * alpha)) >> 8));
	}	      
}

/*
 * Function:    fb_putpixel_alpha()
 * Arguments:	alpha, x, y, rgb values, and surface
 * Returns:     None.
 * Description: draws a pixel onto a surface at x, y position
 *              with rgb and alpha values
 */
void fb_putpixel_alpha(int alpha, int x, int y,
                       int r1, int g1, int b1,
                       fb_surface *surface)
{
	int r2, g2, b2, r, g, b;

	fb_getpixel(x, y, &r2, &g2, &b2, surface);
	fb_blend(alpha, r1, g1, b1, r2, g2, b2, &r, &g, &b);
	fb_putpixel(x, y, r, g, b, surface);
}

/*
 * Function:    fb_putpixel_aa()
 * Arguments:   alpha, x, y coords (float values), rgb values, surface.
 * Returns:     None.
 * Description: Draws an anti-aliased pixel to a surface at
 *              the specified x, y position, rgb, and alpha values.
 */
void fb_putpixel_aa(int alpha, float x, float y, int r, int g, int b, fb_surface *surface)
{
	float fx, fy, inv_fx, inv_fy;
	int x2, y2;

	x2 = floor(x);
	y2 = floor(y);

	// get fraction parts
	fx = x - x2;
	fy = y - y2;

	// invert fraction parts
	inv_fx = 1 - fx;
	inv_fy = 1 - fy;

	fb_putpixel_alpha(floor(((inv_fx * inv_fy) * alpha)),  x2,     y2,    r, g, b, surface); // top left
	fb_putpixel_alpha(floor(((fx     * inv_fy) * alpha)), (x2++),  y2,    r, g, b, surface); // top right
	fb_putpixel_alpha(floor(((inv_fx * fy)     * alpha)),  x2,    (y2++), r, g, b, surface); // bottom left
	fb_putpixel_alpha(floor(((fx     * fy)     * alpha)), (x2++), (y2++), r, g, b, surface); // bottom right
}
