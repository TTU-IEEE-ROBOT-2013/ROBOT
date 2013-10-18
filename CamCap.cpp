#include "opencv2/opencv.hpp"
using namespace cv;
//definitions
#define _Cam 0
bool ProcessTarget(Mat Frame);
int ShootTarget()
{
    VideoCapture cap(_Cam); // open the default camera
    if(!cap.isOpened())  // check if we succeeded
        return -1;
	//Set common properties, if we want/need to
	//CV_CAP_PROP_FRAME_WIDTH Width of the frames in the video stream.
    //CV_CAP_PROP_FRAME_HEIGHT Heigth "" "" ... ""
	//CV_CAP_PROP_FPS Frame rate.
	//CV_CAP_PROP_BRIGHTNESS Brightness of the image (only for cameras).
    //CV_CAP_PROP_CONTRAST Contrast of the image (only for cameras).
    //CV_CAP_PROP_SATURATION Saturation of the image (only for cameras).
    //CV_CAP_PROP_HUE Hue of the image (only for cameras).
    //CV_CAP_PROP_GAIN Gain of the image (only for cameras)
	//from example
    /*
    Mat edges;
    namedWindow("edges",1);
	*/
	bool Finished = false;
    while(!Finished)
    {
        Mat frame;
        cap >> frame; // get a new frame from camera
		//we can use HSV values to detect colors easier,
		//by removing odd effects of lighting and intensity
		//With expiramentation, we may prefer HSL
		//CV_BGR2HSV is used to do this conversion
		//it is also a very fast conversion, reportedly used on
		//hardware in 70's
		//however if you want any specific color scheme
		//we will get it here for you
		
		cvtColor(frame, edges, CV_BGR2HSV);
		//RGB Red Green Blue
		//color work here can be counterintuitive
		//HSV Hue Saturation Value
		//HSL Hue Saturation Luminance
		//both easier and more intuitive to work with
		//with hue specifying general color.
		//HSL can be used to negate lighting effects by simply 
		//setting H=H, S=S, L=0 (so HS0)
		//only opencv works in order HLS not HSL
		//CV_BGR2HLS is used for this conversion
		
		//next we will process our frame, moving servo, ect
		//the faster this function works, the higher our framerate.
		//so don't wait for servos to finish moving, just get them started
		//then stop them when target is ready to shoot
		Finished = ProcessTarget(frame);
		//process target will also be responsible for counting time
		//for 3 seconds to pass.
		//DO NOT DO THIS WITH SLEEP
		
		// the following was part of opencv example
        /*
		cvtColor(frame, edges, CV_BGR2GRAY);
        GaussianBlur(edges, edges, Size(7,7), 1.5, 1.5);
        Canny(edges, edges, 0, 30, 3);
        imshow("edges", edges);
		if(waitKey(30) >= 0) break;
		*/
    }
    // the camera will be deinitialized automatically in VideoCapture destructor
    return 0;
}