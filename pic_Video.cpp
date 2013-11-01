#include <iostream>
#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
using namespace std;
using namespace cv;

int main()
{
Mat mat,mat2;
VideoCapture cap(0);
if(!cap.isOpened()){cout<<"ERROR: no camera";return -1;}
cout<<"Capture";

int i,j;
int SX,SY,ct;


cout<<"Success_INIT\n";
namedWindow("ActiveTarget");
while(true){
SX=0;SY=0;ct=0;
cap >> mat;
//mat2=mat.clone();
//Mat mat3;
//mat3=mat.clone();
cvtColor(mat, mat2, CV_BGR2HLS);
//cap.set(CV_CAP_PROP_FPS, 30);
//cap.set(CV_CAP_PROP_FRAME_WIDTH ,620); //wid
//cap.set(CV_CAP_PROP_FRAME_HEIGHT ,480); //heigh
//Mat s2x;
//cvtColor(mat, s2x, CV_BGR2HSV);
Mat_ <Vec3b> Frame(mat2);
Mat_ <Vec3b> OFrame(mat);
cv::Size sks = Frame.size();
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
cout<<endl<<endl;
SX = SX / ct;
SY = SY / ct;
for(i=SX-3;i<=SX+3;i++)
	for(j=SY-3;j<=SY+3;j++)
	    { if(i>0&&i<sks.width&&j>0&&j<sks.height)
		OFrame(i,j)[0]=255; OFrame(i,j)[1]=0; OFrame(i,j)[2]=0;
	} //green (the gotton format is BGR here)
imshow("ActiveTarget",mat);
//we put green dot in center of target!
//note we don't have to do any mat=OFrame type stuff as there was no data copy
}
return 0;
}
