#include <fstream>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <opencv2/opencv.hpp>
#include "Beagle.h"
#include "Beagle.cpp"

using namespace std;
#define PWM1duty "/sys/devices/ocp.2/pwm_test_P8_13.14/duty"
#define PWM1period "/sys/devices/ocp.2/pwm_test_P8_13.14/period"
#define PWM2duty "/sys/devices/ocp.2/pwm_test_P9_14.15/duty"
#define PWM2period "/sys/devices/ocp.2/pwm_test_P9_14.15/period"

#define GPIO_1_28_N 60
#define GPIO_1_16_N 48
#define GPIO_1_28    "/sys/class/gpio/gpio60"
#define GPIO_1_16    "/sys/class/gpio/gpio48"
#define GPIO_EXPORT  "/sys/class/gpio/export"
#define GPIO_UEXPORT "/sys/class/gpio/unexport"
typedef struct _cpoint {int x;int y} CPoint;
//1_28 gpio = 32 + 28

#define slots "/sys/devices/bone_capemgr.8/slots"
void Set_Pwm(int duty1, int duty2)
{
	fstream EXPORT;
	EXPORT.open(PWM2duty,ios::out | ios::in);
	EXPORT << duty1;
	EXPORT.flush();
	EXPORT.close();
	EXPORT.open(PWM1duty,ios::out | ios::in);
	EXPORT << duty2;
	EXPORT.flush();
	EXPORT.close();
}
void init()
{
	//create our pwms
	fstream EXPORT;
	EXPORT.open(slots,ios::out | ios::in);
	EXPORT << "am335x_pwm";
	EXPORT.flush();
	EXPORT.close();
	EXPORT.open(slots,ios::out | ios::in);
	EXPORT << "bone_pwm_P8_13";
	EXPORT.flush();
	EXPORT.close();
	EXPORT.open(slots,ios::out | ios::in);
	EXPORT << "bone_pwm_P9_14";
	EXPORT.flush();
	EXPORT.close();	
	EXPORT.open(slots,ios::out | ios::in);
	EXPORT << "ADAFRUIT-UART4";
	EXPORT.flush();
	EXPORT.close();	
	//Create our 2 gpio
	FILE * GPO;
	GPO=fopen(GPIO_EXPORT,"a");
	//cout<<(int)GPO;
	//cout.flush();
	fprintf(GPO,"%d",GPIO_1_28_N);
	fclose(GPO);
	GPO=fopen(GPIO_EXPORT,"a");
	//cout<<(int)GPO;
	//cout.flush();
	fprintf(GPO,"%d",GPIO_1_16_N);
	fclose(GPO);
	EXPORT.open(PWM2period,ios::out | ios::in);
	EXPORT << 20000000;
	EXPORT.flush();
	EXPORT.close();
	EXPORT.open(PWM1period,ios::out | ios::in);
	EXPORT << 20000000;
	EXPORT.flush();
	EXPORT.close();
	
	
}
CPoint TFind()
	{
		Mat mat,mat2;
		cap >> mat2;
		cvtColor(mat2, mat, CV_BGR2HLS);
		Mat_ <Vec3b> Frame(mat);
		//	Mat_ <Vec3b> OFrame(mat2);
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
					//we can also add color-based
					int dx = (Frame(i-1,j)[2]-Frame(i+1,j)[2]);
					int dy = (Frame(i,j-1)[2]-Frame(i,j+1)[2]);
					if(dx<0)dx*=-1;
					if(dy<0)dy*=-1;
					int dxy = dx+dy;
					SX+=i*dxy;
					SY+=j*dxy;
					ct+=dxy;
					//			Frame(i,j)[1]=255; //make it different
				}
				//		else
				//		{
				//			Frame(i,j)[1]=0;
				//		}
				//		else
				//		Frame(i,j)[1]=0;	
				//Frame(i,j)[0]=0;
			}
			SX = SX / ct;
			SY = SY / ct;
		}
	}
void DeInit()
{
	FILE * GPO;
	GPO=fopen(GPIO_UEXPORT,"a");
	//cout<<(int)GPO;
	//cout.flush();
	fprintf(GPO,"%d",GPIO_1_28_N);
	fclose(GPO);
	GPO=fopen(GPIO_UEXPORT,"a");
	//cout<<(int)GPO;
	//cout.flush();
	fprintf(GPO,"%d",GPIO_1_16_N);
	fclose(GPO);
	
}
#define SFact 10000
void ACCPWM(int dP,int dT)
{
static int P =1500000;
static int T =2200000;
P+=dP*SFact;
T-=dT*SFact;
if(P>2300000)P=2300000;
if(P<500000) P=500000;
if(T>2400000)T=2400000;
if(T<1900000)T=1900000;
Set_Pwm(T,P);

}
int main()
{
	init();
	int ipL,ipR;
	while(true)
	{
	CPoint A=TFind();
	ACCPWM(A.x,A.y);
	}
	
}