#include <stdio.h>
//#include <iostream>
//#include <fstream>
#include <cv.h>
#include "highgui.h"
#include "mouse.h"
//using namespace std;

int        g_slider_hue_max= 186;
int        g_slider_hue_min= 123;
int        g_slider_sat_max= 193;
int        g_slider_sat_min= 89;
int        g_slider_val_max= 249;
int        g_slider_val_min= 81;
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
void set_sliders_filter(const char * win_name, CvScalar *avg,CvScalar *stn){
        g_slider_hue_max =  avg->val[0]+2*stn->val[0];
	//cvSetTrackbarPos("Hue Max",win_name,g_slider_hue_max );
        g_slider_sat_max =  avg->val[1]+4*stn->val[1];
	//cvSetTrackbarPos("Sat Max",win_name,g_slider_sat_max );
        g_slider_val_max =  avg->val[2]+4*stn->val[2];
	//cvSetTrackbarPos("Val Max",win_name,g_slider_val_max );
        g_slider_hue_min =  avg->val[0]-2*stn->val[0];
	//cvSetTrackbarPos("Hue Min",win_name,g_slider_hue_min );
        g_slider_sat_min =  avg->val[1]-4*stn->val[1];
	//cvSetTrackbarPos("Sat Min",win_name,g_slider_sat_min );
        g_slider_val_min =  avg->val[2]-4*stn->val[2];
	//cvSetTrackbarPos("Val Min",win_name,g_slider_val_min );
	onTrackbarSlide(0);

}

int main( int argc, char** argv ) {
	CvMoments mom = {0};
	CvPoint fishPos = {0,0};
	MouseParams msPrm = {0};
	IplImage* frame;
	IplImage* cl_frame;
	IplImage* cl_frame_temp;
	IplImage* gr_frame;
	CvSize    frameSize ;
	CvScalar  avgScalar;
	CvScalar  stnScalar;
	//cvNamedWindow( "set_HSV", CV_WINDOW_NORMAL);
	cvNamedWindow( "Camera", CV_WINDOW_NORMAL);
	g_capture	= cvCreateCameraCapture( 0 );
	frame		= cvQueryFrame( g_capture );
	cvCvtColor(frame,frame,CV_RGB2HSV);
	cl_frame	= cvCloneImage(frame);
	cl_frame_temp	= cvCloneImage(frame);
	frameSize	= cvGetSize(frame);
	gr_frame	= cvCreateImage(frameSize,IPL_DEPTH_8U,1);
//	set_trac_bar("set_HSV");
	onTrackbarSlide(0);
	msPrm.isDrawing = false;
	msPrm.image	= cl_frame;
	msPrm.box	= cvRect( 0, 0, 1, 1 );
	mouse("Camera",&msPrm);
	//printf("hacked frames %d w %d h %d\n",frames,tmpw,tmph);


	while(1) {
		if(msPrm.isDrawing){
			//printf("is dr\n");
			cvCopy(frame,cl_frame_temp);
			cvSetImageROI(cl_frame_temp,msPrm.box);
			cvAvgSdv(cl_frame_temp,&avgScalar,&stnScalar,NULL);
			set_sliders_filter("set_HSV",&avgScalar,&stnScalar);
			printf("avg h - %f s - %f v - %f\n",
			       avgScalar.val[0],
			       avgScalar.val[1],
			       avgScalar.val[2]
			       );
			printf("avg hs - %f ss - %f vs - %f\n",
			       stnScalar.val[0],
			       stnScalar.val[1],
			       stnScalar.val[2]
			       );
			cvResetImageROI(cl_frame_temp);
			cvCvtColor(cl_frame_temp,cl_frame_temp,CV_HSV2RGB);
			draw_box(cl_frame_temp,msPrm.box);
			cvShowImage("Camera",cl_frame_temp);
		}
		else{

			frame = cvQueryFrame( g_capture );
	//		if(msPrm.box.width != 0){
	//			draw_box(frame,msPrm.box);
	//		}
	//		else{
	//		}
			cvSmooth( frame, frame, CV_GAUSSIAN, 3, 3 );
			cvCircle(frame,
				 fishPos,
				 sqrt(mom.m00)/2,
				 cvScalar(0x00,0x00,0x00)
				 );
			cvShowImage("Camera",frame);
			cvCvtColor(frame,frame,CV_RGB2HSV);
			cvCopy(frame,cl_frame);
			cvInRangeS(frame,g_hsv_min,g_hsv_max,gr_frame);
			cvErode(gr_frame,gr_frame,NULL,2);
			cvDilate(gr_frame,gr_frame,NULL,2);
			cvMoments(gr_frame,&mom,1);
			fishPos.x = (mom.m10/mom.m00);
			fishPos.y = (mom.m01/mom.m00);
//			cvErode(gr_frame,gr_frame,NULL,10);
		}
		//cvShowImage( "set_HSV", gr_frame );
		char c = (char)cvWaitKey(25);
		if( c == 27 ) break;
	}
	cvReleaseImage( &cl_frame );
	cvReleaseImage( &cl_frame_temp );
	cvReleaseCapture( &g_capture );
	//cvDestroyWindow( "set_HSV" );
	cvDestroyWindow( "Camera" );
	return(0);
}
