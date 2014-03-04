#include <iostream>
#include <fstream>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <errno.h>
#define TRUE 1

//ADC WORK HERE
#ifdef TRUE
//ADC DRIVER using memmap
//    munmap(ddrMem, 0x0FFFFFFF);
//ddrMem = mmap(0, 0x0FFFFFFF, PROT_WRITE | PROT_READ, MAP_SHARED, mem_fd, DDR_BASEADDR);
//ADC_TSC DMA (NO USE?)

//ADC_TSC  	0x44E0_D000 >> 0x44E0_EFFF
//			0x44E0_F000	>> 0x44E0_FFFF
//0x54C0_0000 to 0x54C0_FFFF

using namespace std;
#define INT32 int
#define ADC_START  0x44E0D000
#define ADC_SEN    0x54
#define ADC_SCFG   0x64
#define ADC_DSTP   8
#define ADC_FIFO0  0x100
#define ADC_FIFO1  0x200
#define ADC_CTL    0x40
#define ADC_STAT   0x44
#define ADC_FLEN1  0xF0
#define ADC_FLEN0  0xE4
//44h bit 5 1=busy (working)
//write 0
#define GPIO_DEBOUNCE 0x150
#define GPIO_DEBOUNCD_TIME 0x154
//read
#define GPIO_DATAIN 0x138
//sizeof(mem)
#define GPIO_MEM_LEN 0x2000
static const int __GPIO_BLOCK_LIST[]=
{
0x44E07000, //GPIO0
0x4804C000, //GPIO1
0x481AC000,
0x481AE000,
0
};
class GPIO_READ_DMA
{
private:
int pin;
int * DTI;
void * _MEM;
int    _MEM_F;
public:
bool ER;
GPIO_READ_DMA(int BLOCK, int PIN)
{
	///ip=ips=false;
	///PinVal=new char[100];
	//PinDirection=new char[100];
	char PinDirection[1024];
	//TV=new char[10];
	FILE * TP=fopen("/sys/class/gpio/export", "a");
	//sprintf(TV,"%d",Pin);
	int IPin=32*BLOCK + PIN;
	if(TP!=0)
	{
		fprintf(TP,"%d",IPin);
		fclose(TP);
		//sprintf(PinVal,"/sys/class/gpio/gpio%d/value",pin);
		sprintf(PinDirection,"/sys/class/gpio/gpio%d/direction",IPin);
			//ips=true;
			//ip=true;
		FILE * PinMode=fopen(PinDirection,"rb+");
		//if(PinMode==0)return;
		fprintf(PinMode,"in");
		fclose(PinMode);
	}
	fclose(TP);
		ER=false;
		if(BLOCK > 3 || BLOCK < 0)
		{ER=true;return;}
		pin=PIN;
		_MEM_F=0;
		_MEM_F = open("/dev/mem", O_RDWR);
		if(_MEM_F==0){ER=true;return;}
		_MEM=mmap(0,0xFFFF,PROT_WRITE | PROT_READ, MAP_SHARED, _MEM_F,__GPIO_BLOCK_LIST[BLOCK]);
		if(_MEM==0){ER=true; close(_MEM_F);}
		DTI=(int*)((char*)_MEM+GPIO_DATAIN);
		int* tp=(int*)((char*)_MEM+GPIO_DEBOUNCE);
		(*tp)=(*tp)&(!(1<<PIN)); 
		//guarentee we have no debouncing (or guarentee we have debouncing if needed)
		//this processor supports debouncing up to 255 31us intervals (~7ms)
		
	msync(_MEM,GPIO_MEM_LEN,MS_SYNC);
}
bool Read()
{
	msync(_MEM,GPIO_MEM_LEN,MS_SYNC);
	bool A = ((*DTI & 1<<pin)!=0);
	return A;
}
};
class ADC_DMA
{
private:
	void * ADCX;
	int * stx;
	int ln;
	int MEM;
	int stpx;
public:
	bool ER;

