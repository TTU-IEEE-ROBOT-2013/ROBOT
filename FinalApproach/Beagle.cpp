#ifndef COMMENTS
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

Thanks.
Thanks to Learn,Build,Share <http://learnbuildshare.wordpress.com/2013/05/29/beaglebone-black-digital-ouput/>
for basis on how to read/write pins 
(far beyond modified by now, but i wouldn't of known how without them)
and thanks to RabidGeek for linuxwebcam.h and example showing how to set up.
and thanks to multiple others for for ioctl and acess name for i2c operation.
*/
#endif
#define  TRUE  1
/*Includes*/
#ifdef TRUE
#include <stdio.h>
#include <cmath>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <fcntl.h>
//as it is compiled on beaglebone, this is only one needed.
#include <unistd.h>
#include <linux/i2c-dev.h>
#include "Beagle.h"
#include "Tuning.h"
#include <iostream>
using namespace std;
#endif
/* cI2C: Class for i2c comm */
#ifdef TRUE  
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
#endif
/* cRGBC: uses i2c for our rgbc sensor*/
#ifdef TRUE
cRGBC::cRGBC()
{

	fh= open("/dev/i2c-2", O_RDWR); //open i2c
	ioctl(fh, I2C_SLAVE, TCS34717);
	byte RPC=0x03;
	byte ad=0x00;//0x80;
	byte cmd[] = {0x80,0x03};
	//byte dat = 0x03;
	write(fh,cmd,2);
	int i;
	sleep(3);
	//#ifdef not_Defined
	//	for(i=0;i<0x1B;i++)
	//	{
	//		write(fh,i | 0x80,)
	//	}
	//#endif
	//write(fh,&dat,1);

}
cRGBC::~cRGBC()
{
	close(fh );
}
short cRGBC::RedRead()
{
	byte cmd = 0x36 | 0xA0;// 0x76;
	short rpt;
	write(fh,&cmd,1);
	read(fh,&rpt,2);
	return rpt;
}
short cRGBC::GreenRead()
{
	byte cmd = 0x38 | 0xA0;// 0x78;
	short rpt;
	write(fh,&cmd,1);
	read(fh,&rpt,2);
	return rpt;
}
short cRGBC::BlueRead()
{
	byte cmd = 0x3A | 0xA0;// 0x7A;
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

#endif
#ifndef TRUE
cRGBC::cRGBC()
{

	fh= open("dev/i2c-2", O_RDWR); //open i2c
	ioctl(fh, I2C_SLAVE, TCS34717); 
	

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
#endif
/*c_bin_io Binary IO*/
//tested principle behind it.
#ifdef TRUE

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
		sprintf(PinVal,"/sys/class/gpio/gpio%d/value",inPin);
		sprintf(PinDirection,"/sys/class/gpio/gpio%d/direction",inPin);
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
		delete	TV;
		delete	PinVal;
		delete	PinDirection;
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
	ip=false;
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
#endif
//Helper functions
#ifdef TRUE
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
/*
double abs(double x)
{
	if(x<0)return -x; else return x;
}
*/
#endif
/*ADC functions (just need 2) */
#ifdef _RAW_ADC_FS
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
#else
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
	char op[255];int ret=0;
	sprintf(op,ADCT,ADC_ID);
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
	return ret;
}

#endif
//untested {EnablePWM} (should work. very similar to adaFruit inits.
/*PWM back-end functions*/
#ifdef TRUE
void EnablePWM(int HDR,int pin)
{
	const char * am33xx_pwm = "am33xx_pwm";
	FILE * A = fopen(BEAGLE_CAPE_SLOTS,"ab");
	if(A==NULL) return;
	fprintf(A,am33xx_pwm);
	fclose(A);
	//often the file needs be cycled first on this board
	A = fopen(BEAGLE_CAPE_SLOTS,"ab");
	fprintf(A,"bone_pwm_P%d_%d",HDR,pin);
	fclose(A);
	//bone_pwm_P8_13
}
/*NOTE: period is in nanoseconds, stranger is that duty is in nanoseconds (NOT %).
To convert is easy dutyNS=duty*periodNS/100; (for duty %, leave out 100 if duty is simple double)*/
void WritePWM(int HDR,int pin,int periodNS,int dutyNS)
{
	if(periodNS<dutyNS)return;
	char pd[255];
	char tslk;
	sprintf(pd, "/sys/devices/ocp.2/pwm_test_P%d_%d.*/period",HDR,pin);
	FILE * A = fopen(pd,"ab");
	if(A==0)return;
	fprintf(A,"%d",periodNS);
	fclose(A);
	sprintf(pd, "/sys/devices/ocp.2/pwm_test_P%d_%d.*/duty",HDR,pin);
	fopen(pd,"ab");
	if(A==0)return;
	fprintf(A,"%d",dutyNS);
	fclose(A);
}

void EZWritePWM(int HDR, int pin, double FS, double Duty)
{
	if(Duty > 1)Duty=1;
	if(Duty < 0)Duty=0;
	int periodNS = (int)(1e9*1.0/FS);
	int dutyNS = (int)(periodNS * Duty);
	WritePWM(HDR,pin,periodNS,dutyNS);
}
#endif
/*PWMAccumulator: an accumulator that also outputs directly to PWM*/
#ifdef TRUE 
PWMAccumulator::PWMAccumulator(int HDRx, int pinx, double FS, double Initial)
{
	EnablePWM(HDRx,pinx);
	HDR=HDRx;pin=pinx;
	period = (int)(FS);
	LoT=0;
	HiT=period;
	value=(int)(Initial*period);
	//ritePWM(HDR,pin,period,value);
	/*
	const char * am33xx_pwm = "am33xx_pwm";
	
	FILE * A = fopen(BEAGLE_CAPE_SLOTS,"ab");
	if(A==NULL) {cout<<"ERROR: FILE PROBLEM! CAPEMGR\n"<<BEAGLE_CAPE_SLOTS<<endl; return;}
	fprintf(A,am33xx_pwm);
	fclose(A);
	//often the file needs be cycled first on this board
	A = fopen(BEAGLE_CAPE_SLOTS,"ab");
	fprintf(A,"bone_pwm_p%d_%d",HDR,pin);
	fclose(A);
	*/
	char pd[255];
	int tslk;
	//A=0;
	//we have problems with errors opening at first. It needs time to initialize
	usleep(100000);
	FILE * A=0;
	for(int i=0;i<1000 && A==0;i++)
	{
		sprintf(pd, "/sys/devices/ocp.2/pwm_test_P%d_%d.%d/period",HDR,pin,i);
		A = fopen(pd,"ab");
		tslk=i;
	}
	if(A!=0)
	{
		CALPIN=tslk;
		fclose(A);
	}
	else
	{
		cout<<"FATAL ERROR: PWM COULD NOT BE INITIALIZED!"<<HDR<<"|"<<pin<<"|"<<tslk;
	}
	
}

void PWMAccumulator::accumulate(double DAV)
{
	DAV *= DELTA_ACC; //scale feed-in
	value+=DAV*(.01*period);
	if(value > HiT)value=HiT;
	if(value < LoT)value=LoT;	
	//if(periodNS<dutyNS)return; vistigial code
	char pd[255];
	char tslk;
	sprintf(pd, "/sys/devices/ocp.2/pwm_test_P%d_%d.%d/period",HDR,pin,CALPIN);
	FILE * A = fopen(pd,"ab");
	if(A==0)return;
	fprintf(A,"%d",period);
	fclose(A);
	sprintf(pd, "/sys/devices/ocp.2/pwm_test_P%d_%d.%d/duty",HDR,pin,CALPIN);
	fopen(pd,"ab");
	if(A==0)return;
	fprintf(A,"%d",value);
	fclose(A);
}

void PWMAccumulator::set(int DAV)
{
	value=DAV;//*period;
	if(value > HiT)value=HiT;
	if(value < LoT)value=LoT;
	//WritePWM(HDR,pin,period,value);
	char pd[255];
	char tslk;
	sprintf(pd, "/sys/devices/ocp.2/pwm_test_P%d_%d.%d/period",HDR,pin,CALPIN);
	FILE * A = fopen(pd,"ab");
	if(A==0)return;
	fprintf(A,"%d",period);
	fclose(A);
	sprintf(pd, "/sys/devices/ocp.2/pwm_test_P%d_%d.%d/duty",HDR,pin,CALPIN);
	fopen(pd,"ab");
	if(A==0)return;
	fprintf(A,"%d",value);
	fclose(A); 
}
double PWMAccumulator::get()
{
	double ret = value / period;
	return ret;
}
void PWMAccumulator::LowLimit(int a)
{
	LoT=a;
}
void PWMAccumulator::HighLimit(int a)
{
	HiT=a;
}
#endif
/*Helper Func for Drive Control*/
#ifdef TRUE
void InitUart4()
{
	FILE * A;// = fopen(pd,"ab");
	A = fopen(BEAGLE_CAPE_SLOTS,"ab");
	if(A != 0) 
	{
		fprintf(A , UART_4);
		fclose(A);
	}
}
#endif
/*Drive Controler*/
#ifdef TRUE
DRV::DRV()
{
	InitUart4();
	opt=new ofstream();
	opt->open("/dev/ttyO4",ios::binary | ios::in | ios::out);

}
void DRV::Drive(int dir1, int dir2)
{
	int spd1;
	int spd2;
	int d1;
	int d2;
	if(dir1 < 0)
	{
		d1 = 1;
		spd1=-dir1;
	}
	else
	{
		d1=0;
		spd1=dir1;
	}
	if(dir2 < 0)
	{
		d2=1;
		spd2=-dir2;
	}
	else
	{
		d2=0;
		spd2=dir2;
	}
	char opx[4];
	opx[0]=0x80;
	opx[1]=0;
	//dir=dir && 1;
	//d2=d2 && 1;
	//d1=d1 && 1;
	//d1
	//opx[2]=(char)mtr<<1 | dir;
	opx[2]=((char)0<<1 | d1);
	if(spd1 > 127)spd1=127;
	opx[3]=spd1;
	//cout << pi << "||||||" << mtr << endl;
	//cout << "Motor 1 Start" << endl;
	opt->write(opx,4);
	usleep(10000);
	//cout << "Motor 1 Write" << endl;
	opt->flush();
	//cout << "Motor 1 Flushed" << endl;
	usleep(10000);
	//d2
	opx[2]=((char)1<<1 | d2);
	if(spd2 > 127)spd2=127;
	opx[3]=spd2;
	//cout << "Motor 2 Start" << endl;
	opt->write(opx,4);
	usleep(10000);
	//cout << "Motor 2 Write" << endl;
	opt->flush();
	usleep(10000);
	//cout << "Motor 2 Flushed" << endl;


}
#endif
/*Line Sensor All-In-One function*/
#ifdef TRUE
#define EZ_A EZ_APPROXIMATION
#define EZ_T TURN_THRESH
double Test2In()
{
	double A = AIN(TP2);
	return A;
}
double TestIn()
{
	double A = AIN(TP1);
	return A;
}
double bDiffIn()
{
	double T = AIN(TP1);
	double T2= AIN(TP2); //TP2 should be more than A and B
	double A,B;
	A=AIN(DP);
	B=AIN(DM);
	double C = A-B;
	return C;
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
/*Used for time based functions*/
#ifdef TRUE
DTime::DTime()
{
	this->Init();
}
void DTime::Init()
{
	timeval X;
	gettimeofday(&X,0);
	S=X.tv_sec;
	M=X.tv_usec;
}
double DTime::poll()
{
	timeval X;
	gettimeofday(&X,0);
	double B = (X.tv_sec-S)+(X.tv_usec-M)/1e6;
	S=X.tv_sec;
	M=X.tv_usec;
	return B;
}
#endif
/*PID controllers*/
#ifdef TRUE
//local delay function for PID and APID
void QDelay(double * X)
{
	X[2]=X[1];
	X[1]=X[0];
}
//y[n]=AX[n]+GX[n-1]+AX[n-2]+Y[n-2]

PID::PID()
{
	_P=1;
	_I=1;
	_D=0;
	TMX.Init();
	Y[0]=Y[1]=Y[2]=X[0]=X[1]=X[2]=0;
}

APID::APID()
{
	_P=1;
	_I=1;
	_D=0;
	_DMN=0;
	_DMX=.01;
	_PMX=10;
	_PMN=.1;
	_IMX=2;
	_IMN=.1;
	TMX.Init();
	Y[0]=Y[1]=Y[2]=X[0]=X[1]=X[2]=0;
}
APID::APID(double PMAX,double PMIN, double IMAX, double IMIN,double DMAX, double DMIN)
{
	_P=1;
	_I=1;
	_D=0;
	_DMN=DMIN;
	_DMX=DMAX;
	_PMX=PMAX;
	_PMN=PMIN;
	_IMX=IMAX;
	_IMN=IMIN;
	TMX.Init();
	Y[0]=Y[1]=Y[2]=X[0]=X[1]=X[2]=0;
}

PID::PID(double P,double I,double D)
{
	_P=P;
	_I=I;
	_D=D;
	TMX.Init();
	Y[0]=Y[1]=Y[2]=X[0]=X[1]=X[2]=0;
}
void PID::Clear()
{
	TMX.Init();
	Y[0]=Y[1]=Y[2]=X[0]=X[1]=X[2]=0;
}
double PID::Exec(double x)
{
	double T=TMX.poll();
	A=_P+_I*T/2+2*_D/T;
	G=2*(_I*T/2+2*_D/T);
	QDelay(X);
	QDelay(Y);
	X[0]=x;//-fy;//differetial input takes care of negative feedback.
	//PID
	Y[0]=A*X[0]+G*X[1]+A*X[2]+Y[2];
	//done
	return Y[0];
}

void APID::Clear()
{
	TMX.Init();
	Y[0]=Y[1]=Y[2]=X[0]=X[1]=X[2]=0;
}
double APID::Exec(double x)
{
	//the difference: we re-calculate the PID terms.
	double e=x;
	double px;
	double P=0,I=0,D=0;
	if(abs(x)<APID_THROW && abs(x)>APID_FALL)
	{
		P=(x-APID_FALL)/(APID_THROW-APID_FALL)*(_PMX-_PMN)+_PMN;
		//dido for i and d
		D=(x-APID_FALL)/(APID_THROW-APID_FALL)*(_DMX-_DMN)+_DMN;
		I=(x-APID_FALL)/(APID_THROW-APID_FALL)*(_IMX-_IMN)+_IMN-APID_INTEGRAL_CURE*I;
	}
	else
	I=_IMN-APID_INTEGRAL_CURE*I;
	if(I<_IMN)I=_IMN;
	double T=TMX.poll();
	A=P+I*T/2+2*D/T;
	G=2*(I*T/2+2*D/T);
	QDelay(X);
	QDelay(Y);
	X[0]=x;//-fy;//negative feedback (differential input fixes)
	//PID
	Y[0]=A*X[0]+G*X[1]+A*X[2]+Y[2];
	//done
	return Y[0];
}
#endif
/*True adaptive control system in a box*/
/*Here is hopeing autocorrelation y with y is as good as cross corelation
y with x (since x is y in this case and thus the autocorrelation*/
#ifdef TRUE
ACON::ACON()
{
	for(int j=0;j<FIR_LEN-1;j++)
	{
		X[j]=0;
		F[j]=0;
	}
	F[0]=1;
	AXP=0;
}
double ACON::Exec(double x)
{
	//if this exploads, a -1 implementation will be needed
	//x=-AXP;//Like this!!
	int j;
	double p=0;
	double delta=.01*(1-exp(-abs(x)*.1));
	for(j=FIR_LEN-1;j>0;j--)
	{
		//Apply delay
		X[j]=X[j-1];
		//filter with them
		p+=X[j]*F[j];
	}
	X[0]=x;
	p+=X[0]*F[0];
	for(j=0;j<FIR_LEN-1;j++)
	F[j]+=delta*X[0]*X[j];
	//using p instead of y since y is output of sensor
	//which is also x in this case (due to feedback)
	AXP+=p;
	return AXP;
}
#endif
/*The important interface between Control module and motor speed*/
#ifdef TRUE
int __MT_CONV_L(double A, double R)
{
	int ret;
	double RT;
	RT = A * SPD_FACT * R + TARG_SPEED;
	return (R * RT);
}
int __MT_CONV_R(double A, double R)
{
	double RT;
	int ret;
	RT=-1 * A * SPD_FACT * R + TARG_SPEED;
	return (R * RT);
}
#endif
