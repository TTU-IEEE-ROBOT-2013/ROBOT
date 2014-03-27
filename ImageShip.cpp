#include "MotherShip.h"
#include <vector>
//uncomment next when we have block detection or shooting to turn off sleep delays
//#define BLOCKDETECT
using namespace std;
#define UNUSED
/*Class Navigator: Navigates*/
#ifdef TRUE
//constants
const int _S_DR = 90, _S_TR=127;
//it take many tools to build a robot, math is only one of them
typedef struct _V3 {int a; int b; int c;} V3;

V3 RGB2HSL(V3 RGB)
{
double a,b,c;
double h,s,l;
a=RGB.a/(1.0*MAX_RGBC_VAL);
b=RGB.b/(1.0*MAX_RGBC_VAL);
c=RGB.c/(1.0*MAX_RGBC_VAL);
double R=a,G=b,B=c;
double mx,mn;
int sc=0;
mx=a;if(b>mx){mx=b;sc=1;}if(c>mx){mx=c;sc=2;}
mn=a;if(b<mn)mn=b;if(c<mn)mn=c;
l=(mx+mn)/2;
if(mn=mx)s=0;
else if(l<=.5)
s=(mx-mn)/(mx+mn);
else if(l>.5)
s=(mx-mn)/(2.0-mx-mn);
switch(sc)
{
case 0:
h=(G-R)/(mx-mn);
break;
case 1:
h=2.0+(B-R)/(mx-mn);
case 2:
h=4.0+(R-G)/(mx-mn);
}
if(h<0)h+=6;
h=h/6;
V3 RT;
RT.a=h*MAX_RGBC_VAL;
RT.b=s*MAX_RGBC_VAL;
RT.c=l*MAX_RGBC_VAL;
return RT;
}
/*Class Shooter: Shoots the target*/
#ifdef TRUE
//VideoCapture cap(0);
class Shooter
{
private:
	VideoCapture * cap;
	PWMAccumulator * Pan;
	PWMAccumulator * Tilt;
	c_bin_io * FlyWheels;
	c_bin_io * LinActuate;
public:
	Shooter()
	{
		cap = new VideoCapture(0);
		Pan =new PWMAccumulator(PAN_PWM);
		Tilt=new PWMAccumulator(TILT_PWM);
		FlyWheels=new c_bin_io(FW_GPIO);
		LinActuate=new c_bin_io(LA_GPIO);
		Pan->LowLimit  ( 500000);
		Pan->HighLimit (2300000);
		Tilt->LowLimit (1900000);
		Tilt->HighLimit(2400000);
		Tilt->set    (( (2400000)   +
		(1900000)*2)/3);
		Pan->set    (( ( 500000)   +
		(2300000))/2);
		//..
		FlyWheels->SetOP();
		FlyWheels->Write(false);
		LinActuate->SetOP();
		LinActuate->Write(LA_CLOSED);
		//Note: if it does not initialize properly, may need to change.
		if(!cap->isOpened()){cout<<"ERROR: no camera";return;}
	}
	CPoint TFind()
	{	char op[255];
		//	cout<<"NO CALL?";
		//	cout<<"??"<<endl;
		CPoint cvoid = {-1 -1};
		if(!cap->isOpened()){cout<<"ERROR: no camera";return cvoid;}
		Mat mat,mat2;
		//	cout << "Pre Cap";
		//	cout << endl;	
		{
		AGAIN:
		
		cap->read(mat);
		cvtColor(mat, mat2, CV_BGR2HLS);
		static int PK=0;
		sprintf(op,"_%d.bmp",PK++);	
		imwrite(op,mat2);
		
		//		(*cap) >> mat2;
		//	cout << "Post Cap";
		//	cout << "FAIL_CONVERT?";
		
		Mat_ <Vec3b> Frame(mat);
		Mat_ <Vec3b> OFrame(mat2);
		cv::Size sks = Frame.size();
		int i,j;
		int SX,SY,ct;
		SX=0;SY=0;ct=0;
		int FW = 5;
		for(i=FW;i<sks.height-FW;i++)
		for(j=FW;j<sks.width-FW;j++)
		{
			
			int a=(OFrame(i-FW,j)[0] + OFrame(i,j+FW)[0] + 
			OFrame(i+FW,j)[0] + OFrame(i,j-FW)[0]-4*OFrame(i,j)[0]);
			if(a<0)a+=256;
			Frame(i,j)[0]=a;
			Frame(i,j)[1]=OFrame(i,j)[1];//(OFrame(i-FW,j)[1] + OFrame(i,j+FW)[1] + 
			//OFrame(i+FW,j)[1] + OFrame(i,j-FW)[1]-4*OFrame(i,j)[1]);
			
			Frame(i,j)[2]=OFrame(i,j)[2];//(OFrame(i-FW,j)[2] + OFrame(i,j-FW)[2] + 
			
			//OFrame(i+FW,j)[2] + OFrame(i,j+FW)[2]-4*OFrame(i,j)[2]);
			
			if((OFrame(i,j)[0] < 25 && OFrame(i,j)[0] > 12))// && OFrame(i,j)[0] < 20 )
			{  //the value is red hue (20 > h | h > 240)
				if(OFrame(i,j)[1] > 70  &&  OFrame(i,j)[2] > 170 && OFrame(i,j)[1]!=255 && Frame(i,j)[0]<20)//off-white can be red with low saturation and high luminance. something similar for black
				{
					
					SX+=j;
					SY+=i;
					ct++;
					OFrame(i,j)[0]=90;
					OFrame(i,j)[1]=140;
					OFrame(i,j)[2]=255;
				}
				
			}
			else
			{
				//OFrame(i,j)[0]=255;
				//OFrame(i,j)[1]=0;
				//OFrame(i,j)[2]=0;
			}

		}
		//		cout << ct;
		//		cout << endl;
		if(ct !=0){
			SX = SX / ct;
			SY = SY / ct;
		}else{SX=SY=-1;}
		//	cout << "SEE SOMETHING?";
		//	cout << endl;
		for(i=0;i<5;i++)
		for(j=0;j<5;j++)
		OFrame(i,j)[0]=255;
		OFrame(i,j)[1]=157;
		OFrame(i,j)[2]=10;
		cvtColor(mat2, mat, CV_HLS2BGR);
	//if(SX > 4 && SX < 356 && SY > 4 && SY < 236)
	//for(i=-3;i<=3;i++)
	//for(j=-3;j<=3;j++)
	//{OFrame(SY+i,SX+j)[0]=0; OFrame(SY+i,SX+j)[1]=255; OFrame(SY+i,SX+j)[2]=0;} //green (the gotton format is BGR here)
	
		
		sprintf(op,"_%d-f.bmp",PK++);
		
		imwrite(op,mat);
		sleep(5);
		goto AGAIN;
		}
		CPoint RT;
		//RT.x=SX;
		//RT.y=SY;
		return RT;
	}

