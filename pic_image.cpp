#include <iostream>
#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
//while in TEST_MODE we have additional debug output that is un-needed (and un-desirable) for runs
#define TEST_MODE
//REC_MODE will write frames to a video buffer BEFORE processing. So that the video can be placed somewhere as TTU advertising or something.
//this mode will likewise be disabled during final operation.
//#define REC_MODE

using namespace std;
using namespace cv;
//insert global variables within a namespace to prevent name conflicts
namespace IMPROC_GLOBALS
{
Mat mat,mat2;
}
//we want our variables to be accesed here often, so do the using namespace bit
using namespace IMPROC_GLOBALS;
#ifdef TEST_MODE //
int main()
{
cout<<"Capture";
//our extra complex initialization code.
//all joking aside, this takes about 1-2 second, so we should think about either
//a: initialize at start signal after we start moving
//b: initialize during the approach to firing block 
//one approach would be to use paralell execution
//either using linux-based threads or by moving nav code completely into pru. 
//the best option would be use linux-based threads and save pru for data acquisition (what it's made for)

//the Mat class is the base image class.

VideoCapture cap(0);
if(!cap.isOpened()){cout<<"ERROR: no camera";return -1;}
//cap>>mat;
//cap>>mat2;
/*some optional init code is to capture a frame into each Mat object.  The Mat objects of opencv are coded
as to reuse memory if enough is already alocated to reduce allocations. because of this, the first call to
each function that creates the matrix takes longer than each subsequent call. Because we are not changing our 
image size, sample size, or number of channels (RGB=3, HSL=3, each 8 bit per channel) we can simply capture
into every matrix one time to auto-init the memory backing.
*/




//In the quest for debugging we sometimes implement horribly ineffiecient code.
//which is why the last thing to do is always optimize out "make it work" debug code.
//mat2=mat.clone();
//Mat mat3;
//mat3=mat.clone();


for(i=SX-3;i<=SX+3;i++)
	for(j=SY-3;j<=SY+3;j++)
	    { if(i>0&&i<sks.width&&j>0&&j<sks.height)
		OFrame(i,j)[0]=255; OFrame(i,j)[1]=0; OFrame(i,j)[2]=0;
	}
}	//green (the gotton format is BGR here)
//we put green dot in center of target!
//note we don't have to do any mat=OFrame type stuff as there was no data copy
cout<<"Captured";
cout<<endl<<endl;
imwrite("alpha.png", mat);
return 0;
}
#endif

/*-----------------------------------------Start of Image Processing (this will be moved to it's own function)--------------------------------------*/
int ProcessImage()
{{

//cap >> mat; //get the picture, convert to HLS space, and generate our _mat for use.
cvtColor(mat, mat2, CV_BGR2HLS);
Mat_ <Vec3b> Frame(mat2);
Mat_ <Vec3b> OFrame(mat);
int i,j;
int SX,SY,ct;
SX=0;SY=0;ct=0;
cv::Size sks = Frame.size();

//3x3 block filter
for(i=1;i<sks.height-1;i++)
for(j=1;j<sks.width-1;j++)
{int di,dj;
int ah;
ah=0;
for(di=-1;di<=1;di++)
for(dj=-1;dj<=1;dj++)
{
ah+=Frame(i+di,j+dj)[0]
}
ah=ah/9;
if(ah > 255)ah=255;
Frame(i,j)[0]=ah;	
}
for(i=1;i<sks.height-1;i++)
for(j=1;j<sks.width-1;j++)
{
//cout<<i<<"|"<<j<<endl;
 if(Frame(i,j)[0] < 12) {  //the value is red hue (12 > h | h > 240)
	//if(Frame(i,j)[2] > 200) //Can vary the sat value to require it to be more or less red (like pink is a red)
		if(Frame(i,j)[1] < 190 && Frame(i,j)[2] > 65) //vary the luminance values.  Black and White can be red with Very Low or Very High luminance
		//off-white can be red with low saturation and high luminance. something similar for black
		{
			SX+=i;
			SY+=j;
			ct++;
			Frame(i,j)[1]=255; //make it white 
			OFrame(i,j)[1]=255;
			OFrame(i,j)[0]=0;
			OFrame(i,j)[2]=0;

		}
		else
			{
			Frame(i,j)[1]=0;
			OFrame(i,j)[1]=0;
			OFrame(i,j)[0]=0;
			OFrame(i,j)[2]=255;
			}
		}
		else
		{
			Frame(i,j)[1]=0;
					{
			Frame(i,j)[1]=0;
			OFrame(i,j)[1]=0;
			OFrame(i,j)[0]=0;
			OFrame(i,j)[2]=255;
			}
		}	
//Frame(i,j)[0]=0;
}
cout<<"SUCCESS_FIND";
//cout<<endl<<endl;
if(ct > 0) //we can't afford DivZero errors in actual program.
{
SX = SX / ct;
SY = SY / ct;
}
}}

