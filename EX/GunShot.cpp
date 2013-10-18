#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "linuxwebcam.h"
#include "Target.h" //contains ttargeting helper functions
#define rgb RRGB
#include "TargTuning.h" //holds constants just for targeting only
#define sgn(A) ((0 < A) - (A < 0)) //signum function
namespace _GUN
{

int OPTx=0,OPTy=0,OPTcx=0,OPTcy=0;
class GunCam : public linuxWebcam
{
	private:
		int i;
	public:
		GunCam(){
			i = 0;
		}

		virtual void processImage(imageData * img)
		{
			if(img->getFormat() == RGB ) {
				int w = img->getWidth();
				int h = img->getHeight();
				OPTcx=w/2;
				OPTcy=h/2;
				//center of image
				int size=img->getSize();
				RAW_BITMAP RB;
				RB.size=size;
				RB.width=w;
				RB.height=h;
				RB.length=RB.size/3;
				RB.data=(RGB_t*)img->data;
				RGB_t COLS[] =COLORS_TARGET; //check #define for COLORS_TARGET if you think this aint no array
				Point R = ProcRGBtoRMGM(RB,COLS,COLORS_COUNT);
				OPTx=R.x;
				OPTy=R.y;
				/*
				You insert target (image processing) finding code here
				You have All data needed. RB.data uses array of rgb structure
				RB[i].RED for red component
				RB[i].BLUE for blue component
				RB[i].GREEN for green component
				*/

			} else if (img->getFormat()==YUV){
				//Is there and advantage to YUV?
			}
		}

};
GunCam wc;
/* more functions to support mdx and it servo-actions 
(shoot should be self-contained in software and mostly hardware implemented<555 single pulse out>)*/
void mdx(int a,int b)//sets servo moving (with feed back from camera when to stop)
{
static double VOLT=0;
//servo control here (need static int/static double or something
//and conversion form x,y to angular coords.
}
void shoot()//shoots the gun
{
//write a single high to the (not yet choosen) output
//trigger should activate during a hightolow transition
//then wait .1sec, then write back low for when next shot comes
}
}
using namespace _GUN;
void ShootTarget()
{	
	wc.start_capturing();
	int dx,dy;
	do
	{
	dx=sgn(OPTx-OPTcx); //note these values when controlling the servo
	dy=sgn(OPTy-OPTcy); //sgn returns just -1,0,1. if you want mdx more complex, just remove sgn
	//but dx and dy would be in terms of pixels(non-real-unit) then
	mdx(dx,dy);//important to note the gun don't move untill second loop through
	wc.captureFrameRGB();
	}while(OPTx!=OPTcx&&OPTy!=OPTcy);
	shoot();//lazer could be a gould test of aiming system without shooting.
	wc.stop_capturing();
	//to return to navigation, just let this function RETURN!
	return;
}
void GunInit()
{
	//myLinuxWebcam wc;
	char dev_name[] = "/dev/video0";    // Change this if you have another video device(we shouldn't).

	wc.open_device (dev_name);          // Open the video device
	wc.initDevice (640, 480);           // initialize it to the size of the image you want
										// if your webcamera cannot handle that resolution, it *should* change it to a resolution it can handle
	printf("Camera initialized: %d x %d", wc.getWidth(), wc.getHeight() );
}
void GunDone()
{
	wc.uninitDevice ();                 // uninitialize the device
	wc.close_device ();                 // close the stream
}