#include "hal.h"
#include "device/video.h"
#include "device/palette.h"

#include <string.h>
#include <stdlib.h>

int get_fps();

void SDL_BlitSurface(SDL_Surface *src, SDL_Rect *scrrect, 
		SDL_Surface *dst, SDL_Rect *dstrect) {
	assert(dst && src);

	/* TODO: Performs a fast blit from the source surface to the 
	 * destination surface. Only the position is used in the
	 * ``dstrect'' (the width and height are ignored). If either
	 * ``srcrect'' or ``dstrect'' are NULL, the entire surface 
	 * (``src'' or ``dst'') is copied. The final blit rectangle 
	 * is saved in ``dstrect'' after all clipping is performed
	 * (``srcrect'' is not modified).
	 */

//	assert(0);
	int src_x,src_y,dst_x,dst_y;
	uint32_t src_w=0,src_h=0;
	if(scrrect==NULL){
		src_x=0;
		src_y=0;
		src_w=src->w;
		src_h=src_h;
	}
	else{
		src_x=scrrect->x;
		src_y=scrrect->y;
		src_w=scrrect->w;
		src_h=scrrect->h;
	}
	if(dstrect==NULL){
		dst_x=0;
		dst_y=0;
//		dst_w=dst->w;
//		dst_h=dst_h;
	
		dstrect=malloc(sizeof(SDL_Rect));
		dstrect->x=0;
		dstrect->y=0;
		dstrect->w=src_w;
		dstrect->h=src_h;
	}
	else{
		dst_x=dstrect->x;
		dst_y=dstrect->y;
		dstrect->w=src_w;
		dstrect->h=src_h;
//		dst_w=dstrect->w;
//		dst_h=dstrect->h;
	}
	int i,j;
	for(i=0;i<src_h;i++){
		for(j=0;j<src_w;j++){
			dst->pixels[(dst_y+i)*dst->pitch+dst_x+j]=src->pixels[(src_y+i)*src->pitch+src_x+j];		
		}
	}
}

void SDL_FillRect(SDL_Surface *dst, SDL_Rect *dstrect, uint32_t color) {
	assert(dst);
	assert(color <= 0xff);

	/* TODO: Fill the rectangle area described by ``dstrect''
	 * in surface ``dst'' with color ``color''. If dstrect is
	 * NULL, fill the whole surface.
	 */

//	assert(0);
	int dst_x,dst_y;
	uint32_t dst_w=0,dst_h=0;
	if(dstrect==NULL){
		dst_x=dstrect->x;
		dst_y=dstrect->y;
		dst_w=dstrect->w;
		dst_h=dstrect->h;
	}
	else{
		dst_x=0;
		dst_y=0;
		dst_w=dst->w;
		dst_h=dst->h;
	}
	int i,j;
	for(i=0;i<dst_h;i++){
		for(j=0;j<dst_w;j++){
			dst->pixels[(dst_y+i)*dst->pitch+dst_x+j]=color&0xff;
		}
	}
}

void SDL_UpdateRect(SDL_Surface *screen, int x, int y, int w, int h) {
	assert(screen);
	assert(screen->pitch == 320);
	if(screen->flags & SDL_HWSURFACE) {
		if(x == 0 && y == 0) {
			/* Draw FPS */
			vmem = VMEM_ADDR;
			char buf[80];
			sprintf(buf, "%dFPS", get_fps());
			draw_string(buf, 0, 0, 10);
		}
		return;
	}

	/* TODO: Copy the pixels in the rectangle area to the screen. */

//	assert(0);
	if(x==0&&y==0&&w==0&&h==0){
		x=0;
		y=0;
		w=screen->w;
		h=screen->h;
	}
	int i,j;
	for(i=0;i<h;i++){
		for(j=0;j<w;j++){
			draw_pixel(y+i,x+j,screen->pixels[(y+i)*screen->pitch+x+j]);
		}
	}
}

void SDL_SetPalette(SDL_Surface *s, int flags, SDL_Color *colors, 
		int firstcolor, int ncolors) {
	assert(s);
	assert(s->format);
	assert(s->format->palette);
	assert(firstcolor == 0);

	if(s->format->palette->colors == NULL || s->format->palette->ncolors != ncolors) {
		if(s->format->palette->ncolors != ncolors && s->format->palette->colors != NULL) {
			/* If the size of the new palette is different 
			 * from the old one, free the old one.
			 */
			free(s->format->palette->colors);
		}

		/* Get new memory space to store the new palette. */
		s->format->palette->colors = malloc(sizeof(SDL_Color) * ncolors);
		assert(s->format->palette->colors);
	}

	/* Set the new palette. */
	s->format->palette->ncolors = ncolors;
	memcpy(s->format->palette->colors, colors, sizeof(SDL_Color) * ncolors);

	if(s->flags & SDL_HWSURFACE) {
		/* TODO: Set the VGA palette by calling write_palette(). */
//		assert(0);
		write_palette(s->format->palette->colors,s->format->palette->ncolors);
	}
}

/* ======== The following functions are already implemented. ======== */

void SDL_SoftStretch(SDL_Surface *src, SDL_Rect *scrrect, 
		SDL_Surface *dst, SDL_Rect *dstrect) {
	assert(src && dst);
	int x = (scrrect == NULL ? 0 : scrrect->x);
	int y = (scrrect == NULL ? 0 : scrrect->y);
	int w = (scrrect == NULL ? src->w : scrrect->w);
	int h = (scrrect == NULL ? src->h : scrrect->h);

	assert(dstrect);
	if(w == dstrect->w && h == dstrect->h) {
		/* The source rectangle and the destination rectangle
		 * are of the same size. If that is the case, there
		 * is no need to stretch, just copy. */
		SDL_Rect rect;
		rect.x = x;
		rect.y = y;
		rect.w = w;
		rect.h = h;
		SDL_BlitSurface(src, &rect, dst, dstrect);
	}
	else {
		/* No other case occurs in NEMU-PAL. */
		assert(0);
	}
}

SDL_Surface* SDL_CreateRGBSurface(uint32_t flags, int width, int height, int depth,
		uint32_t Rmask, uint32_t Gmask, uint32_t Bmask, uint32_t Amask) {
	SDL_Surface *s = malloc(sizeof(SDL_Surface));
	assert(s);
	s->format = malloc(sizeof(SDL_PixelFormat));
	assert(s);
	s->format->palette = malloc(sizeof(SDL_Palette));
	assert(s->format->palette);
	s->format->palette->colors = NULL;

	s->format->BitsPerPixel = depth;

	s->flags = flags;
	s->w = width;
	s->h = height;
	s->pitch = (width * depth) >> 3;
	s->pixels = (flags & SDL_HWSURFACE ? (void *)VMEM_ADDR : malloc(s->pitch * height));
	assert(s->pixels);

	return s;
}

SDL_Surface* SDL_SetVideoMode(int width, int height, int bpp, uint32_t flags) {
	return SDL_CreateRGBSurface(flags,  width, height, bpp,
			0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000);
}

void SDL_FreeSurface(SDL_Surface *s) {
	if(s != NULL) {
		if(s->format != NULL) {
			if(s->format->palette != NULL) {
				if(s->format->palette->colors != NULL) {
					free(s->format->palette->colors);
				}
				free(s->format->palette);
			}

			free(s->format);
		}
		
		if(s->pixels != NULL) {
			free(s->pixels);
		}

		free(s);
	}
}

