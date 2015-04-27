#include<stdio.h>  // file 
#include <stdlib.h> // atoi 
#include<ctype.h> // isspace
#include <errno.h> //errno
#include "FrameBuffer.h"

/*
 * Function:	fb_load_ppm()
 * Arguments:	Filename of ppm image to load
 * Returns:	Image on an fb surface
 * Description: Attempts to load a ppm format image and 
 * 		place it in an fb_surface memory bitmap.  
 */
fb_surface *fb_load_ppm(char *filename)
{
	fb_surface *input_ppm;
	FILE *ppm_file;
	char buf[1024];
	char tmpc1, tmpc2;
	int num = 0, gonum = 0, i, w = 0, h = 0, m = 0, x, y, r, g, b;

/*
 * Reduces duplicate code. Trust me.
 */
#define tmpc1_if() \
{ \
	if (tmpc1 == '#') { \
		fgets(buf, 1024, ppm_file); \
		tmpc1 = fgetc(ppm_file); \
	} \
} 

	ppm_file = fopen(filename, "rb");
	
	if (ppm_file == NULL) {
		snprintf(buf, 1024, "Unable to open %s", filename);
		perror(buf);
		return NULL;
	}
	
	tmpc1 = fgetc(ppm_file);
	tmpc2 = fgetc(ppm_file);

	if (tmpc1 != 'P' || tmpc2 != '6') {
		fprintf(stderr, "This file isn't a proper format ppm. "
			  	"Can only handle RAWBITS format.\n");
		return NULL;
	}

	/* reduces duplicate code */
	goto first;

	/* HACK HACK HACK HACK!!!!!!! */
	for (num = 0; num < 3; num++) {
	    second:
		gonum++;

		while (isspace(tmpc1)) {
		    first:
			tmpc1 = fgetc(ppm_file);
			tmpc1_if();


			if (gonum == 0)
				goto second;
		}

		buf[0] = tmpc1;
		buf[1] = '\0';

		for (i = 1; !isspace(tmpc1); i++) {
			tmpc1 = fgetc(ppm_file);
			tmpc1_if();
			buf[i] = tmpc1;
			buf[i + 1] = '\0';
		}
	
		if (num == 0)	
			w = atoi(buf);
		else if (num == 1)
			h = atoi(buf);
		else
			m = atoi(buf);
	}
	
	input_ppm = fb_create_surface(w, h);

	for (y = 0; y < h && !feof(ppm_file); y++)
		for(x = 0; x < w && !feof(ppm_file); x++) {
			r = (unsigned char)fgetc(ppm_file);
			g = (unsigned char)fgetc(ppm_file);
			b = (unsigned char)fgetc(ppm_file);
			fb_putpixel(x, y, r, g, b, input_ppm);
		}

	fclose(ppm_file);

	return input_ppm;
}

/*
 * Function:	fb_save_ppm()
 * Arguments:	Name of file to save to, surface to save
 * Returns:	Zero on success, otherwise non zero
 * Description:	Saves the contents of the surface indicated to a ppm
 * 		RAWBITS format image file.
 */
int fb_save_ppm(char *filename, fb_surface *surface, int grayscale)
{
        int x, y;
        int r, g, b;
        FILE *output_image;

        output_image = fopen(filename, "wb");

        if (surface == NULL)
                return -1;
        if (output_image == NULL)
                return -1;

	fprintf(output_image, "P%d\n", grayscale ? 5 : 6);
        fprintf(output_image, "%i %i\n", surface->width, surface->height);
        fprintf(output_image, "255\n");

        for (y = 0; y < surface->width; y++)
                for (x = 0; x < surface->width; x++) {
                        fb_getpixel(x, y, &r, &g, &b, surface);
                        fputc(r & 0xFF, output_image);
                        fputc(g & 0xFF, output_image);
                        fputc(b & 0xFF, output_image);
                }

        if (fclose(output_image))
                return errno;

        return 0;
}
//------------------------------------------------------------------------------
void  fb_draw_image(fb_surface *dest,fb_surface *src)
{

        int xpos = 0, ypos = 0;
        int r, g, b;
        float ratio_x;
        float ratio_y;
        int x1, y1;

        register unsigned char *ptr1, *ptr2;

//        if (w == src->width && h == src->height)
//                fb_draw_image(src, dest, x, y);

        ratio_x = (float)src->width / (float)dest->width;
        ratio_y = (float)src->height / (float)dest->height;

        ptr1 = (unsigned char *)(src->mem_pointer  + src->mem_offset);

        ptr2 = (unsigned char *)(dest->mem_pointer + dest->mem_offset);

        for (ypos = 0; ypos < dest->height; ypos++)
                for (xpos = 0; xpos < dest->width; xpos++) {
                        x1 = (int)(xpos * ratio_x);
                        y1 = (int)(ypos* ratio_y);

                        fb_getpixel(x1, y1, &r, &g, &b, src);
                        fb_putpixel(xpos, ypos, r, g, b, dest);
                }


}

