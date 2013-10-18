/*
This code file contains basic functions for beagle bone black. The Contents are
I2C (get using OpenI2C_CH3 (byte addr), free using (CloseI2C)
this gives simplified access to the I2C channel I2C2 (pins 19 and 20 and BBB) 
with comm specified to the device with address addr.
addresses 1b and 54-57 are reserved.
RGBC (get using OpenI2C_3_FOR_TC34717, free using CloseI2CRGBC)
This gives simplified, specific access to the TC34717 RGBC color sensor
the datasheet is in the <Documentation> folder 
Specifically can read red,green,blue,and clear data separately.
literally RedRead,GreenRead,BlueRead,ClearRead (return 16bit short)
also write(regaddr,data) can be used to configure the sensor, if needed
(ex, for gain modification, see datasheet for register address, and data spec)
and read(regaddr) can be used to poll specific non-color data (or color data
but the other functions are better for that).
IP_OP_bin (get using OpenDigitalPin, free using delete statement.
This allows General Purpose I/O on pins on Beagle Bone Black
Similar to arduino's DigitalRead/DigitalWrite (but as object)
SetIp  Sets pin mode to digital input
SetOp  Sets pin mode to digital output
Read   Returns pin value iff(if and only if) pinmode is input
Write  Sets pin value iff pinmode is output
WebCam (get using GetWebCam, free using delete statement)
offers an object to get bitmap images from a web cam.
GetImage returns a RAW_BITMAP object.
RAW_BITMAP contains width(int),height(int), and data(RGB_t)
RGB_t has RED,GREEN,BLUE (each 1 byte (8bit))

*/

/* 
Thanks.
Thanks to Learn,Build,Share <http://learnbuildshare.wordpress.com/2013/05/29/beaglebone-black-digital-ouput/>
for basis on how to read/write pins 
(far beyond modified by now, but i wouldn't of known how without them)
and thanks to RabidGeek for linuxwebcam.h and example showing how to set up.
and thanks to multiple others for for ioctl and acess name for i2c operation.
*/




#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
//as it is compiled on beaglebone, this is only one needed.
#include <unistd.h>
#include <linux/i2c-dev.h>
//#include "linuxwebcam.h"
//using namespace std;
#include "Beagle.h"
#include "Tuning.h"
#ifndef I_Class
/* cI2C: Class for i2c comm */
#pragma region I2C
cI2C::cI2C(int ADDR)
{
	op	= open("dev/i2c-3", O_RDWR); //open i2c
	if(op>0)
		ioctl(op, I2C_SLAVE, ADDR); 
	//I2C * Ret = new cI2C(op);
	//return Ret;
}
cI2C::~cI2C()
{
	if(op>0)
		close(op);
}
void cI2C::Write(byte addr,byte value)
{
	if(op<=0)return;
	byte rt[]={addr,value};
	write(op,rt,2);
}
byte cI2C::Read(byte address)
{
	if(op<=0)return 0;
	write(op,&address,1);
	byte ret;
	read(op,&ret,1);
	return ret;

}
#pragma endregion Class for I2c comm
/* cRGBC: uses i2c for our rgbc sensor*/
#pragma region RGBC
cRGBC::cRGBC()
{

	fh= open("dev/i2c-3", O_RDWR); //open i2c
	ioctl(fh, I2C_SLAVE, TCS34717); 
	byte RPC=0x03;
	byte ad=0x00;//0x80;

}
cRGBC::~cRGBC()
{
	close(fh );
}
short cRGBC::RedRead()
{	
	byte cmd = 0x36;// 0x76;
	short rpt;
	write(fh,&cmd,1);
	read(fh,&rpt,2);
	return rpt;
}
short cRGBC::GreenRead()
{	
	byte cmd = 0x38;// 0x78;
	short rpt;
	write(fh,&cmd,1);
	read(fh,&rpt,2);
	return rpt;
}
short cRGBC::BlueRead()
{	
	byte cmd = 0x3A;// 0x7A;
	short rpt;
	write(fh,&cmd,1);
	read(fh,&rpt,2);
	return rpt;
}
short cRGBC::ClearRead()
{	
	byte cmd = 0x34;//0x74;
	short rpt;
	write(fh,&cmd,1);
	read(fh,&rpt,2);
	return rpt;
}
void cRGBC::Write(byte regaddr, byte data)
{
	byte cmd=0x20 | regaddr;
	byte b2[]={cmd,data};
	write(fh,b2,2);
}
byte cRGBC::Read(byte regaddr)
{
	byte cmd=0x20 | regaddr;
	byte rpt;
	write(fh,&cmd,1);
	read(fh,&rpt,1);
	return rpt;
}
#pragma endregion Class for ic2 for rgbc comm
/*c_bin_io Binary IO*/
//tested principle behind it.
#pragma region bin_io
c_bin_io::c_bin_io(int inPin)
{
	ip=ips=false;
	PinVal=new char[100];
	PinDirection=new char[100];

	TV=new char[10];
	FILE * TP=fopen("/sys/class/gpio/export", "a");
	//sprintf(TV,"%d",Pin);
	IPin=inPin;
	if(TP!=0)
	{
		fprintf(TP,"%d",inPin);
		fclose(TP);
		//printf("/n");
		sprintf(PinVal,"/sys/class/gpio/gpio%d/value",inPin);
		//printf("/sys/class/gpio/gpio%d/value",inPin);
		//printf("/n");
		sprintf(PinDirection,"/sys/class/gpio/gpio%d/direction",inPin);
		//printf("/sys/class/gpio/gpio%d/direction",inPin);
		//printf("/n");
	}
	OK=true;

}
c_bin_io::~c_bin_io()
{
	if(OK)
	{
		FILE * TP = fopen("/sys/class/gpio/unexport", "ab");
		if(TP!=0)
		{
			fprintf(TP,"%d",IPin);
			fclose(TP);
		}
		delete TV,PinVal,PinDirection;
	}
}
void c_bin_io::SetIP()
{
	ips=true;
	ip=true;
	if(OK)
	{
		PinMode=fopen(PinDirection,"rb+");
		//strcpy(TV,"in");
		if(PinMode==0)return;
		fprintf(PinMode,"in");
		fclose(PinMode);
	}
}
void c_bin_io::SetOP()
{
	ips=true;
	ip=true;
	if(OK)
	{
		PinMode=fopen(PinDirection,"rb+");
		if(PinMode==0)return;
		//strcpy(TV,"in");
		fprintf(PinMode,"out");
		fclose(PinMode);
	}
}
bool c_bin_io::Read()
{
	if(ips && ip) 
	{
		bool ret;
		Pin=fopen(PinVal,"rb+");
		if(Pin==0)return false;
		fread(TV,1,1,Pin);
		if(strcmp(TV,"1")==0)
			ret=true;
		else 
			ret=false;
		fclose(Pin);
		return ret;
	}
}
void c_bin_io::Write(bool val)
{
	if(ips && !ip) //not input = output
	{
		Pin=fopen(PinVal,"rb+");
		if(Pin==NULL)return;
		if(val)
			fprintf(Pin,"1");
		else
			fprintf(Pin,"0");
		fclose(Pin);
	}
}
#pragma endregion Class for standard digital I/O

