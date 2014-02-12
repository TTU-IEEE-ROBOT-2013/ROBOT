
//adative control system implementing a simple accumulator
//and a LMS adaptive filter
#ifndef CTL_ALGORITHM
#define CTL_ALGORITHM
#define CTL_MIN_DELTA_THRESHOLD .001
class CTLAdaptor
{
private:
	double * FIR;
	double * DELAY;
	double * delta;
	double YPast;
	double ODX;
	int LEN;
	
public:
	CTLAdaptor();
	~CTLAdaptor();
	///it is assumed x=0 when on target
	///as from a differential sensor, or 
	///after negative feedback applied
	///(x is error signal)
	///it is assumed that x has been mapped
	///appropriately, so that -1<=r<=1
	///where r is the approximate range of values
	///commmonly occupied in normal operation,
	///with x=0 when 100% perfect 
	///adjusting this scaleing may help in tuning performance
	///attributes such as speed/accurracy/stability
	///large factors increase speed(and accuracy if it remains stable)
	///but may cause instability.
	double Process(double x);
	//naturally you must provide feedback mechanism
};

#endif