	void Action()
	{
		Tilt->set    (( (2400000)   +
		(1900000)*2)/3);
		Pan->set    (( ( 500000)   +
		(2300000))/2);
		//	cout << "init";
		//	cout << endl;
		//VideoCapture capx(0);
		//	cout << "ERC";
		//	cout << endl;
		//include the timer made earlier to wait for 3 seconds
		CPoint Aim;
		Aim.x=0;
		Aim.y=0;
		int Ex,Ey;
		while((abs(Aim.x - (CX_SCN))>CX_EPS/2)||(abs(Aim.y - CY_SCN)>CY_EPS/2))
		{
			//		cout << "GRABA";
			//		cout << endl;
			Aim=TFind();
			//TMPX(Aim.x,Aim.y,capx);
			
			//cout<<Aim.x<<"|"<<Aim.y;//<<"||"<<CX_SCN<<"|"<<CY_SCN;
			//cout<<endl;
			Ex=(Aim.x-CX_SCN);
			Ey=(Aim.y-CY_SCN);
			//cout<<Ex<<"|"<<Ey;
			//cout<<endl;
			Pan->accumulate(Ex);
			Tilt->accumulate(Ey);
		}
		ShootGun();
		
	}
	void ShootGun()
	{
		FlyWheels->Write(true);
		sleep(1);
		LinActuate->Write(LA_OPEN);
		sleep(4);
		LinActuate->Write(LA_CLOSED);
		sleep(2);
		FlyWheels->Write(false);
		cout << "pow";
	}

};

