//#pragma once
#ifndef __Beagle_Gen_purpose_input_output
#define __Beagle_Gen_purpose_input_output
#include <stdio.h>
#include <stdlib.h>
typedef unsigned char byte;
#define TCS34717 0x29 //address
//#ifndef I_Class //pure virtal interface fail me on linux. WHY?
//cI2C
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
void WriteF(const char * FileName, const char * value);
char * ReadF(const char * FileName, char * buff, int length);
double AIN(int ADC_ID);
void EnableADCs();
#endif