	/*
	First, helper funtions
	*/
	//_SET_STEPS enables steps using 16 bit input. msb is step 16, lsb is step 1
	void _SET_STEPS(int STS)
	{
		STS &= 0xFFFF;
		STS << 1;
		INT32 * V=((INT32*)((char*)ADCX+ADC_SEN));
		(*V)=STS;
	}
	//config steps configures step n (16 max) to input voltage from PIN and 
	//store result in FIFO
	void _CONFIG_STEP_N(int n, int FIFO, int PIN)
	{
		if(FIFO != 1)FIFO=0;
		if(PIN < 0 || PIN > 8)PIN=0;
		INT32 CFG=0;
		CFG |= (FIFO << 26);
		CFG |= (PIN  << 19);
		CFG |= (0x1  << 00);
		INT32 * C=((INT32*)((char*)ADCX+ADC_SCFG+n*ADC_DSTP));
		(*C)=CFG;
		(*(C+1))=0xFF0000FF;
	}
	//check status
	bool _STAT()
	{
		INT32 * C=((INT32*)((char*)ADCX+ADC_STAT));
		return ((*C) & (1<<5));
	}
	//Reads the first value from FIFO, then removing it
	int _POP_FIFO0()
	{
		INT32 * C=((INT32*)((char*)ADCX+ADC_FIFO0));
		return (*C) & 0xFFF;
	}
	int _POP_FIFO1()
	{
		INT32 * C=((INT32*)((char*)ADCX+ADC_FIFO1));
		return (*C) & 0xFFF;
	}
	int _LEN_FIFO1()
	{
		INT32 * C=((INT32*)((char*)ADCX+ADC_FLEN1));
		return (*C) & 0x7F;
	}
	int _LEN_FIFO0()
	{
		INT32 * C=((INT32*)((char*)ADCX+ADC_FLEN0));
		return (*C) & 0x7F;
	}
	//start getting data.
	void _ENABLE()
	{
		INT32 * C=((INT32*)((char*)ADCX+ADC_CTL));
		(*C)=1;
	} 
	//stop getting data
	void _DISABLE()
	{
		INT32 * C=((INT32*)((char*)ADCX+ADC_CTL));
		(*C)=0;
	}
	void _UPD()
	{
		msync(ADCX,0xFFFF,MS_SYNC);
	}
	void Clear()
	{
		_UPD();
		while(_LEN_FIFO0() > 0)
		{	
			_POP_FIFO0();
			_UPD();
		}
		while(_LEN_FIFO1() > 0)
		{	
			_POP_FIFO1();
			_UPD();
		}
	}
	//constructor (DMA init)
	ADC_DMA()
	{
		ER=false;
		MEM=0;
		MEM = open("/dev/mem", O_RDWR);
		if(MEM==0)ER=true;
		//cout << "OPENED MEM"<<endl;
		ADCX=mmap(0,0xFFFF,PROT_WRITE | PROT_READ, MAP_SHARED, MEM,ADC_START);
		//cout << "MAPPED MEM"<<ADCX<<endl;
		if(ADCX==0)
		{
			close(MEM);
			MEM=0;
			ER=true;
		}

		if(ER)return;
		stpx=0xFF;
		_SET_STEPS(stpx);
		//cout << "STPS";
		stx=new int[8];
		for(int i=0;i<8;i++)stx[i]=0;
		
		{
			//configure to read all 8 adc's, interlacing values into the FIFO's
			//_CONFIG_STEP_N(int n, int FIFO, int PIN)
			_CONFIG_STEP_N(0,0,0);
			_CONFIG_STEP_N(1,0,1);
			_CONFIG_STEP_N(2,0,2);
			_CONFIG_STEP_N(3,0,3);
			_CONFIG_STEP_N(4,0,4);
			_CONFIG_STEP_N(5,0,5);
			_CONFIG_STEP_N(6,0,6);
			_CONFIG_STEP_N(7,0,7);
		}
		msync(ADCX,0xFFFF,MS_SYNC);
	}
	
	~ADC_DMA()
	{
		if(MEM != 0 ) 
		{
			munmap(ADCX, 0x2FFF);
			close(MEM);
			delete stx;
		}
	}
	
	
	void Poll()
	{
		Clear();
		_SET_STEPS(stpx);
		_ENABLE();
		msync(ADCX,0xFFFF,MS_SYNC);
		while(!_STAT())msync(ADCX,0xFFFF,MS_SYNC); //wait for calculations done
		//this may not be doing its job.
		usleep(1000);
		for(int i=0;i<8;i++)
		{
			msync(ADCX,0xFFFF,MS_SYNC);			
			stx[i]=_POP_FIFO0();
			//stx[i+1]=_POP_FIFO1();
			
		}
		_DISABLE();
	}
	double AIN(int N)
	{
		return stx[N]* (1.8/4096*1000);;
	}
	double DiffIn()
	{
		return (stx[0]-stx[1])* (1.8/4096*1000);;
	}
	/*
	NOW: Create the rest of the class to give easier access
	to these underlying properties.
	functions
		SetInput(int * channels, int ct);//set which channels to get
		Poll(); //get the data from adcs
		AIN(int c); //get data point recieved from channel c
	
		//maybe.
		GetData (int * channels, int ct); //return all data recieved.
	should be all that is needed.
	
	*/
	
	
};

// int main()
// {
	// cout << "init" << endl;
	// ADC_DMA AX;
	// AX.Clear();
	// cout <<"ERR?"<<AX.ER<<endl;
	// while(true)
	// {
		// AX.Clear();
		// AX.Poll();
		// cout << AX.AIN(2)<<"   "<<AX.AIN(3)<<"    "<<AX.AIN(0)<<"    "<<AX.AIN(1)<<endl;
		// sleep(1);
	// }
// }
#endif
//include our headers last.
//which is always a good idea.
#include "Tuning.h"
#include "Beagle.h"
using namespace std;
using namespace cv;

typedef struct _CPS
{
int x;
int y;
} CPoint;