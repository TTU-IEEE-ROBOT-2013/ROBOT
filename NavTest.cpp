#include "Beagle.h"
#include <iostream>
using namespace std;
#define LINE_THRESHOLD 0xFF
#define MAX_VAL 0xFFFF
#define MIN_VAL (0 - MAX_VAL)
#define LTGT(x) (x>LINE_THRESHOLD)
#define LTLT(x) (x<LINE_THRESHOLD)
#define DPrint(x) cout<<x //change to ;cout<<x when not debug
//will need to determin good threshold
int main()
{
EnableADCs();
cout<<LineTrace();
return 0;
}

double LineTrace()
{
double a,b,c,d;
a=AIN(1);
b=AIN(0);
c=AIN(3);
d=AIN(2);
DPrint("Threshold_R"<<b);
DPrint("Threshold_L"<<c);
double OT;
OT=a-d;
if(b<LINE_THRESHOLD && c<LINE_THRESHOLD)
	return OT
else if(LTGT(b) && LTLT(c) && LTLT(a) && LTLT(d))
	return MAX_VAL;
else if(LTLT(b) && LTLT(c) && LTLT(a) && LTLT(d))
	return MIN_VAL;
}
