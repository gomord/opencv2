#include <cv.h>
#include <highgui.h>

// Define our callback which we will install for
// mouse events.
//
void my_mouse_callback(
		       int event, int x, int y, int flags, void* param
		      );
void moti_mouse_callback(
			 int event, int x, int y, int flags, void* param
			);

CvRect box;
bool drawing_box = false;

// A litte subroutine to draw a box onto an image
//
void draw_box( IplImage* img, CvRect box ) {
	cvRectangle (
		     img,
		     cvPoint(box.x,box.y),
		     cvPoint(box.x+box.width,box.y+box.height),
		     cvScalar(0xff,0x00,0x00)    /* red */
		    );
}
typedef struct MouseParams{
	IplImage* image;
	CvRect    box;
	int       isDrawing;
} MouseParams;
int mouse(const char *win_name,MouseParams* mouseParams){
	printf("win_name - %s\n",win_name);
	//	CvRect* box = (CvRect *)malloc(sizeof(CvRect));
	cvSetMouseCallback(
			   win_name,
			   moti_mouse_callback,
			   mouseParams
			  );
	return 0;
}
int main( int argc, char* argv[] ) {

	box = cvRect(-1,-1,0,0);
	IplImage* image = cvCreateImage(
					cvSize(200,200),
					IPL_DEPTH_8U,
					3
				       );
	cvZero( image );
	MouseParams msPrm = {
		.image = image,
		.box   = box,
		.isDrawing = false
	};
	IplImage* temp = cvCloneImage( image );

	cvNamedWindow( "Box Example" );

	// Here is the crucial moment that we actually install
	// the callback.  Note that we set the value ‘param’ to
	// be the image we are working with so that the callback
	// will have the image to edit.
	//
	/*
	   cvSetMouseCallback(
	   "Box Example",
	   my_mouse_callback,
	   (void*) image
	   );
	 */
	mouse("Box Example",&msPrm);

	// The main program loop.  Here we copy the working image
	// to the ‘temp’ image, and if the user is drawing, then
	// put the currently contemplated box onto that temp image.
	// display the temp image, and wait 15ms for a keystroke,
	// then repeat…
	//
	while( 1 ) {

		cvCopyImage( image, temp );
		if( msPrm.isDrawing ){
			//printf("is Drawing\n");
			draw_box( temp, msPrm.box );
		};
		cvShowImage( "Box Example", temp );

		if( cvWaitKey( 15 )==27 ) break;
	}

	// Be tidy
	//
	cvReleaseImage( &image );
	cvReleaseImage( &temp );
	cvDestroyWindow( "Box Example" );
}

void moti_mouse_callback(
			 int event, int x, int y, int flags, void* param )
{

	MouseParams*  msPrm = (MouseParams*) param;

	switch( event ) {
	case CV_EVENT_RBUTTONDOWN: {
		msPrm->isDrawing = false;
		msPrm->box = cvRect( x, y, 0, 0 );
	}
		break;
	case CV_EVENT_MOUSEMOVE: {
		if( msPrm->isDrawing ) {
			msPrm->box.width  = x-msPrm->box.x;
			msPrm->box.height = y-msPrm->box.y;
		}
	}
		break;
	case CV_EVENT_LBUTTONDOWN: {
		msPrm->isDrawing = true;
		//printf("BD\n");
		msPrm->box = cvRect( x, y, 0, 0 );
	}
		break;
	case CV_EVENT_LBUTTONUP: {
		msPrm->isDrawing = false;
		//printf("BU\n");
		if(msPrm->box.width<0  ) {
			msPrm->box.x += msPrm->box.width;
			msPrm->box.width *=-1;
		}
		if(msPrm->box.height<0 ) {
			msPrm->box.y += msPrm->box.height;
			msPrm->box.height*=-1;
		}
		draw_box( msPrm->image,msPrm->box);
	}
		break;
	}
}
// This is our mouse callback.  If the user
// presses the left button, we start a box.
// when the user releases that button, then we
// add the box to the current image.  When the
// mouse is dragged (with the button down) we
// resize the box.
//
void my_mouse_callback(
		       int event, int x, int y, int flags, void* param )
{

	IplImage* image = (IplImage*) param;

	switch( event ) {
	case CV_EVENT_MOUSEMOVE: {
		if( drawing_box ) {
			box.width  = x-box.x;
			box.height = y-box.y;
		}
	}
		break;
	case CV_EVENT_LBUTTONDOWN: {
		drawing_box = true;
		box = cvRect( x, y, 0, 0 );
	}
		break;
	case CV_EVENT_LBUTTONUP: {
		drawing_box = false;
		if( box.width < 0  ) {
			box.x+=box.width;
			box.width *=-1;
		}
		if( box.height < 0 ) {
			box.y+=box.height;
			box.height *= -1;
		}
		draw_box( image, box );
	}
		break;
	}
}
