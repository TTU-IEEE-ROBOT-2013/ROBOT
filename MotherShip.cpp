#include "MotherShip.h"
#include <vector>
using namespace std;
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
		#ifndef USE_PID
		CTL=new ACON();; //discreate self-adapting control system(No Time Funcs Req'd)
		#else
		CTL=new PID(1,1,2); //someone can set the weights appropriately
		#endif
		APIN=new ADC_DMA;
		LED_PWR->SetOP();
		LED_PWR->Write(true);
		
		
		// cout << "TST1" <<endl;
		// LineFW1.push_back(new c_bin_io(R1));
		// LineFW1.push_back(new c_bin_io(C1));
		// LineFW1.push_back(new c_bin_io(L1));
	
		// cout << "TST1" <<endl;
	
		// #define L LineFW1
		// L[0]->SetIP();
		// L[1]->SetIP();
		// L[2]->SetIP();
		// #undef L
		
		
		// cout << "TST1" <<endl;
		// LineFW2.push_back(new c_bin_io(R2));
		// LineFW2.push_back(new c_bin_io(C2));
		// LineFW2.push_back(new c_bin_io(L2));
		
		
		// cout << "TST1" <<endl;
		// #define L LineFW2
		// L[0]->SetIP();
		// L[1]->SetIP();
		// L[2]->SetIP();
		// #undef L
		
		//cout << "TST1" <<endl;
		
		Drive = new DRV();
		BFinder = new cRGBC();
		REV=1;
		MSR=0;
		MSL=0;
		CCLR;
	}
	void Decelerate()
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
		while(false)
		{
			int R,L;
			cin >> R >> L;
			Drive->Drive(R,L);
		}
		//sleep(5);
		cout<< "LEFT";
		Drive->Drive(0,0);
		sleep(5);
		Drive->Drive(80,80);
		double * FAIL=0;
		const int _S_DR = 80, _S_TR=100;
FWD:
		while(true)
		{
			bool a=LMFC->Read(),b=LMFL->Read(),c=LMFR->Read();
			if(a&&b&&c)
			{
			Drive->Drive(0,0);
			sleep(1);
			goto REV;
			}
			if(a)
			Drive->Drive(_S_DR,_S_DR);
			else if(b)
			Drive->Drive(0,_S_TR);
			else if(c)
			Drive->Drive(_S_TR,0);
		}
REV:
		while(true)
		{
			bool a=LMBC->Read(),b=LMBL->Read(),c=LMBR->Read();
			if(a&&b&&c)
			{
			Drive->Drive(0,0);
			sleep(1);
			goto FWD;
			}
			if(a)
			Drive->Drive(-_S_DR,-_S_DR);
			else if(b)
			Drive->Drive(0,-_S_TR);
			else if(c)
			Drive->Drive(-_S_TR,0);
		}
	}
};
#endif
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
		(1900000))/2);
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
			int a=(OFrame(i-FW,j)[0] + OFrame(i,j+FW)[0] + 
			OFrame(i+FW,j)[0] + OFrame(i,j-FW)[0]-4*OFrame(i,j)[0]);
			if(a<0)a+=256;
			Frame(i,j)[0]=a;
			Frame(i,j)[1]=OFrame(i,j)[1];//(OFrame(i-FW,j)[1] + OFrame(i,j+FW)[1] + 
			//OFrame(i+FW,j)[1] + OFrame(i,j-FW)[1]-4*OFrame(i,j)[1]);

			Frame(i,j)[2]=OFrame(i,j)[2];//(OFrame(i-FW,j)[2] + OFrame(i,j-FW)[2] + 
			//OFrame(i+FW,j)[2] + OFrame(i,j+FW)[2]-4*OFrame(i,j)[2]);
			
			if(Frame(i,j)[0] < 20 && OFrame(i,j)[0] < 20)// || Frame(i,j)[0] > 245) 
			{  //the value is red hue (20 > h | h > 240)
				if(OFrame(i,j)[1] < 190 && OFrame(i,j)[1] > 50 && OFrame(i,j)[2] > 65 && OFrame(i,j)[2] < 250) //vary the luminance values.  Black and White can be red with Very Low or Very High luminance
				//off-white can be red with low saturation and high luminance. something similar for black
				{
					
					SX+=j;
					SY+=i;
					ct++;
					//OFrame(i,j)[0]=0;
					//OFrame(i,j)[1]=255;
					//OFrame(i,j)[2]=255;
				}
				else
				{
					//OFrame(i,j)[0]=255;
					//OFrame(i,j)[1]=0;
					//OFrame(i,j)[2]=0;
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

	void Action()
	{
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
			
			cout<<Aim.x<<"|"<<Aim.y;//<<"||"<<CX_SCN<<"|"<<CY_SCN;
			cout<<endl;
			Ex=(Aim.x-CX_SCN);
			Ey=(Aim.y-CY_SCN);
			//cout<<Ex<<"|"<<Ey;
			//cout<<endl;
			Pan->accumulate(Ex/4);
			Tilt->accumulate(Ey/4);
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
/*int main(): Waits for LEDS to turn on and begins execution*/
#ifdef TRUE
int main()
{
	cout << "C";
	cout <<"D";
	Navigator X;
	cout << "A";
	cout << "B";
	Shooter Y;
	X.DCrap();
	//Y.Action();
	double S=TestIn();
	WriteF(LED0,"1");
	cout << "done" << endl;
	while(S*EZ_APPROXIMATION > TestIn())
	{
		//Y.Action();	//cout<<TestIn()<<endl;
	}
	

}
#endif	