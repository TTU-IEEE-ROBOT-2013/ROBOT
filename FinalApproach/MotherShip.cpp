#include "MotherShip.h"

namespace MotherShip
{
	cRGBC BlockType;
	//c_bin_io FlyWheels(FW_GPIO);
	//c_bin_io LinActuate(LA_GPIO);
	//PWMAccumulator Pan(PAN_PWM);
	//PWMAccumulator Tilt(TILT_PWM);
	
	
}
using namespace MotherShip;
/*Class Navigator: Navigates*/
#ifdef TRUE
#define USE_PID //uncomment to use PID, comment to go Adaptive
#ifdef USE_PID
#define CCLR CTL->Clear();
#else
#define CCLR ;
#endif
class Navigator
{
	//reverse DiffIn for Reverse periods
private:
	//PID PX(NAV_P,NAV_I,NAV_D);//simpler times
	#ifndef USE_PID
	ACON* CTL; //discreate self-adapting control system(No Time Funcs Req'd)
	#else
	PID* CTL; //someone can set the weights appropriately
	#endif
	DRV* Drive;
	cRGBC* BFinder;
	int REV;
	int MSR,MSL;
public:
	Navigator()
	{
		#ifndef USE_PID
		CTL=new ACON();; //discreate self-adapting control system(No Time Funcs Req'd)
		#else
		CTL=new PID(1,1,2); //someone can set the weights appropriately
		#endif
		Drive = new DRV();
		BFinder = new cRGBC();
		REV=1;
		CCLR;
	}
	void Decelerate()
	{
		//this function will be tweeked (with addition/subtraction of steps)
		//to achive max performance
		Drive->Drive(.5*MSR,.5*MSL);
		usleep(20000);
		Drive->Drive(0*MSR,0*MSR);
	}

	
	void DCrap()
	{
		MSR=MSL=100;
		Drive->Drive(100,100);
		sleep(10);
		Decelerate();
	}
	void FWD_Turn()
	{
		//decelerate should ensure we are just a little beyond the block
		Drive->Drive(TurnSpeedT); //TurnSpeedT is a multi-variable macro
		double NOT_THERE = 10;
		double IsThere=0;
		while(IsThere<NOT_THERE)
		IsThere=TestIn();
		NOT_THERE=Test2In();
		Drive->Drive(TurnSpeedT2);
		usleep(1000);
		Drive->Drive(0,0);
		Decelerate();//we cheat a little here.
	}
	void REV_Turn()
	{
		Decelerate();
	}
	int FWD_Intersect(bool ETurn) //0 for intersect, 1 for block
	{
		double dP;
		CCLR;
		while(true)
		{
			dP=DiffIn();
			if(FWD_FindBlock())
			{
				Decelerate();
				return 1;
			}
			if(dP==TURN_TIME)
			{
				if(ETurn)
				{
					Decelerate();
					FWD_Turn();
					return 0;
				}
				else
				return 0;
			}
			if(dP==OUT_OF_BOUNDS)
			{
				/*MAJOR ERROR (TEH hard part)*/
				//for now, just stop and avoid wheel damaging.
				Decelerate();
			}
			//at last, special cirumstances out, lets go
			double dV = CTL->Exec(dP); //apply control system
			MSR=AVG_SPEED+SPD_FACT*dV;
			MSL=AVG_SPEED-SPD_FACT*dV;
			Drive->Drive(MSR,MSL);
		}
		
	}
	int REV_Intersect()
	{
		double dP;
		CCLR;
		while(true)
		{
			dP=DiffIn();
			if(FWD_FindBlock())
			{
				Decelerate();
				return 1;
			}
			if(dP==TURN_TIME)
			{
				Decelerate();
				REV_Turn();
				return 0;
			}
			if(dP==OUT_OF_BOUNDS)
			{
				/*MAJOR ERROR (TEH hard part)*/
				//for now, just stop and avoid wheel damaging.
				Decelerate();
			}
			//at last, special cirumstances out, lets go
			double dV = -CTL->Exec(dP); //apply control system
			MSR=AVG_SPEED+SPD_FACT*dV;
			MSL=AVG_SPEED-SPD_FACT*dV;
			Drive->Drive(-MSR,-MSL);
		}
		
	}
	bool FWD_FindBlock()
	{
		static int BPos = 0;
		int red=  BFinder->RedRead(  );
		int green=BFinder->GreenRead();
		int blue= BFinder->BlueRead( );
		if(IS_BLOCK(red,green,blue))
		{
			return true;
		}
		return false;
	}
		bool NavigateToFirstBlock()
	{
		#ifdef USE_PID
		CTL->Clear();
		#endif
		REV=1;
		FWD_Intersect(false);
		FWD_Intersect(true);
		FWD_Intersect(false);
		REV=-1;
		return true;
	}
	bool NavigateToNextBlock()
	{
		#ifdef USE_PID
		CTL->Clear();
		#endif
		if(REV=-1)
		{
			REV_Intersect();
			int P=FWD_Intersect(true);
			if(P==1)
			{
				return false;
			}
			FWD_Intersect(false);
			REV=-1;
			return true;
		}
		else
		{
			FWD_Intersect(true);
			int P=FWD_Intersect(false);
			if(P==1)
			{
				return false;
			}
			return true;
		}
	}
};
#endif
/*Class Shooter: Shoots the target*/
#ifndef TRUE
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
	{
		cout<<TestIn()<<endl;
	}
	

}
#endif	