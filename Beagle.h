//#pragma once
//thanks to community at stack overflow
//pio_addr = mmap(0, GPIO1_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, GPIO1_START_ADDR);
//for beaglebone mean_and_dirty, we will use this style to increase speed (iff we need to)
//if we don't, i wont.  the delay of the filesystem may not hurt. but we keep this ref for later.

#ifndef __Beagle_Gen_purpose_input_output
#define __Beagle_Gen_purpose_input_output
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include "Tuning.h"
using namespace std;


//typedefs----------------------------------------------------------
typedef unsigned char byte;

//defines-----------------------------------------------------------
#define TCS34717 0x29 //address
#define UART_BASE "/dev/ttyO%d" //just read/write to this, with the
								//uart port as %d
								//just remember to enable them first!

								
								
class DTime
{
private:
time_t S;
suseconds_t M;
public:
DTime();
void Init(); //call to clear clock
double poll(); //get time since last call
//to Init() or poll() in seconds
};
class cI2C//: public I2C
{
private:
public:
	int op;
	cI2C(int ADDR);//construct
	~cI2C();//destruct
	void Write(byte addr,byte value);//write to device at addr (7bit 0 to 127)
	byte Read(byte address);//read from device at address (7bit 0 to 127)
};
//rgbc
class cRGBC//:public RGBC
{
public:
	int fh;
	//00000011
	//cmd
	//1000000 (80)
	cRGBC();
	~cRGBC();
	short RedRead();
	short GreenRead();
	short BlueRead();
	short ClearRead();
	void Write(byte regaddr, byte data);
	byte Read(byte regaddr);
};
//tested
class c_bin_io
{
private:
	bool OK,ip,ips;
	FILE *Pin, *PinMode; 
	int IPin;
	char * PinVal, * PinDirection, *TV;
public:
	c_bin_io(int inPin);
	~c_bin_io();
	void SetIP();
	void SetOP();
	bool Read();
	void Write(bool val);
};
class PWMAccumulator
{
private:
int value;
int period;
int LoT,HiT,CALPIN;
int HDR,pin;
public:
PWMAccumulator(int HDR, int pin, double TS, double Initial);
void accumulate(double DAV);
void set(double DAV);
double get();
void LowLimit (int a);
void HighLimit(int b);
};
//Adaptive (sliding) version of the PID
class APID
{
private:
DTime TMX;
double A,G;
double _P,_I,_D;
double 	_PMX,_PMN,_IMX,_IMN,
		_DMX,_DMN;
double X[3];
double Y[3];
public:
APID();
APID(double PMAX,double PMIN, double IMAX, double IMIN, double DMAX, double DMIN);
void Clear();//clears memory (re-init to 0)
 //executes the Transfer function generated
double Exec(double x);
};
class PID //actually just applies a second order Z transfer function
{
private:
double _P,_I,_D;
double A,G;
double Y[3];
double X[3];
DTime TMX;
public:
PID(); //applies a generic unity PI
PID(double P,double I,double D); //applies a PID based on parameters specified
void Clear();//clears memory (re-init to 0)
double Exec(double x); //executes the Transfer function generated
};
class DRV
{
private:
	ofstream opt;
public:
	DRV();
	~DRV();
	void Drive(int dir1, int dir2);
};
class ACON
{
private:
double X[FIR_LEN];
double F[FIR_LEN];
double AXP;
public:
ACON();
double Exec(double);

};
void WriteF(const char * FileName, const char * value);
char * ReadF(const char * FileName, char * buff, int length);
double AIN(int ADC_ID);
void EnableADCs();
//untested since last edit.
void EnablePWM(int HDR,int pin);
//works
void WritePWM(int HDR,int pin,int periodNS,int dutyNS);
//PWM EZ
void EZWritePWM(int HDR, int pin, double FS, double Duty);
//note: DUTY is a relative Fixed input (0<D<2^16-1)

//Differential input
double TestIn();
double Test2In();
double DiffIn();
double CenterPoint();

#endif
