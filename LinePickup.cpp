#include <iostream>
#include "tuning.h"
using namespace std;
//this ONE def is already in beagle.cpp and doesn't go into Tuning.h
#define TRUE 1
//these Defs go into Tuning.h
//change these (the first 4 need updated based on pins used)
#define DP 1
#define DM 2
#define TP1 3
#define TP2 4
#define OUT_OF_BOUNDS -1e100
#define TURN_TIME 1e100
#define TURN_THRESH 1.5 //We know to turn when
#define EZ_APPROXIMATION 1.1
//the following stay in the cpp file (not Tuning.h or Beagle.h)

void EnableADCs()
{
	//echo cape-bone-iio > /sys/devices/bone_capemgr.*/slots
	FILE * A;
	A=fopen(BEAGLE_CAPE_SLOTS,"ab");
	if(A==NULL)return;
	fprintf(A,"cape-bone-iio");
	fclose(A);
}
#ifdef TRUE
#define EZ_A EZ_APPROXIMATION
#define EZ_T TURN_THRESH
double AIN(int ADC_ID)
{
	char op[255];double ret;
	sprintf(op,ADCT,ADC_ID);
	FILE * A = fopen(op,"rb");
	if(A==NULL)return 0;
	fscanf(A,"%d",&ret);
	return ret;
}
double DiffIn()
{
	double T = AIN(TP1);
	double A,B;
	A=AIN(DP);
	B=AIN(DM);
	double C = A-B;
	if((A>T*EZ_A || B>T*EZ_A)&&(T<A*EZ_A&&T<B*EZ_A))
	{
		return C;
	}
	if((T>A || T>B) && !(T>A*EZ_T && T>B*EZ_T))
	{
		return TURN_TIME;//value that will never be returned by main calculation
	}
	if((T>A/EZ_A && T>B/EZ_A))
	{
		return OUT_OF_BOUNDS;
	}
	
}
double CenterPoint()
{
	double T=AIN(TP1);
	double E=AIN(TP2);
	double C=E-T;
	if(T>E/EZ_A && T<E*EZ_A)
	{
		return -1; //Center on the intersection (we drive past this sligthly) before we turn
	}
	else if(T>E*EZ_A)
	{
		return 0; //Center not over a line
	}
	else if(E>T*EZ_A)
	{
		return C; //Center over a line (turn complete)
	}
}
#undef EZ_A
#undef EZ_T
#endif
int main()
{
	EnableADCs();
	while(true)
	{
		double A = DiffIn();
		if (A==TURN_TIME)
		{
			cout << "TURN LEFT";
		}else if(A==OUT_OF_BOUNDS || A==(-OUT_OF_BOUNDS/2))
		{
			cout << "Out of bounds!";
		}
		else
		{
			cout << A;
		}
		char wt[1000];
		cin >> wt;
	}
}