void WriteF(const char * FileName, const char * value)
{
	FILE * A;
	A=fopen(FileName,"rb+");
	if(A==NULL) return;
	fprintf(A,value);
	fclose(A);
}
char * ReadF(const char * FileName, char * buff, int length)
{
	FILE * A;
	A=fopen(FileName,"rb+");
	if(A==NULL)return 0;
	fread(buff,1,1,A);
	fclose(A);
}
void EnableADCs()
{
//echo cape-bone-iio > /sys/devices/bone_capemgr.*/slots
FILE * A;
A=fopen(BEAGLE_CAPE_SLOTS,"ab");
if(A==NULL)return;
fprintf(A,"cape-bone-iio");
fclose(A);
}
double AIN(int ADC_ID)
{
char op[255];double ret;
sprintf(op,ADCT,ADC_ID);
FILE * A = fopen(op,"rb");
if(A==NULL)return 0;
fscanf(A,"%d",&ret);
return ret;
}
//untested
void EnablePWM(int HDR,int pin)
{
const char * am33xx_pwm = "am33xx_pwm";
FILE * A = fopen(BEAGLE_CAPE_SLOTS,"ab");
if(A==NULL) return;
fprintf(A,am33xx_pwm);
fclose(A);
//often the file needs be cycled first on this board
A = fopen(BEAGLE_CAPE_SLOTS,"ab");
fprintf(A,"bone_pwm_p%d_%d",HDR,pin);
fclose(A);
//bone_pwm_P8_13
}
/*NOTE: period is in nanoseconds, stranger is that duty is in nanoseconds (NOT %).
To convert is easy dutyNS=duty*periodNS/100; (for duty %, leave out 100 if duty is simple double)*/
void WritePWM(int HDR,int pin,int periodNS,int dutyNS)
{
	if(periodNS<dutyNS)return;
	char pd[255];
	sprintf(pd, "/sys/devices/ocp.*/pwm_test_P%d_%d.*/period",HDR,pin);
	FILE * A = fopen(pd,"ab");
	if(A==0)return;
	fprintf(A,"%d",periodNS);
	fclose(A);
	sprintf(pd, "/sys/devices/ocp.*/pwm_test_P%d_%d.*/duty",HDR,pin);
	fopen(pd,"ab");
	if(A==0)return;
	fprintf(A,"%d",dutyNS);
	fclose(A);
}
#else
#endif
