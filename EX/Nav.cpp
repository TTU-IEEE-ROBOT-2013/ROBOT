#include "Main.h"
#include "Beagle.h"
#include <unistd.h>
#include <bitset>
using namespace std;
namespace NAVIGATION
{
	//keep global variables in unique namespaces to avoid duplication (same with functions not called from outside).
	c_bin_io * LineVars[10];
	cRGBC * CT;
	int Lpins[]={LT1,LT2,LT3,LT4,LT5,LT6,LT7,LT8,LT9,LT10};
	c_bin_io  * LeftDirection, * RightDirection, * Lon, *RELeft, *RERight;
	int IRightSpeed;
	int ILeftSpeed;
	bool Move();
	int LineCheck();
	void Count1()
	{
	double a=1.3,b=1.3,c=1.3;
	//bool c=false;
	if(RELeft->Read())
		while(RELeft->Read())
			a=b*c; //waist time
	while(RELeft->Read()==false)
		a=b*c; //waist time
		//rotary encoder (will need specifics later(points per turn))
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
		}else if(CT->BlueRead() < NAV_BL && // Red
		CT->RedRead()>NAV_RH &&
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
		LeftDirection->Write(false);
		RightDirection->Write(true);
		IRightSpeed = ILeftSpeed = TurnSpeed;
		Move();
		wait(RotationsPer90dTurn);//#of rotations to do a turn
		RightDirection->Write(false);
		while(LineCheck()==0xFFFF)
		Move();
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
		wait(RotationsPer90dTurn);//#of rotations to do a turn
		LeftDirection->Write(false);
		while(LineCheck()==0xFFFF)
		Move();
	}
	bool Move() //set movement speed.
	{
		//8=normal (will set default in Tuning.h)
		int actspeed= ILeftSpeed * MAX_DUTY / 16;
		WritePWM(LeftDrive,PERIOD,actspeed);
		actspeed=IRightSpeed * MAX_DUTY / 16;
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
void Start()
{

while(!LineVars[5]->Read())
{
sleep(.1);
}
}
void SeeRed()
{
		double a=1.3,b=1.3,c=1.3;
		while(CTest() != 2)
		{
			LineCheck();
			Move();
			a=b*c; //waist time
		}
}
void NavEnd()
{
	int i;
	//EnablePWM(LeftDrive);
	//EnablePWM(RightDrive);
	for(i=0;i<10;i++)
	{
		delete LineVars[i];
		//LineVars[i]->SetIP();
	}
	delete LeftDirection;//=new c_bin_io(LeftExite);
	//LeftDirection->SetOP();
	delete RightDirection;//=new c_bin_io(RightExite);
	//RightDirection->SetOP();
	delete CT;//=new cRGBC();
	delete Lon;//=new c_bin_io(LSTART);
	//Lon->SetOP();
	delete RELeft;//=new c_bin_io(REL_);
	//RELeft->SetIP();
	delete RERight;//=new c_bin_io(RER_);
	//RERight->SetIP();
	//Lon->Write(false);
}
void FollowLine()
{
Lon->Write(true);
while(LineCheck()==0)
Move();//skip "first" line(as it is part of starting box)
while(LineCheck()==0)
Move();
Count2Blue();//shot 1
while(LineCheck()==0)
Move();
Count2Blue();
while(LineCheck()==0);
Count2Blue();
SeeRed();
Lon->Write(false);
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
		LineVars[i]->SetIP();
	}
	LeftDirection=new c_bin_io(LeftExite);
	LeftDirection->SetOP();
	RightDirection=new c_bin_io(RightExite);
	RightDirection->SetOP();
	CT=new cRGBC();
	Lon=new c_bin_io(LSTART);
	Lon->SetOP();
	RELeft=new c_bin_io(REL_);
	RELeft->SetIP();
	RERight=new c_bin_io(RER_);
	RERight->SetIP();
	Lon->Write(false);//ensure our lights are out
}
void Navigate()
{
Start();
FollowLine();
NavEnd();
}