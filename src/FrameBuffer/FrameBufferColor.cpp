#include "FrameBufferColor.h"
#include <math.h>

/*
 * Function:	fb_make_color_8()
 * Arguments:	rgb values (0-255)
 * Returns:	8-bit color
 * Description: Makes a color out of specified rgb values for
 * 		specified bit depth and returns it.
 */
unsigned long fb_make_color_8(int r, int g, int b)
{
	r >>= 5;
	g >>= 5;
	b >>= 6;

	r &= 7;
	g &= 7;
	b &= 3;

	return (r << 5 | g << 2 | b << 0);
}

/*
 * Function:	fb_make_color_15()
 * Arguments:	rgb values (0-255)
 * Returns:	15-bit color
 * Description: Makes a color out of specified rgb values for
 * 		specified bit depth and returns it.
 */
unsigned long fb_make_color_15(int r, int g, int b)
{
	r >>= 3;
	g >>= 3;
	b >>= 3;

	return (r << 10 | g << 5 | b);
}

/*
 * Function:	fb_make_color_16()
 * Arguments:	rgb values (0-255)
 * Returns:	16-bit color
 * Description: Makes a color out of specified rgb values for
 * 		specified bit depth and returns it.
 */
unsigned long fb_make_color_16(int r, int g, int b)
{
	r >>= 3;
	g >>= 2;
	b >>= 3;
	
	return (r << 11 | g << 5 | b);
}

/*
 * Function:	fb_make_color_24()
 * Arguments:	rgb values (0-255)
 * Returns:	24-bit color
 * Description: Makes a color out of specified rgb values for
 * 		specified bit depth and returns it.
 */
unsigned long fb_make_color_24(int r, int g, int b)
{

	r &= 0xFF;
	g &= 0xFF;
	b &= 0xFF;

	return (r << fb_screen->red_offset | 
                g << fb_screen->green_offset | 
                b << fb_screen->blue_offset);
}

/*
 * Function:	fb_make_color_unknown()
 * Arguments:	rgb values (0-255)
 * Returns:	What is hopefully the color
 * Description:	Tries to figure out how to make a color for
 * 		an unknown bit depth.
 */
unsigned long fb_make_color_unknown(int r, int g, int b)
{
	r &= 0xFF;
	g &= 0xFF;
	b &= 0xFF;

	r <<= fb_screen->red_size - 8;
	g <<= fb_screen->green_size - 8;
	b <<= fb_screen->blue_size - 8;

	r &= 0xFF << (fb_screen->red_size - 8);
	g &= 0xFF << (fb_screen->green_size - 8);
	b &= 0xFF << (fb_screen->blue_size - 8);

	return (r << fb_screen->red_offset |
		g << fb_screen->green_offset |
		b << fb_screen->blue_offset);
}

/*
* Function:	fb_get_color()
* Arguments:	Hardware color, pointers to location to store 
* 		color components
* Returns:	None
* Description:	Stores the color components of the given hardware
* 		color in the given locations.
*/
void fb_get_color(unsigned int hwcolor, int *r, int *g, int *b)
{
       *r = ((hwcolor >> fb_screen->red_offset) & ((1 << fb_screen->red_size) - 1)) >> (fb_screen->red_size - 8);
       *g = ((hwcolor >> fb_screen->green_offset) & ((1 << fb_screen->green_size) - 1)) >> (fb_screen->green_size - 8);
       *b = ((hwcolor >> fb_screen->blue_offset) & ((1 << fb_screen->blue_size) - 1)) >> (fb_screen->blue_size - 8);
}


/*
 * Function:    fb_get_textcolor()
 * Arguments:   Pointers to foreground and background to
 *              place values in.
 * Returns:     None.
 * Description: Places current foreground and background
 *              colors in specified variables.
 */
void fb_get_textcolor(int *fg, int *bg)
{
	*fg = fb_screen->text.color;
	*bg = fb_screen->text.bgcolor;
}

/*
 * Function:    fb_sget_textcolor()
 * Arguments:   Pointers to surface to get values from
 *              and pointers to foreground and background to
 *              place values in.
 * Returns:     None.
 * Description: Places current foreground and background
 *              colors in specified variables.
 */
void fb_sget_textcolor(fb_surface *surface, int *fg, int *bg)
{
	*fg = surface->text.color;
	*bg = surface->text.bgcolor;
}

/*
 * Function:    fb_set_textcolor()
 * Arguments:   Foreground and background values.
 * Returns:     None.
 * Description: Sets the text color to the specified
 *              foreground and background values.
 */
void fb_set_textcolor(int fg, int bg)
{
	fb_screen->text.color   = fg;
	fb_screen->text.bgcolor = bg;
}

/*
 * Function:    fb_sset_textcolor()
 * Arguments:   Surface, foreground and background values.
 * Returns:     None.
 * Description: Sets the text color to the specified
 *              foreground and background values.
 */
void fb_sset_textcolor(fb_surface *surface, int fg, int bg)
{
	surface->text.color   = fg;
	surface->text.bgcolor = bg;
}

