#include "FrameBuffer.h"

/*
 * Function:	draw_horizontal_run()
 * Arguments:	(*x, *y): Pointers to current coordinates in line, 
 * 		(run_length): the length of the current run,
 * 		(x_advance): the size in bytes of one pixel,
 * 		(r, g, b): the color of the run,
 * 		(*surface): the destination drawing surface.
 * Returns:	None
 * Description:	Helper function for fb_line used to draw horizontal
 * 		segments in x-major oblique lines, as part of the
 * 		Bresenham line drawing algorithm.  Never used
 * 		outside of fb_line, so don't try; the function is
 * 		useless in any other context.
 */
static void draw_horizontal_run(int *x, int *y, int run_length, 
                                int x_advance, int r, int g, int b, 
				fb_surface *surface)
{
	int i;

	if (run_length < 0)
		run_length *= -1;

	for (i = 0; i < run_length; i ++)
		fb_putpixel(*x + (i * x_advance), 
			    *y, r, g, b, surface);

	*x += run_length * x_advance;
	*y += 1;
}

/*
 * Function:	draw_vertical_run()
 * Arguments:	(*x, *y): Pointers to current coordinates in line, 
 * 		(run_length): the length of the current run,
 * 		(x_advance): the size in bytes of one pixel,
 * 		(r, g, b): the color of the run,
 * 		(*surface): the destination drawing surface.
 * Returns:	None
 * Description: Helper function for fb_line used to draw vertical
 * 		segments in y-major oblique lines, as part of the
 * 		Bresenham line drawing algorithm.  Never used
 * 		outside of fb_line, so don't try; the function is
 * 		useless in any other context.
 */
static void draw_vertical_run(int *x, int *y, int run_length, 
			      int x_advance, int r, int g, int b, 
			      fb_surface *surface)
{
	int i;

	if (run_length < 0)
		run_length *= -1;

	for (i = 0; i < run_length; i++)
		fb_putpixel(*x, *y + i, 
			    r, g, b, 
			    surface);

	*x += x_advance;
	*y += run_length;
}

/*
 * Function:	fb_line()
 * Arguments:	(x1, y1): Coordinates of first point on the line,
 * 		(x2, y2): Coordinates of last point on the line,
 * 		(r, g, b): Color of the line,
 * 		(*surface): Destination drawing surface.
 * Returns:	None
 * Description:	Draws a line of the specified color from the specified
 * 		start point to the specified end point on the specified
 * 		drawing surface.  One day there may be a function which
 * 		will allow lines to blend between colors from one point
 * 		to the other.  fb_line uses Bresenham's run-sliced line 
 * 		drawing algorithm, based on the implementation by Michael
 * 		Abrash, and as such is quite fast.
 */
