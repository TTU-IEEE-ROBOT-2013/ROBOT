#include "MotherShip.h"

namespace MotherShip
{
	cRGBC BlockType;
	//c_bin_io FlyWheels(FW_GPIO);
	//c_bin_io LinActuate(LA_GPIO);
	//PWMAccumulator Pan(PAN_PWM);
	//PWMAccumulator Tilt(TILT_PWM);
	DRV Drive();
	
}
using namespace MotherShip;
/*Class Navigator: Navigates*/
#ifdef TRUE
class Navigator
{
private:
	//PID PX(NAV_P,NAV_I,NAV_D);//simpler times
	ACON PFIR(); //discreate self-adapting control system(No Time Funcs Req'd)
	PID  FailSafePID();
public:
	
};
#endif
/*Class Shooter: Shoots the target*/
#ifdef TRUE
class Shooter
{
private:
	VideoCapture cap(0);
	PWMAccumulator Pan(PAN_PWM);
	PWMAccumulator Tilt(TILT_PWM);
	c_bin_io FlyWheels(FW_GPIO);
	c_bin_io LinActuate(LA_GPIO);
public:
	Shooter()
	{
		Pan.LowLimit  ( 900000);
		Pan.HighLimit (1500000);
		Tilt.LowLimit ( 900000);
		Tilt.HighLimit(1500000);
		Pan.Set    (( ( 900000)   +
					  (1500000))/2);
		Pan.Set    (( ( 900000)   +
					  (1500000))/2);
		//..
		FlyWheels.SetOP();
		FlyWheels.Write(false);
		LinActuate.SetOP();
		LinActuate.Write(LA_CLOSED);
		//Note: if it does not initialize properly, may need to change.
		if(!cap.isOpened()){cout<<"ERROR: no camera";return -1;}
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
		void Action()
		{
		//include the timer made earlier to wait for 3 seconds
		CPoint Aim;
		Aim.x=0;
		Aim.y=0;
		while((Aim.x - CX_SCN>CX_EPS)&&(Aim.y - CY_SCN>CY_EPS))
		{
			Aim=TFind();
			int Ex,Ey;
			Ex=(Aim.x-CX_SCN);
			Ey=(Aim.y-CY_SCN);
			Pan.Accumulate(Ex);
			Tilt.Accumulate(Ey);
		}
		
		}
		void ShootGun()
		{
			
		}
	};
	//Vestigial Code
	#ifndef TRUE
	int NOT_main_157663()
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
					int dx = (Frame(i-1,j)[2]-Frame(i+1,j)[2]);
					int dy = (Frame(i,j-1)-Frame(i,j+1));
					if(dx<0)dx*=-1;
					if(dy<0)dy*=-1;
					int dxy = dx+dy;
					SX+=i*dxy;
					SY+=j*dxy;
					ct+=dxy;
					Frame(i,j)[1]=255; //make it different
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

#endif
#endif
		
		/*int main(): Waits for LEDS to turn on and begins execution*/
#ifdef TRUE
		int main()
		{
			double S=TestIn();
			WriteF(LED0,"1");
			while(S*EZ_APPROXIMATION > TestIn())
			{}

		}
#endif	