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
cap >> mat2;
cvtColor(mat2, mat, CV_BGR2HLS);
//cap.set(CV_CAP_PROP_FPS, 30);
//cap.set(CV_CAP_PROP_FRAME_WIDTH ,620); //wid
//cap.set(CV_CAP_PROP_FRAME_HEIGHT ,480); //heigh
//Mat s2x;
//cvtColor(mat, s2x, CV_BGR2HSV);
Mat_ <Vec3b> Frame(mat);
Mat_ <Vec3b> OFrame(mat2);
int i,j;
int SX,SY,ct;
SX=0;SY=0;ct=0;
for(i=0;i<mat.cols();i++)
for(j=0;j<mat.rows();j++)
{
 if(Frame(i,j)[0] < 20) {  //the value is red hue (20 > h | h > 240)
	//if(Frame(i,j)[2] > 200) //Can vary the sat value to require it to be more or less red (like pink is a red)
		if(Frame(i,j)[1] < 190 && Frame(i,j)[2] > 65) //vary the luminance values.  Black and White can be red with Very Low or Very High luminance
		//off-white can be red with low saturation and high luminance. something similar for black
		{
		SX+=i;
		SY+=j;
		ct++;
		Frame(i,j)[1]=255; //make it white 
		}
		else
		{
		Frame(i,j)[1]=0;
		}
		else
	Frame(i,j)[1]=0;	
//Frame(i,j)[0]=0;
}
SX = SX / ct;
SY = SY / ct;
for(i=-3;i<=3;i++)
	for(j=-3;j<=3;j++)
	    {OFrame(i,j)[0]=0; OFrame(i,j)[1]=255; OFrame(i,j)[2]=0;} //green (the gotton format is BGR here)
//we put green dot in center of target!
//note we don't have to do any mat=OFrame type stuff as there was no data copy
cout<<"Captured";
imwrite("alpha.png", mat);	
return 0;
}
