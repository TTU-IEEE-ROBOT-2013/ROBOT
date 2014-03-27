#include "Beagle.h"
#include "Tuning.h"
#include <iostream>
using namespace std;
void echo(const char * F,const char * V)
{
FILE * A=fopen(F,"ab");
if(A==0)return;
fprintf(A,V);
fclose(A);
}
int main()
{
		 PWMAccumulator P(PAN_PWM);
	 PWMAccumulator T(TILT_PWM);
	c_bin_io  FlyWheels(FW_GPIO);
	c_bin_io  LinActuate(LA_GPIO);;
	FlyWheels.SetOP();
	FlyWheels.Write(false);
	LinActuate.SetOP();
	LinActuate.Write(LA_CLOSED);
	echo(LED1T,"none");
	echo(LED2T,"none");
	echo(LED3T,"none");
	echo(LED0T,"none");
	echo(LED1,"0");
	echo(LED2,"0");
	echo(LED3,"0");
	echo(LED0,"0");
	//while(true)sleep(10);	
       //system("ROBOT");

	
}