#include <stdio.h>
#include <time.h>
#include <cv.h>
#include "highgui.h"
#include "mouse.h"
#define AVG_POS 0
#define RANGE_POS 1
int        g_slider_hue_max = 186;
int        g_slider_hue_min = 123;
int        g_slider_sat_max = 193;
int        g_slider_sat_min = 89;
int        g_slider_val_max = 249;
int        g_slider_val_min = 81;
int        g_mouse	    = 0;
CvScalar   g_hsv_max;
CvScalar   g_hsv_min;
CvCapture* g_capture         = NULL;

int     frames = 0;
void onTrackbarSlide(int pos) {
	g_hsv_max = cvScalar(g_slider_hue_max,
			     g_slider_sat_max,
			     g_slider_val_max
			);
	g_hsv_min = cvScalar(g_slider_hue_min,
			     g_slider_sat_min,
			     g_slider_val_min
			);
	/*cvSetCaptureProperty(
	  g_capture,
	  CV_CAP_PROP_POS_FRAMES,
	  pos
	  );*/
	//frames = pos;
	//g_slider_position = pos;
}

void onTrackbarMouse(int pos){
	printf("test pos %d, g_mouse %d\n",pos, g_mouse);
}
void set_mouse_bar(const char* win_name){
	cvCreateTrackbar("avg-range",
			win_name,
			&g_mouse,
			1,
			onTrackbarMouse);
}
void set_trac_bar(const char* win_name){
	cvCreateTrackbar(
			"Hue Max",
			win_name,
			&g_slider_hue_max,
			255,
			onTrackbarSlide
			);
	cvCreateTrackbar(
			"Hue Min",
			win_name,
			&g_slider_hue_min,
			255,
			onTrackbarSlide
			);
	cvCreateTrackbar(
			"Sat Max",
			win_name,
			&g_slider_sat_max,
			255,
			onTrackbarSlide
			);
	cvCreateTrackbar(
			"Sat Min",
			win_name,
			&g_slider_sat_min,
			255,
			onTrackbarSlide
			);
	cvCreateTrackbar(
			"Val Max",
			win_name,
			&g_slider_sat_max,
			255,
			onTrackbarSlide
			);
	cvCreateTrackbar(
			"Val Min",
			win_name,
			&g_slider_val_min,
			255,
			onTrackbarSlide
			);
}
void set_sliders_filter_hsv(const char * win_name, CvScalar *avg,CvScalar *stn){
        g_slider_hue_max =  avg->val[0]+1*stn->val[0];
	//cvSetTrackbarPos("Hue Max",win_name,g_slider_hue_max );
        g_slider_sat_max =  avg->val[1]+2*stn->val[1];
	//cvSetTrackbarPos("Sat Max",win_name,g_slider_sat_max );
        g_slider_val_max =  avg->val[2]+2*stn->val[2];
	//cvSetTrackbarPos("Val Max",win_name,g_slider_val_max );
        g_slider_hue_min =  avg->val[0]-1*stn->val[0];
	//cvSetTrackbarPos("Hue Min",win_name,g_slider_hue_min );
        g_slider_sat_min =  avg->val[1]-2*stn->val[1];
	//cvSetTrackbarPos("Sat Min",win_name,g_slider_sat_min );
        g_slider_val_min =  avg->val[2]-2*stn->val[2];
	//cvSetTrackbarPos("Val Min",win_name,g_slider_val_min );
	onTrackbarSlide(0);

}
void set_sliders_filter(const char * win_name, CvScalar *avg,CvScalar *stn){
        g_slider_hue_max =  avg->val[0]+2*stn->val[0];
	//cvSetTrackbarPos("Hue Max",win_name,g_slider_hue_max );
        g_slider_sat_max =  avg->val[1]+2*stn->val[1];
	//cvSetTrackbarPos("Sat Max",win_name,g_slider_sat_max );
        g_slider_val_max =  avg->val[2]+2*stn->val[2];
	//cvSetTrackbarPos("Val Max",win_name,g_slider_val_max );
        g_slider_hue_min =  avg->val[0]-2*stn->val[0];
	//cvSetTrackbarPos("Hue Min",win_name,g_slider_hue_min );
        g_slider_sat_min =  avg->val[1]-2*stn->val[1];
	//cvSetTrackbarPos("Sat Min",win_name,g_slider_sat_min );
        g_slider_val_min =  avg->val[2]-2*stn->val[2];
	//cvSetTrackbarPos("Val Min",win_name,g_slider_val_min );
	onTrackbarSlide(0);

}
clock_t begin, end;
double	time_spent;
CvScalar  g_avgScalar;
CvScalar  g_stnScalar;
MouseParams g_msPrm = {0};
int set_avg_pos( IplImage* cl_frame){
	cvSetImageROI(cl_frame,g_msPrm.box);
	cvAvgSdv(cl_frame,&g_avgScalar,&g_stnScalar,NULL);
	set_sliders_filter("set_HSV",&g_avgScalar,&g_stnScalar);
	printf("avg h - %f s - %f v - %f\n",
			g_avgScalar.val[0],
			g_avgScalar.val[1],
			g_avgScalar.val[2]
	      );
	printf("avg hs - %f ss - %f vs - %f\n",
			g_stnScalar.val[0],
			g_stnScalar.val[1],
			g_stnScalar.val[2]
	      );
	cvResetImageROI(cl_frame);
	//cvCvtColor(cl_frame,cl_frame,CV_HSV2RGB);
	draw_box(cl_frame,g_msPrm.box);
	cvShowImage("Camera",cl_frame);
	return 0;

}
int main( int argc, char** argv ) {
	
	int mod = 0;
	CvMoments mom = {0};
	CvPoint fishPos = {0,0};
	IplImage* frame;
	IplImage* cl_frame;
	IplImage* cl_frame_temp;
	IplImage* gr_frame;
	CvSize    frameSize ;
	//cvNamedWindow( "set_HSV", CV_WINDOW_NORMAL);
	cvNamedWindow( "Camera", CV_WINDOW_NORMAL);
	set_mouse_bar("Camera");
	g_capture	= cvCreateCameraCapture( 0 );
	if(g_capture == NULL){
		printf("no camra in comp\n");
		exit(0);
	}
	frame		= cvQueryFrame( g_capture );
	cvCvtColor(frame,frame,CV_RGB2HSV);
	cl_frame	= cvCloneImage(frame);
	cl_frame_temp	= cvCloneImage(frame);
	frameSize	= cvGetSize(frame);
	gr_frame	= cvCreateImage(frameSize,IPL_DEPTH_8U,1);
//	set_trac_bar("set_HSV");
	onTrackbarSlide(0);
	g_msPrm.isDrawing = false;
	g_msPrm.image	= cl_frame;
	g_msPrm.box	= cvRect( 0, 0, 1, 1 );
	mouse("Camera",&g_msPrm);
	//printf("hacked frames %d w %d h %d\n",frames,tmpw,tmph);

	begin = clock();
	while(1) {
	mod++;
		if(g_msPrm.isDrawing){
			cvCopy(frame,cl_frame);
		switch(g_mouse){
		case RANGE_POS:
		break;
		case AVG_POS:
		set_avg_pos(cl_frame);
		break;
		default:
		break;
		}
	}
		else{

			frame = cvQueryFrame( g_capture );
//			if(!(++mod&0x3))
//			cvShowImage("Camera",frame);
#if 1
	//		if(g_msPrm.box.width != 0){
	//			draw_box(frame,g_msPrm.box);
	//		}
	//		else{
	//		}
			cvCircle(frame,
				 fishPos,
				 sqrt(mom.m00)/1,
				 cvScalar(0x00,0x00,0x00)
				 );
			//if(!(++mod&0x3))
			cvSmooth( frame, frame, CV_GAUSSIAN, 3, 3 );
			cvShowImage("Camera",frame);
			//cvCvtColor(frame,frame,CV_RGB2HSV);
			cvInRangeS(frame,g_hsv_min,g_hsv_max,gr_frame);
			cvErode(gr_frame,gr_frame,NULL,2);
			cvDilate(gr_frame,gr_frame,NULL,2);
			cvMoments(gr_frame,&mom,1);
			fishPos.x = (mom.m10/mom.m00);
			fishPos.y = (mom.m01/mom.m00);
//			cvErode(gr_frame,gr_frame,NULL,10);
#endif
		}
		//cvShowImage( "set_HSV", gr_frame );
		char c = (char)cvWaitKey(05);
		if( c == 27 ) break;
	}
	end = clock();
	time_spent = (double)(end - begin)/CLOCKS_PER_SEC;
	printf("time of prog %f, numframe %d,numframe for sec %d\n",
		time_spent,mod,mod/(int)time_spent);
	cvReleaseImage( &cl_frame );
	cvReleaseImage( &cl_frame_temp );
	cvReleaseCapture( &g_capture );
	//cvDestroyWindow( "set_HSV" );
	cvDestroyWindow( "Camera" );
	return(0);
}
