#include "Beagle.h"
#include "Target.h"
#include <stdlib.h>
#include <stdio.h>
#ifdef _WIN32
#include "EmptyCam.h"
#else
#include "linuxwebcam.h"
#endif
/*Thanks goes out to RabidGeek again for the linuxwebcam.h header
and for example of how to use it.*/
class myLinuxWebcam : public linuxWebcam
{
	private:
		int i;
	public:
		myLinuxWebcam(){
			i = 0;
		}

		virtual void processImage(imageData * img)
		{
			if(img->getFormat() == RGB ) {
				int w = img->getWidth();
				int h = img->getHeight();
				int size=img->getSize();
				RAW_BITMAP RB;
				RB.size=size;
				RB.width=w;
				RB.height=h;
				RB.length=RB.size/3;
				RB.data=(RGB_t*)img->data;
				/*
				You insert target (image processing) finding code here
				You have All data needed. RB.data uses array of rgb structure
				RB[i].RED for red component
				RB[i].BLUE for blue component
				RB[i].GREEN for green component
				*/

			} else if (img->getFormat()==YUV){

				// Do something with the YUV data
				//   img->data          <--- points to the data
				//   img->getSize()     <--- total number of bytes in the data
			}
		}

};
namespace WebCamGlobals
{
	//tell me you have better way to keep global vars
	//out of way (instead of "don't use")
	myLinuxWebcam wc;
	bool IsInit=false;
}
using namespace WebCamGlobals;
bool InitWebcam()
{
	//myLinuxWebcam wc;
	char dev_name[] = "/dev/video0";    // Change this if you have another video device.

	wc.open_device (dev_name);          // Open the video device
	wc.initDevice (640, 480);           // initialize it to the size of the image you want
										// if your webcamera cannot handle that resolution, it *should* change it to a resolution it can handle
	printf("Camera initialized: %d x %d", wc.getWidth(), wc.getHeight() );
   IsInit=true;
return IsInit;
}
bool Capture()
{
	wc.start_capturing();
	wc.captureFrameRGB();
	wc.stop_capturing();
	return true;
}
bool UnInitWebcam()
{
	wc.uninitDevice ();                 // uninitialize the device
	wc.close_device ();                 // close the stream
	return true;
}
