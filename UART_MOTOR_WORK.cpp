#include <fstream>
#include <iostream>
#include <unistd.h>
#include "Tuning.h"
using namespace std;
#define UART_4 = "ADAFRUIT-UART4"
#define steps 5
typedef struct step
{
	int dir1;
	int dir2;
	int SLPT;
}stp;
stp steps_[] =
{
	{55,60,1000}
	,{60,58,1000}
	,{-50,55,1000}
	,{50,50,2000}
	,{0,0,0}
};
//Arg(Z)=arg(Z) mod 2*pi
void InitUart4();
int Drive(ofstream &opt,int dir1, int dir2);
int main()
{
	ofstream opt;
	opt.open("/dev/ttyO4",ios::binary | ios::in | ios::out);
	char opx[4];

	int mtr = 2;
	int pi,pi2;
	int dir;
	if(true)
	for(int ki=0;ki<steps;ki++)
	{
		step p = steps_[ki];
		Drive(opt, p.dir1, p.dir2);
		usleep(1000*p.SLPT);
	}

	if(false)
	while(1)
	{
		cout<<"enter diff. speed (mtr0 then mtr1)\n";
		cin >> pi;
		cin >> pi2;
		Drive(opt,pi,pi2);
	}
	if(false){
		opx[0]=0x80;
		opx[1]=0;
		cout<<"motor then direction then speed\n";
		cin >> mtr;
		cin >> dir;
		cin >> pi;

		dir=dir && 1;
		opx[2]=(char)mtr<<1 | dir;
		if(pi > 127)pi=127;
		opx[3]=pi;
		cout << pi << "||||||" << mtr << endl;
		opt.write(opx,4);
		opt.flush();
	}
}



void InitUart4()
{
	FILE * A = fopen(BEAGLE_CAPE_SLOTS,"ab");
	if(A==NULL) return;
	fprintf(A,UART_4);
	fclose(A);
}
int Drive(ofstream &opt,int dir1, int dir2)
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
		d2=0;
		spd2=-dir2;
	}
	else
	{
		d2=1;
		spd2=dir2;
	}
	char opx[4];
	opx[0]=0x80;
	opx[1]=0;
	//dir=dir && 1;
	d2=d2 && 1;
	d1=d1 && 1;
	//d1
	//opx[2]=(char)mtr<<1 | dir;
	opx[2]=((char)0<<1 | d1);
	if(spd1 > 127)spd1=127;
	opx[3]=spd1;
	//cout << pi << "||||||" << mtr << endl;
	opt.write(opx,4);
	opt.flush();

	//d2
	opx[2]=((char)1<<1 | d2);
	if(spd2 > 127)spd2=127;
	opx[3]=spd2;
	opt.write(opx,4);
	opt.flush();


}