#include "MotherShip.h"
#include "Tuning.h"
#include <vector>
//uncomment next when we have block detection or shooting to turn off sleep delays
//#define BLOCKDETECT
using namespace std;
#define UNUSED
/*Class Navigator: Navigates*/
#ifdef TRUE
//constants
const int _S_DR = 80, _S_TR=127;
//it take many tools to build a robot, math is only one of them
typedef struct _V3 {int a; int b; int c;} V3;
void echo(const char * F,const char * V)
{
FILE * A=fopen(F,"ab");
if(A==0)return;
fprintf(A,V);
fclose(A);
}
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
	ADC_DMA * APIN;
public:
	Shooter()
	{
		APIN=new ADC_DMA;
		cap = new VideoCapture(0);
		Mat mat;
		cap->read(mat);
		Pan =new PWMAccumulator(PAN_PWM);
		Tilt=new PWMAccumulator(TILT_PWM);
		FlyWheels=new c_bin_io(FW_GPIO);
		LinActuate=new c_bin_io(LA_GPIO);
		Pan->LowLimit  ( 842000);
		Pan->HighLimit (2186000);
		Tilt->LowLimit (1958000);
		Tilt->HighLimit(2186000);
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
	{
		//	cout<<"NO CALL?";
		//	cout<<"??"<<endl;
		CPoint cvoid = {-1 -1};
		if(!cap->isOpened()){cout<<"ERROR: no camera";return cvoid;}
		Mat mat,mat2;
		//	cout << "Pre Cap";
		//	cout << endl;	
		cap->read(mat);	
		//		(*cap) >> mat2;
		//	cout << "Post Cap";
		//	cout << "FAIL_CONVERT?";
		cvtColor(mat, mat2, CV_BGR2HLS);
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
			/*
			int a=(OFrame(i-FW,j)[0] + OFrame(i,j+FW)[0] + 
			OFrame(i+FW,j)[0] + OFrame(i,j-FW)[0]-4*OFrame(i,j)[0]);
			if(a<0)a+=256;
			Frame(i,j)[0]=a;
			Frame(i,j)[1]=OFrame(i,j)[1];//(OFrame(i-FW,j)[1] + OFrame(i,j+FW)[1] + 
			//OFrame(i+FW,j)[1] + OFrame(i,j-FW)[1]-4*OFrame(i,j)[1]);
			
			Frame(i,j)[2]=OFrame(i,j)[2];//(OFrame(i-FW,j)[2] + OFrame(i,j-FW)[2] + 
			*/
			//OFrame(i+FW,j)[2] + OFrame(i,j+FW)[2]-4*OFrame(i,j)[2]);
			
			if((OFrame(i,j)[0] < 25 && OFrame(i,j)[0] > 12))// && OFrame(i,j)[0] < 20 )
			{  //the value is red hue (20 > h | h > 240)
				if(OFrame(i,j)[1] > 70  &&  OFrame(i,j)[2] > 170 && OFrame(i,j)[1]!=255)//&& OFrame(i,j)[2] < 180    	)
						//OFrame(i,j)[1] > 50 && OFrame(i,j)[1] < 200	)//off-white can be red with low saturation and high luminance. something similar for black
				{
					
					SX+=j;
					SY+=i;
					ct++;
					//	Frame(i,j)[0]=0;
					//	Frame(i,j)[1]=110;
					//	Frame(i,j)[2]=255;
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
		/*
		cvtColor(mat, mat2, CV_HLS2BGR);
	if(SX > 4 && SX < 356 && SY > 4 && SY < 236)
	for(i=-3;i<=3;i++)
	for(j=-3;j<=3;j++)
	{OFrame(SY+i,SX+j)[0]=0; OFrame(SY+i,SX+j)[1]=255; OFrame(SY+i,SX+j)[2]=0;} //green (the gotton format is BGR here)
	
		char op[255];
		static int PK=0;
		sprintf(op,"_%d.jpg",PK++);
		
		imwrite(op,mat2);
		*/
		CPoint RT;
		RT.x=SX;
		RT.y=SY;
		return RT;
	}
	bool Valid()
	{
		//AIN5
		APIN->Poll();
		double V = APIN->AIN(5);
		V=V/62;
		cout << V;
		if(V > MIN_TARG_DIST && V < MAX_TARG_DIST)
		return true;
		else
		return false;
	}
	void Action()
//time.h
	{    //ti.com/ieeesec
		time_t timer1,timer2;
		struct tm tax;
		double secs;
		time(&timer1);
		
		Mat mat;
		cap->read(mat);
		cap->read(mat);
		cap->read(mat);
		cap->read(mat);
		cap->read(mat);
		cap->read(mat);
		CPoint Aim;
		double Ex=0,Ey=0;
		{
NOTVALID:
			Aim.x=0;
			Aim.y=0;
			Tilt->set    (( (2400000)   +
			(1900000)*2)/3);
			Pan->set    (( ( 500000)*3   +
			(2300000)*2)/5);
			while((abs(Aim.x - (CX_SCN))>CX_EPS/2)||(abs(Aim.y - (CY_SCN))>CY_EPS/2))
			{
				Aim=TFind();
				Ex=(Aim.x-(CX_SCN));
				Ey=(Aim.y-(CY_SCN));
				if(Ex<40)Ex=1*Ex;else if(Ex<90) Ex=3*Ex; else Ex=5*Ex;
				if(Ey<40)Ey=1*Ey;else if(Ey<90) Ex=3*Ex; else Ey=5*Ex;
				Pan->accumulate(Ex*.5);
				Tilt->accumulate(Ey);
			time(&timer2);
			secs=difftime(timer2,timer1);
			if(secs > 23)break;
			}
			//cout << "pow";
			ShootGun();
		}
		
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
	//Back
	GPIO_READ_DMA * LMBR;
	GPIO_READ_DMA * LMBC;
	GPIO_READ_DMA * LMBL;
	//Front
	GPIO_READ_DMA * LMFR;
	GPIO_READ_DMA * LMFC;
	GPIO_READ_DMA * LMFL;
	//From Arduino
	GPIO_READ_DMA * BLOCK_DET;
	GPIO_READ_DMA * START_SIG;
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
		START_SIG=new GPIO_READ_DMA(0,20);
		BLOCK_DET=new GPIO_READ_DMA(1,13);
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
		
	echo(LED1T,"none");
	echo(LED2T,"none");
	echo(LED3T,"none");
	echo(LED0T,"none");
	echo(LED1,"0");
	echo(LED2,"0");
	echo(LED3,"0");
	echo(LED0,"0");
	}
	void Start()
	{
	LED_PWR->Write(true);
	while(!START_SIG->Read());
		echo(LED0T,"heartbeat");
	}
	bool BLOCK_DETECT()
	{
		bool R = BLOCK_DET->Read();
	}
	void BLED_DETECT() //are we start signal or block?
	{
		cout << "TS\n";
		double x[100];
		double afirst=0;
		int i,ac=0;
		for(i=0;i<100;i++)
		x[i]=0;
		for(i=0;i<100;i++)
		{
			APIN->Poll();
			double tmp = APIN->DiffIn();
			afirst+=tmp;
			x[99-i]=tmp;
		}
		afirst/=100;
		cout << afirst << endl;
		//cout << "SET\n";
		while(true)
		{
			
			double a=0;
			APIN->Poll();
			for(i=99;i>0;i--)
			{x[i]=x[i-1];a+=x[i];}
			x[0]=APIN->DiffIn();
			a+=x[0];
			a/=100;
			//cout << a << endl;
			//cout << a << endl;
			usleep(10000);
			if(a > 1100)
			{
			//cout << a << endl;
			usleep(10000);
				ac++;
				//if(ac>20)
				{
					return;
				}
			}
			else
			ac=0;
		}
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
	void NoDetect()
	{
	LED_PWR->Write(false);
	}
	void LeaveBlock()
	{
			LED_PWR->Write(false);
			Forward();
			bool a=LMFC->Read(),b=LMFL->Read(),c=LMFR->Read();
			Drive->Drive(_S_DR,_S_DR);
//			usleep(1000000);
//
			while(a && b && c)
			{
				a=LMFC->Read();b=LMFL->Read();c=LMFR->Read();
			}
	}
	int aMSR,aMSL;
	void Forward()
	{
		Drive->Drive(_S_DR,_S_DR);
		aMSR=aMSL=_S_DR;
		while(true)
		{
			bool a=LMFC->Read(),b=LMFL->Read(),c=LMFR->Read();
			bool bd=BLOCK_DETECT();
			//bd=(Block_Detect)
			if((a&&b&&c))
			{
				Drive->Drive(0,0);
				sleep(1);
				return;//goto FL;
			}
			//if(block detected) return
			if(a)
			{
				Drive->Drive(_S_DR,_S_DR);
				aMSR=aMSL=_S_DR;
			}
			else if(b)
			{
				Drive->Drive(0,_S_TR);
				aMSR=0;
				aMSL=_S_TR;
			}
			else if(c)
			{
				Drive->Drive(_S_TR,0);
				aMSL=0;
				aMSR=_S_TR;
			}
		}
	}
		void Forward2()
	{
		Drive->Drive(_S_DR,_S_DR);
		aMSR=aMSL=_S_DR;
		while(true)
		{
			bool a=LMFC->Read(),b=LMFL->Read(),c=LMFR->Read();
			bool bd=BLOCK_DETECT();
			//bd=(Block_Detect)
			if((bd))
			{
				//usleep(300000);
				Drive->Drive(0,0);
				sleep(1);
				return;//goto FL;
			}
			//if(block detected) return 
			if(a)
			{
				Drive->Drive(_S_DR,_S_DR);
				aMSR=aMSL=_S_DR;
			}
			else if(b)
			{
				Drive->Drive(0,_S_TR);
				aMSR=0;
				aMSL=_S_TR;
			}
			else if(c)
			{
				Drive->Drive(_S_TR,0);
				aMSL=0;
				aMSR=_S_TR;
			}
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
		int er=0;
		while(!(LMBC->Read() || LMBR->Read() || LMBL->Read()))
		{
			if(er<100)
			Drive->Drive(-_S_TR,0);
			if(er>100)
			Drive->Drive(0,-_S_TR);
			if(er>200)er=0;
			usleep(1000);
			er++;
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
	void DELB()
	{
	Drive->Drive(-_S_DR,-_S_DR);
	usleep(500000);
	Drive->Drive(0,0);
	}
	void DEL()
	{
	Drive->Drive(_S_DR,_S_DR);
	usleep(200000);
	Drive->Drive(0,0);
	}
	void ReverseLine()
	{
		//int er=50;
		Drive->Drive(-_S_DR,-_S_DR);

//---------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------

		usleep(500000);
		while(!(LMBC->Read() || LMBR->Read() || LMBL->Read()))
		{
			Drive->Drive(-aMSR,-aMSL);
			break;
			//if(er<5)
			//Drive->Drive(-_S_TR,0);
			//if(er>5)
			//Drive->Drive(0,-_S_TR);
			//if(er>10)er=0;
			//usleep(1000);
			//er++;
		} 
		{

		bool d=LMFC->Read(),e=LMFL->Read(),f=LMFR->Read();
		while(d && e && f)
		{
			bool a=LMBC->Read(),b=LMBL->Read(),c=LMBR->Read();
			d=LMFC->Read();e=LMFL->Read();f=LMFR->Read();
			if(a)
			Drive->Drive(-_S_DR,-_S_DR);
			else if(b)
			Drive->Drive(0,-_S_TR);
			else if(c)
			Drive->Drive(-_S_TR,0);
		}
		}
		while(true)
		{
			bool d=LMFC->Read(),e=LMFL->Read(),f=LMFR->Read();
			bool a=LMBC->Read(),b=LMBL->Read(),c=LMBR->Read();
			if(d&&e&&f)
			{
				Drive->Drive(0,0);
				sleep(1);//can speed up without
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


void Reversecrv()
	{
		//int er=50;
		Drive->Drive(-_S_DR,-_S_DR);
		usleep(200000);
		while(!(LMBC->Read() || LMBR->Read() || LMBL->Read()))
		{
			Drive->Drive(-aMSR,-aMSL);
			break;
			//if(er<5)
			//Drive->Drive(-_S_TR,0);
			//if(er>5)
			//Drive->Drive(0,-_S_TR);
			//if(er>10)er=0;
			//usleep(1000);
			//er++;
		} 
		{

		bool d=LMFC->Read(),e=LMFL->Read(),f=LMFR->Read();
		while(d && e && f)
		{
			bool a=LMBC->Read(),b=LMBL->Read(),c=LMBR->Read();
			d=LMFC->Read();e=LMFL->Read();f=LMFR->Read();
			if(a)
			Drive->Drive(-_S_DR,-_S_DR);
			else if(b)
			Drive->Drive(0,-_S_TR);
			else if(c)
			Drive->Drive(-_S_TR,0);
		}
		}
		while(true)
		{
			bool d=LMFC->Read(),e=LMFL->Read(),f=LMFR->Read();
			bool a=LMBC->Read(),b=LMBL->Read(),c=LMBR->Read();
			if(d&&e&&f)
			{
				Drive->Drive(0,0);
				sleep(1);//can speed up without
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
		//Drive->Drive(_S_TR,_S_TR);
		Drive->Drive(_S_DR,_S_DR);
			usleep(100000);
			bool a=LMFC->Read(),b=LMFL->Read(),c=LMFR->Read();
			while(a && b && c)
			{
				a=LMFC->Read();b=LMFL->Read();c=LMFR->Read();
			}
		Drive->Drive(_S_TR,-_S_TR*.8);
		usleep(500000);
		//bool a=LMFC->Read(),b=LMFL->Read(),c=LMFR->Read();
		//bool d=LMBC->Read(),e=LMBL->Read(),f=LMBR->Read();
		while(!LMFC->Read());
		Drive->Drive(0,0);
		//usleep(100000);
		//Drive->Drive(_S_DR,_S_DR);
		//while(!LMFC->Read());
		//Drive->Drive(0,0);
		//usleep(10000);
		//Drive->Drive(_S_DR,_S_DR);
		usleep(1000000);
		return;//goto FWDS;//FWD;
	}
	void UTurn()
	{
		//Drive->Drive(_S_TR,_S_TR);
		//Drive->Drive(_S_DR,_S_DR);
			//usleep(100000);
			//bool a=LMFC->Read(),b=LMFL->Read(),c=LMFR->Read();
			//while(a && b && c)
			//{
			//	a=LMFC->Read();b=LMFL->Read();c=LMFR->Read();
			//}
		Drive->Drive(_S_TR,-_S_TR*.8);
		usleep(800000);
		//bool a=LMFC->Read(),b=LMFL->Read(),c=LMFR->Read();
		//bool d=LMBC->Read(),e=LMBL->Read(),f=LMBR->Read();
		while(!LMFC->Read());
		Drive->Drive(0,0);
		//usleep(100000);
		//Drive->Drive(_S_DR,_S_DR);
		//while(!LMFC->Read());
		//Drive->Drive(0,0);
		//usleep(10000);
		//Drive->Drive(_S_DR,_S_DR);
		usleep(1000000);
		return;//goto FWDS;//FWD;
	}
	//RTR:
	void RightTurn()
	{
		
		Drive->Drive(-_S_TR*.9,-_S_TR*.9);
		usleep(500000);
		Drive->Drive(-_S_TR,_S_TR);
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
		Drive->Drive(-_S_TR,_S_TR);
		usleep(500000);
		//bool a=LMFC->Read(),b=LMFL->Read(),c=LMFR->Read();
		//bool d=LMBC->Read(),e=LMBL->Read(),f=LMBR->Read();
		while(!LMFC->Read());
		Drive->Drive(0,0);
		//usleep(100000);
		//Drive->Drive(_S_DR,_S_DR);
		//while(!LMFC->Read());
		//Drive->Drive(0,0);
		//usleep(10000);
		//Drive->Drive(_S_DR,_S_DR);
		usleep(100000);
		return;//goto ENDX;//FWD;	ENDX:;
	}
	void Stop()
	{
		//ENDX:
		while(START_SIG->Read());
		echo(LED0T,"none");
		echo(LED1,"0");
	}

};
#endif
/*int main(): Waits for LEDS to turn on and begins execution*/
#ifdef TRUE
int main()
{
	STRT:
	Navigator X;
	Shooter Y;
	X.Start();
	//X.NoDetect();
	//X.Forward();
	//X.LeftTurn();
	//X.Forward2();
	//X.LeftTurn();
	//X.Forward();
	//X.NoDetect();
	//X.Forward();
	//X.LeftTurn();
	//X.Forward();
	//X.ReverseLine();
	//X.RightTurnLine();
	//return 0;
	//X.NoDetect();
	//X.Forward2();
	//X.DELB();
	//X.UTurn();
	//X.Forward();
	//return 0;
	
	X.BLED_DETECT();
	cout << "ImFree";
	//get out of the shooting block (may need a delay added)
	X.LeaveBlock();
	
	//FIST NAV
	//GOTO Fist Shot
	X.Forward();
	X.LeftTurn();
	X.Forward2();
	//shoot
	//X.DEL();
	Y.Action();
	//sleep(1);
	//back to line

	X.DELB();
	X.UTurn();
	X.DEL();
	X.Forward();
	X.LeftTurn();
	//2ND NAV
	X.Forward();
	X.LeftTurn();
	X.Forward2();
	//X.DEL();
	Y.Action();
	//sleep(1);
	X.DELB();
	X.UTurn();
	X.DEL();
	X.Forward();
	X.LeftTurn();
	//3RD NAV
	X.Forward();
	X.LeftTurn();
	X.Forward2();
	//X.DEL();
	Y.Action();
	//sleep(1);
	X.DELB();
	X.UTurn();
	X.DEL();
	X.Forward();
	X.LeftTurn();
	//GOTO FINISH
	X.Forward();
	X.Stop();
	//goto STRT;

	//Sleep
}
#endif	