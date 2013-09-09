#include "Main.h"
#include "Beagle.h"
#include <bitset>
using namespace std;
namespace NAVIGATION
{
	//keep global variables in unique namespaces to avoid duplication (same with functions not called from outside).
	c_bin_io * LineVars[10];
	cRGBC * CT;
	int Lpins[]={LT1,LT2,LT3,LT4,LT5,LT6,LT7,LT8,LT9,LT10};
	c_bin_io  * LeftDirection, *RightDirection;
	int IRightSpeed;
	int ILeftSpeed;
	void Count1()
	{
		//count 1 point on encoder (using while)
		//need more info about encoder to implement
	}
	void wait(float T)
	{
		//this will pause further execution until specified # of wheel turns.
		int pk = PointsOnWheel * T;
		for(int i;i<pk;i++)
			Count1();
	}
	int CTest() //test color
	{
		if(CT->BlueRead() > NAV_BH && // BLUE
		CT->RedRead()<NAV_RL &&
		CT->GreenRead()<NAV_GL)
		{
			return 1;
		}else if(CT->BlueRead() > NAV_BL && // Red
		CT->RedRead()<NAV_RH &&
		CT->GreenRead()<NAV_GL)
		{
			return 2;
		}else
		{
			return 0;
		}
	}
	void TurnRight()
	{

	}
	void Return(int Steps)
	{
		int i;int tp;
		for(i=0;i<Steps;i++)
		{
			Count1();
			LineCheck();
			tp=ILeftSpeed;
			ILeftSpeed=IRightSpeed;
			IRightSpeed=tp;
		}
		TurnRight();
		//now (just now) does it return to Navigate.
	}
	void Count2Blue()
	{
		int DT=0;
		while(CTest() != 1)
		{
			Count1();
			DT++;
			LineCheck();
			Move();
		}
		ShootTarget();
		Return(DT);
	}
	void TurnLeft()
	{
		LeftDirection->Write(true);
		RightDirection->Write(false);
		IRightSpeed = ILeftSpeed = TurnSpeed;
		Move();
		wait(2);
		//sleep untill done
		LeftDirection->Write(false);
		Move();
		wait(1);
	}
	bool Move() //set movement speed.
	{
		//8=normal (will set default in Tuning.h)
		int actspeed= LeftSpeed * MAX_DUTY / 16;
		WritePWM(LeftDrive,period,actspeed);
		actspeed=RightSpeed * MAX_DUTY / 16;
	}
int LineCheck()
{
	char tp[10];
	int i=0;
	for(i=0;i<10;i++)
		tp[i]=LineVars[i]->Read();
	double avg=0;int ct=0;
	for(i=0;i<10;i++)
	{
		if(tp[i])
		{
		avg+=((double)(i+1)/10.0);
		ct++;
		}
	}
	if(ct>NavMaxLineWidth)
	{
		return 0xFFFF;
		//we have hit a turning point.
	}
	else
	{
		avg-=5;
		int iavg=(int)avg;
		switch(iavg)
		{
		case 0:
		case 1:
		case -1:
			ILeftSpeed=8;
			IRightSpeed=8;
			break;
		case 2://negative=left,positive=right;
			ILeftSpeed=9;
			IRightSpeed=8;
			break;
		case 3:
			ILeftSpeed=11;
			IRightSpeed=8;
			break;
		case 4:
			ILeftSpeed=13;
			IRightSpeed=8;
			break;
		case 5:
			ILeftSpeed=15;
			IRightSpeed=8;
			break;
		case -2://negative=left,positive=right;
			ILeftSpeed=8;
			IRightSpeed=9;
			break;
		case -3:
			ILeftSpeed=8;
			IRightSpeed=11;
			break;
		case -4:
			ILeftSpeed=8;
			IRightSpeed=13;
			break;
		case -5:
			ILeftSpeed=8;
			IRightSpeed=15;
			break;
		}
		return 0;
	}
}


}
using namespace NAVIGATION;
//last subroutines in file.
void Nav_init()
{
	int i;
	EnablePWM(LeftDrive);
	EnablePWM(RightDrive);
	for(i=0;i<10;i++)
	{
		LineVars[i] = new c_bin_io(Lpins[i]);
	}
	LeftDirection=new c_bin_io(LeftExite);
	RightDirection=new c_bin_io(RightExite);
	CT=new cRGBC();
}
void Navigate()
{

}