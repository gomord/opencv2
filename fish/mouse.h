#ifndef _MOUSE_H
#define _MOUSE_H

typedef struct MouseParams{
	IplImage* image;
	CvRect    box;
	int       isDrawing;
	int	  start_x;
	int	  start_y;
} MouseParams;
int mouse(const char *win_name,MouseParams* mouseParams);
void draw_box( IplImage* img, CvRect box ); 
#endif //_MOUSE_H
