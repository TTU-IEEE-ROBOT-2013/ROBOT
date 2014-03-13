#include <iostream>
#include "Beagle.cpp"
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
	//c_bin_io TLED(LED_GPIO);
	int REV;
	int MSR,MSL;
public:
	Navigator()
	{
		//TLED.SetIP();
		LED_PWR=new c_bin_io(LED_GPIO);//
		EnableADCs();
		//InitUart4();
		#ifndef USE_PID
		CTL=new ACON();; //discreate self-adapting control system(No Time Funcs Req'd)
		#else
		CTL=new PID(1,1,2); //someone can set the weights appropriately
		#endif
		LED_PWR->SetOP();
		LED_PWR->Write(true);

		Drive = new DRV();
		BFinder = new cRGBC();
		REV=1;
		MSR=0;
		MSL=0;
		//CCLR;
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
		sleep(2);
		//int * X=(int*)10000;
		//cout << *X;
		//return;
		//MSR=MSL=100;
		//Drive->Drive(100,100);
		//sleep(10);
		//Decelerate();
		PID pt(2.5,.05,.05);
		AIN(4);AIN(4);AIN(4);
		double dV =0;

		while(AIN(4) < 1640 )
		{
			//double ID = AIN(4);
			//cout << ID << endl;
			double dP=DiffIn()+20;
			//dV+=dP/200;
			dV=pt.Exec(dP);
			//double dV = -CTL->Exec(dP); //apply control system
			cout << dP << "|" << dV << endl;
			
			MSR=50+.001*dV;
			MSL=50-.001*dV;
			if(MSR > 55)MSR=55;
			if(MSL > 55)MSL=55;
			if(MSR < -55)MSR=-55;
			if(MSL < -55)MSL=-55;
			Drive->Drive(MSR,MSL);
		}
		Drive->Drive(70,-70);
		usleep(1400000);
		//usleep(50000);
		//while(AIN(6) < 1710);
		Drive->Drive(50,50);
		usleep(500000);
		//brute-force-fire
		sleep(2);
		Drive->Drive(0,0);
		return;
		/*
		AIN(4);AIN(4);AIN(4);
		while(AIN(4) < 1640 )
		{
			//double ID = AIN(4);
			//cout << ID << endl;
			double dP=DiffIn()+20;
			//dV+=dP/200;
			dV=pt.Exec(dP);
			//double dV = -CTL->Exec(dP); //apply control system
			cout << dP << "|" << dV << endl;
			
			MSR=50+.0001*dV;
			MSL=50-.0001*dV;
			if(MSR > 55)MSR=55;
			if(MSL > 55)MSL=55;
			if(MSR < -55)MSR=-55;
			if(MSL < -55)MSL=-55;
			Drive->Drive(MSR,MSL);
		}
		Drive->Drive(0,0);
		while(true)
		{
		//AIN4
		double ID = AIN(6);
		cout << ID << endl;
		}
	*/
	}
};
int main()
{
Navigator NAV;
NAV.DCrap();
}