#endif
class Navigator
{
	//reverse DiffIn for Reverse periods
private:
	//PID PX(NAV_P,NAV_I,NAV_D);//simpler times
	DRV* Drive;
	cRGBC* BFinder;
	c_bin_io * LED_PWR;
	//typically like pointers, but won't do pointer to vector.
	//Back
	GPIO_READ_DMA * LMBR;
	GPIO_READ_DMA * LMBC;
	GPIO_READ_DMA * LMBL;
	//Front
	GPIO_READ_DMA * LMFR;
	GPIO_READ_DMA * LMFC;
	GPIO_READ_DMA * LMFL;
	//
	ADC_DMA * APIN;
	//c_bin_io TLED(LED_GPIO);
	int REV;
	int MSR,MSL;
public:
	Navigator() 
	{
		LMBR=new GPIO_READ_DMA(1,15);
		LMBC=new GPIO_READ_DMA(0,27);
		LMBL=new GPIO_READ_DMA(0,22);
		LMFR=new GPIO_READ_DMA(1,12);
		LMFC=new GPIO_READ_DMA(1,14);
		LMFL=new GPIO_READ_DMA(2,1);
		cout << "TST1" <<endl;
		//TLED.SetIP();
		LED_PWR=new c_bin_io(LED_GPIO);//
		EnableADCs();
		//InitUart4();
		APIN=new ADC_DMA;
		LED_PWR->SetOP();
		LED_PWR->Write(true);
		
		Drive = new DRV();
		BFinder = new cRGBC();
		REV=1;
		MSR=0;
		MSL=0;
	}
	bool BLED_DETECT() //are we start signal or block?
	{
	
	}
	UNUSED void Decelerate()
	{
		//this function will be tweeked (with addition/subtraction of steps)
		//to achive max performance
		Drive->Drive(.5*MSR,.5*MSL);
		usleep(100000);
		Drive->Drive(0*MSR,0*MSR);
	}

	
	void DCrap()
	{
		cout<< "RIGHT LEFT\n";
		//sleep(5);
		cout<< "LEFT";
		Drive->Drive(0,0);
		sleep(5);
		//Drive->Drive(100,100);
		//double * FAIL=0;
		return;
		}
//FWD:
void LeaveBlock()
{
bool a=LMFC->Read(),b=LMFL->Read(),c=LMFR->Read();
Drive->Drive(_S_DR,_S_DR);
usleep(500000);
while(a && b && c)
{
a=LMFC->Read(),b=LMFL->Read(),c=LMFR->Read();
}
}
void Forward()
{
		while(true)
		{
			bool a=LMFC->Read(),b=LMFL->Read(),c=LMFR->Read();
			bool bd=false;
			//bd=(Block_Detect)
			if((a&&b&&c) ||(bd))
			{
			Drive->Drive(0,0);
			sleep(1);
			return;//goto FL;
			}
			//if(block detected) return
			if(a)
			Drive->Drive(_S_DR,_S_DR);
			else if(b)
			Drive->Drive(0,_S_TR);
			else if(c)
			Drive->Drive(_S_TR,0);

			}
			}
//FWDS:
			void useless(){ //same as FWD (only goto was different. gotos became return, so now useless
/*
		while(true)
		{
			bool a=LMFC->Read(),b=LMFL->Read(),c=LMFR->Read();
			if(a&&b&&c)
			{
			Drive->Drive(0,0);
			usleep(500000);

			//sleepgoto REV;
			}
			//if(block detected) goto REV
			if(a)
			Drive->Drive(_S_DR,_S_DR);
			else if(b)
			Drive->Drive(0,_S_TR);
			else if(c)
			Drive->Drive(_S_TR,0);
		}
		*/
		}
//REV:
void Reverse()
{
			//DONT FORGET TO KILL THIS
			//when we get block detection.
			#ifndef BLOCKDETECT
			{
			Drive->Drive(-_S_DR,-_S_DR);
			usleep(1000000);
			}
			#endif
		while(!(LMBC->Read() || LMBR->Read() || LMBL->Read()))
		{
			Drive->Drive(-_S_TR,0);
		} 
		while(true)
		{
			//bool d=LMFC->Read(),e=LMFL->Read(),f=LMFR->Read();
			bool a=LMBC->Read(),b=LMBL->Read(),c=LMBR->Read();
			if(a&&b&&c)
			{
			Drive->Drive(0,0);
			sleep(1);
			return;//goto RTR;
			}
			if(a)
			Drive->Drive(-_S_DR,-_S_DR);
			else if(b)
			Drive->Drive(0,-_S_TR);
			else if(c)
			Drive->Drive(-_S_TR,0);
		}
}
//REVF:
	void ReverseLine()
	{
			#ifndef BLOCKDETECT
			{
			Drive->Drive(-_S_DR,-_S_DR);
			usleep(1000000);
			}
			#endif
		while(!(LMBC->Read() || LMBR->Read() || LMBL->Read()))
		{
			Drive->Drive(-_S_TR,0);
		} 
		while(true)
		{
			bool d=LMFC->Read(),e=LMFL->Read(),f=LMFR->Read();
			bool a=LMBC->Read(),b=LMBL->Read(),c=LMBR->Read();
			if(d&&e&&f)
			{
			Drive->Drive(0,0);
			sleep(1);
			return;//goto RTR;
			}
			if(a)
			Drive->Drive(-_S_DR,-_S_DR);
			else if(b)
			Drive->Drive(0,-_S_TR);
			else if(c)
			Drive->Drive(-_S_TR,0);
		}
		}
			//FL:
void LeftTurn()
{
		Drive->Drive(_S_TR,_S_TR);
		usleep(200000);
		Drive->Drive(_S_TR*.8,-_S_TR*.8);
		usleep(500000);
			//bool a=LMFC->Read(),b=LMFL->Read(),c=LMFR->Read();
			//bool d=LMBC->Read(),e=LMBL->Read(),f=LMBR->Read();
			while(!LMFC->Read());
			Drive->Drive(0,0);
			usleep(100000);
			Drive->Drive(_S_DR,_S_DR);
			//while(!LMFC->Read());
			Drive->Drive(0,0);
			//usleep(10000);
			//Drive->Drive(_S_DR,_S_DR);
			usleep(100000);
			return;//goto FWDS;//FWD;
			}
			//RTR:
void RightTurn()
{
		
		Drive->Drive(-_S_TR*.9,-_S_TR*.9);
		usleep(500000);
		Drive->Drive(-_S_TR*.8,_S_TR*.8);
		usleep(500000);
			//bool a=LMFC->Read(),b=LMFL->Read(),c=LMFR->Read();
			//bool d=LMBC->Read(),e=LMBL->Read(),f=LMBR->Read();
			while(!LMFC->Read());
			Drive->Drive(0,0);
			usleep(100000);
			Drive->Drive(_S_DR,_S_DR);
			//while(!LMFC->Read());
			Drive->Drive(0,0);
			//usleep(10000);
			//Drive->Drive(_S_DR,_S_DR);
			usleep(100000);
			return;//goto ENDX;//FWD;	ENDX:;
}
//RTSR:
void RightTurnLine()
{
		Drive->Drive(_S_TR,_S_TR);
		usleep(200000);
		Drive->Drive(-_S_TR*.8,_S_TR*.8);
		usleep(500000);
			//bool a=LMFC->Read(),b=LMFL->Read(),c=LMFR->Read();
			//bool d=LMBC->Read(),e=LMBL->Read(),f=LMBR->Read();
			while(!LMFC->Read());
			Drive->Drive(0,0);
			usleep(100000);
			Drive->Drive(_S_DR,_S_DR);
			//while(!LMFC->Read());
			Drive->Drive(0,0);
			//usleep(10000);
			//Drive->Drive(_S_DR,_S_DR);
			usleep(100000);
			return;//goto ENDX;//FWD;	ENDX:;
}
	void Stop()
	{
	//ENDX:
	Drive->Drive(0,0);
	}

};
#endif
/*int main(): Waits for LEDS to turn on and begins execution*/
#ifdef TRUE
int main()
{
	Navigator X;
	Shooter Y;
	Y.Action();
	/*
	//get out of the shooting block (may need a delay added)
	X.LeaveBlock();
	
	//FIST NAV
	//GOTO Fist Shot
	X.Forward();
	X.LeftTurn();
	X.Forward();
	//shoot
	Y.Action();
	//back to line
	X.ReverseLine();
	X.RightTurnLine();
	//2ND NAV
	X.Forward();
	X.LeftTurn();
	X.Forward();
	Y.Action();
	X.Reverse();
	X.RightTurn();
	//3RD NAV
	X.Forward();
	X.LeftTurn();
	X.Forward();
	Y.Action();
	X.ReverseLine();
	X.RightTurnLine();
	//GOTO FINISH
	X.Forward();
	//Sleep
	*/
}
#endif	