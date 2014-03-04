#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <unistd.h>
using namespace std;
//TODO: get it going using filesystem as shown here

#define ADC_DEV  "/sys/devices/ocp.2/44e0d000.tscadc/tiadc/iio:device0/in_voltage%d_raw"
#define ADC_TRIG "/sys/devices/ocp.2/44e0d000.tscadc/tiadc/trigger/current_trigger"
#define BEAGLE_CAPE_SLOTS "/sys/devices/bone_capemgr.8/slots" 

void EnableADCs()
{
	//echo cape-bone-iio > /sys/devices/bone_capemgr.*/slots
	FILE * A;
	A=fopen(BEAGLE_CAPE_SLOTS,"ab");
	if(A==NULL)return;
	fprintf(A,"cape-bone-iio","ab");
	fclose(A);
}

double AIN(int ADC_ID)
{
	{
	FILE * A;
	A=fopen(ADC_TRIG);
	fprintf(A,"1");
	fclose(A);
	}
	char op[255];int ret=0;
	sprintf(op,ADC_DEV,ADC_ID);
	//cout << op << endl;
	//ifstream A;
	////A.open(op, ios::in);
	//if(!A.eof())
	{
		//A>>ret;
		FILE * A = fopen(op,"r");
		if(A==NULL)
		{//cout << "KER" << endl;
			return 0;}
try2:
		int err = fscanf(A,"%d",&ret);
		if(err == EOF) goto try2;
		fclose(A);
		//cout << ret << endl;
		//A.close();
	}
	ret * (1.8/4096*1000);
	return ret;
}
int main()
{
for(int i=0;i<1000;i++)
{
	cout << AIN(0);
	sleep(100);
	}
}