void fb_line(int x1, int y1, 
	     int x2, int y2, 
	     int r, int g, int b, 
	     fb_surface *surface)
{
	int temp, adj_up, adj_down, error_term, 
	    x_advance, x_delta, y_delta, whole_step, 
	    initial_pixel_count, final_pixel_count, 
	    i, run_length;


	/* 
	 * We'll always draw top to bottom, to reduce the number 
	 * of cases we have to handle, and to make lines between 
	 * the same endpoints draw the same pixels.
	 */
	if (y1 > y2) {
		temp = y1;
		y1 = y2;
		y2 = temp;
		temp = x1;
		x1 = x2;
		x2 = temp;
	}

	/* 
	 * Figure out whether we're going left or right, and how far 
	 * we're going horizontally.
	 */
	if ((x_delta = x2 - x1) < 0) {
		x_advance = -1;
		x_delta = -x_delta;
	} else {
		x_advance = 1;
	}

	/* Figure out how far we're going vertically */
	y_delta = y2 - y1;

	/* 
	 * Special-case horizontal, vertical, and diagonal lines, 
	 * for speed and to avoid nasty boundary conditions and 
	 * division by 0.
	 */
	if (x_delta == 0) {
		/* Vertical line */
		for (i = 0; i <= y_delta; i++)
			fb_putpixel(x1, y1 + i, r, g, b, surface);

		return;
	}

	if (y_delta == 0) {
		/* Horizontal line */
		if (x_advance < 0) {
			for (i = x_delta; i >= 0; 
			                  i += x_advance)
				fb_putpixel(x1 - i, y1, 
					    r, g, b, 
					    surface);
		} else {
			for (i = 0; i <= x_delta; 
			            i += x_advance)
				fb_putpixel(i + x1, y1, 
					    r, g, b, 
					    surface);
		}

		return;
	}

	if (x_delta == y_delta) {
		/* Diagonal line */
		for (i = 0; i <= x_delta; i++)
			fb_putpixel(x1 + (i * x_advance), 
				    y1 + i, r, g, b, surface);

		return;
	}

	/* Determine whether the line is X or Y major, and handle accordingly */
	if (x_delta >= y_delta) {
		/* X major line */
		/* Minimum # of pixels in a run in this line */
		whole_step = x_delta / y_delta;

		/* 
		 * Error term adjust each time Y steps by 1; used to tell 
		 * when one extra pixel should be drawn as part of a run, 
		 * to account for fractional steps along the X axis per 
		 * 1-pixel steps along y
		 */
		adj_up = (x_delta % y_delta) * 2;

		/* 
		 * Error term adjust when the error term turns over, used 
		 * to factor out the X step made at that time
		 */
		adj_down = y_delta * 2;

		/* 
		 * Initial error term; reflects an inital step of 0.5 along 
		 * the Y axis
		 */
		error_term = (x_delta % y_delta) - (y_delta * 2);

		/* 
		 * The initial and last runs are partial, because Y advances 
		 * only 0.5 for these runs, rather than 1.  Divide one full 
		 * run, plus the initial pixel, between the initial and last 
		 * runs
		 */
		initial_pixel_count = (whole_step / 2) + 1;
		final_pixel_count = initial_pixel_count;

		/* 
		 * If the basic run length is even and there's no fractional 
		 * advance, we have one pixel that could go to either the 
		 * inital or last partial run, which we'll arbitrarily allocate
		 * to the last run
		 */
		if ((adj_up == 0) && ((whole_step & 0x01) == 0))
			initial_pixel_count--;
		
		/* 
		 * If there're an odd number of pixels per run, we have 1 pixel
		 * that can't be allocated to either the initial or last 
		 * partial run, so we'll add 0.5 to error term so this pixel 
		 * will be handled by the normal full-run loop
		 */
		if ((whole_step & 0x01) != 0)
			error_term += y_delta;

		/* Draw the first, partial run of pixels */
		draw_horizontal_run(&x1, &y1, initial_pixel_count, 
				    x_advance, r, g, b, surface);

		/* Draw all full runs */
		for (i = 0; i < (y_delta - 1); i++) {
			/* run is at least this long */
			run_length = whole_step;

			/* 
			 * Advance the error term and add an extra pixel if 
			 * the error term so indicates
			 */
			if ((error_term += adj_up) > 0) {
				run_length++;
				/* reset the error term */
				error_term -= adj_down;
			}

			/* Draw this scan line's run */
			draw_horizontal_run(&x1, &y1, run_length, 
				            x_advance, r, g, b, 
					    surface);
		}

		/* Draw the final run of pixels */
		draw_horizontal_run(&x1, &y1, final_pixel_count, 
				    x_advance, r, g, b, surface);
		return;
	} else {
		/* Y major line */

		/* Minimum # of pixels in a run in this line */
		whole_step = y_delta / x_delta;

		/* 
		 * Error term adjust each time X steps by 1; used to tell when 
		 * 1 extra pixel should be drawn as part of a run, to account 
		 * for fractional steps along the Y axis per 1-pixel steps 
		 * along X
		 */
		adj_up = (y_delta % x_delta) * 2;

		/* 
		 * Error term adjust when the error term turns over, used to 
		 * factor out the Y step made at that time
		 */
		adj_down = x_delta * 2;

		/* Initial error term; reflects initial step of 0.5 along the 
		 * X axis 
		 */
		error_term = (y_delta % x_delta) - (x_delta * 2);

		/* 
		 * The initial and last runs are partial, because X advances 
		 * only 0.5 for these runs, rather than 1.  Divide one full 
		 * run, plus the initial pixel, between the initial and last 
		 * runs
		 */
		initial_pixel_count = (whole_step / 2) + 1;
		final_pixel_count = initial_pixel_count;

		/* 
		 * If the basic run length is even and there's no fractional 
		 * advance, we have 1 pixel that could go to either the 
		 * initial or last partial run, which we'll arbitrarily 
		 * allocate to the last run
		 */
		if ((adj_up == 0) && ((whole_step & 0x01) == 0))
			initial_pixel_count--;

		/* 
		 * If there are an odd number of pixels per run, we have one 
		 * pixel that can't be allocated to either the initial or last 
		 * partial run, so we'll ad 0.5 to the error term so this 
		 * pixel will be handled by the normal rull-run loop
		 */
		if ((whole_step & 0x01) != 0)
			error_term += x_delta;

		/* Draw the first, partial run of pixels */
		draw_vertical_run(&x1, &y1, initial_pixel_count, 
				  x_advance, r, g, b, surface);

		/* Draw all full runs */
		for (i = 0; i < (x_delta - 1); i++) {
			/* run is at least this long */
			run_length = whole_step;

			/* 
			 * Advance the error term and add an extra pixel if the
			 * error term so indicates
			 */
			if ((error_term += adj_up) > 0) {
				run_length++;
				/* reset the error term */
				error_term -= adj_down;
			}

			/* Draw this scan line's run */
			draw_vertical_run(&x1, &y1, run_length, 
					  x_advance, r, g, b, 
					  surface);
		}
		
		/* Draw the final run of pixels */
		draw_vertical_run(&x1, &y1, final_pixel_count, 
				  x_advance, r, g, b, surface);
		return;
	}
}


