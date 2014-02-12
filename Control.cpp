#include "Control.h"

double CTLAdaptor::process(double x)
{
	double y = 0;
	
	//accumulator
	//the accumulator is the discreate time integrator
	double YAC = YPast + x;
	
	//the closer to perfect we are, the smoother the filter
	//the farther we are, the more agressive the filter
	delta = delta*20*x+.0001
	
	if(x<.02)delta=0; //do not adapt the filter if were within 2%
	for(j=FIRLen-1;j>0;j--)
		{
			
			//accumulator
			//Apply filter coefficients 
			FIR[j]+=delta*ODX*DELAY[j];
			//filter with them
			y+=DELAY[j]*FIRCoefs[j];
			//Apply delay
			DELAY[j]=DELAY[ j-1];
		}
		

}