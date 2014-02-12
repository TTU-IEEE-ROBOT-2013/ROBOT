#include "Beagle.h"
#include <iostream>
using namespace std;
int main()
{
	//pwm header P8, pin 13.
	//period of 20ms, duty of 1.5ms
	//definitions are in ns
	PWMAccumulator PAN(8,13,20000000,1500000);
	PAN.LowLimit(900000);
	PAN.HighLimit(2100000);
	PWMAccumulator TILT(9,15,20000000,1500000);
	TILT.LowLimit(900000);
	TILT.HighLimit(2100000);
	//PAN.accumulate(1)
	//TILT.accumulate(1)
	
	//both increment by 1%
	//PAN.accumulate(-1)
	//decrement by 1%
	char in[1024];
	char *a;
	double dlta = 1;
	while (true)
	{
		cout<<	"Use a string to control the pan/tilt\n" <<
				"each character issues a command.\n" <<
				"w increase tilt, s decrease tilt.\n"<<
				"d increase pan, a decrease pan.\n"  <<
				"e increase step size, q decrease step size";
		cin.getline(in,1000,'\n');
		for(a=in;*a!=0;a++)
		{
			if(*a=='w' || *a=='W')
			TILT.accumulate(dlta);
			if(*a=='s' || *a=='S')
			TILT.accumulate(-dlta);
			if(*a=='d' || *a=='D')
			PAN.accumulate(dlta);
			if(*a=='a' || *a=='A')
			PAN.accumulate(-dlta);
			if(*a=='e' || *a=='E')
			dlta*=10;
			if(*a=='q' || *a=='Q')
			dlta/=10;
			
		}
	}


}