//#include "Beagle.h"
#ifdef _WIN32
#include <Windows.h>
#endif
#include <memory.h> //not used much. just for o/p for trac test
#include <stdio.h> 
#include <math.h> //for sqrt
typedef unsigned char Tiny;
#include "Tuning.h"
#include "Target.h"
/*Helper Functions*/
//simple rgb, only output is our RGB_t sturct, not ARGB int for gdi.
//if you need any of these ouside of this cpp, you can place
//prototypes in Target.h
RGB_t RRGB(Tiny R,Tiny G,Tiny B)
{
	RGB_t ret;
		ret.RED=R;
		ret.BLUE=B;
		ret.GREEN=G;
	return ret;
}
RGB_t operator+(RGB_t A,RGB_t B)
{
	RGB_t r;
	if(A.RED+(int)B.RED<256)
	r.RED=A.RED+B.RED;
	else r.RED=255;
	if(A.GREEN+(int)B.GREEN<256)
	r.GREEN=A.GREEN+(int)B.GREEN;
	else r.GREEN=255;
	if(A.BLUE+B.BLUE<256)
		r.BLUE=A.BLUE+B.BLUE;
	else
		r.BLUE=255;
	return r;
}
RGB_t operator-(RGB_t A,RGB_t B)
{
		RGB_t r;
	if((int)A.RED-B.RED>0)
	r.RED=A.RED-B.RED;
	else r.RED=0;
	if((int)A.GREEN-B.GREEN>0)
	r.GREEN=A.GREEN-B.GREEN;
	else r.GREEN=0;
	if((int)A.BLUE-B.BLUE>0)
		r.BLUE=A.BLUE-B.BLUE;
	else
		r.BLUE=0;
	return r;
}
bool inline IsColRange(RGB_t D,RGB_t L,RGB_t H)
{
	if(L.RED<=D.RED && D.RED<=H.RED &&
		L.BLUE<=D.BLUE && D.BLUE<=H.BLUE &&
		L.GREEN<=D.GREEN && D.GREEN<=H.GREEN)
		return true;
	else
		return false;
}


//Trac test is NOT a safe or good function 
//it is a test function (for windows (thanks only to MessageBox))
//designed only to be used in Singular Specific Cases
//(or maybe its due to lack of design)
//the actual function to use
//is ProcRGBtoRMGM
//#ifdef _WIN32
void TracTest(RAW_BITMAP Bitmap)
{
	bool Active = false;
	static int k=-1;
	PrepPTMacro(Bitmap.width,Bitmap.height);
	//if 1 ( it will always doit) (if(0) wil never do it)
	if(1){
		//
	RGB_t OT[]=	 //these RGB values worked with incandesant lights with target held on my sholder.
	{
		RRGB(184,69,83),
		RRGB(165,29,57),
		RRGB(183,43,79),
		RRGB(176,33,63)
	};
	
	Point A = ProcRGBtoRMGM(Bitmap,OT,4); //<-this 4.
	//Point B = SD(Bitmap,A);
	//A=RefactorSD(Bitmap,B);
	int ij,jj;
	for(ij=-10;ij<21;ij++)
		for (jj=-10;jj<20;jj++)
		GetPoint(Bitmap.data,(A.x+ij),(A.y+jj))=RRGB(0,0,255);
	//A=B;
	char mb[1000];
	sprintf(mb,"%d|%d",A.x,A.y);
	//MessageBox(0,mb,0,0);
				
	k++;
	if(k==4)k=0;
}
	
		//there was more here.

}
//#endif
//ProcRGBtoRMGM maps colors near values in TG to RED (255,0,0) and all other values to BLACK (0,0,0)
//it also returns initial Center of Gravity (though you may need to process further to obtain necessary results)
//Bitmap is a RAW_BITMAP created during image capture
//TG is an array of rgb values to which target colors should be close to
//this can be hard coded, or read from a file as you wish.
// RRGB function is useful either way.
//TL is the LENGTH of the array TG
Point ProcRGBtoRMGM(RAW_BITMAP Bitmap, RGB_t * TG,int TL ) 
{
	PrepPTMacro(Bitmap.width,Bitmap.height);
	//TG=TG2;
	double xa=0,ya=0;
	int p=0;
	RGB_t DT;
	//DT is amount of deviation to allow.
	//smaller is stricter guarenteeing not to find colors outside, but possibly cutting colors from target
	//bigger is looser, increasing color found on target and increasing possibility of finding color outside.
	DT=RRGB(20,20,20);
	//simple red. (it was wrote befor the RRGB func)
	RGB_t RED;
	RED.BLUE=0;
	RED.RED=255;
	RED.GREEN=0;
	//simple black (~do~)
	RGB_t ALT;
	ALT.BLUE=0;
	ALT.RED=0;
	ALT.GREEN=0;
	int i,j;
	static int axc=-1;
	axc++;
	for(j=0;j<Bitmap.height;j++)
		for(i=0;i<Bitmap.width;i++)
		{
			//get the point and process it. (using GetPoint)
			RGB_t WBP = GetPoint(Bitmap.data,i,j);
			int k;
			for(k=0;k<TL;k++)
			{
				//note that overloaded operators for RGB guarentee no overflow
			if(IsColRange(WBP,TG[k]-DT,TG[k]+DT))
			{
				WBP=RED;
			}
			}
			//if wbp!= red, set black
			//though, is safer to use == than !=
			//the results of average are
			//completely different
			//idk why.
			if(WBP.RED==RED.RED && WBP.GREEN == RED.GREEN && WBP.BLUE ==RED.BLUE)
			{
				//used to find CG
				xa+=i;
				ya+=j;
				p++;
			}
			else
			WBP=ALT;
			//write point back (using GetPoint)
			GetPoint(Bitmap.data,i,j)=WBP;
			
			//do null
		}
		Point rt;
		rt.x=(int)(xa/p);
		rt.y=(int)(ya/p);
		return rt;
}
Point SD(RAW_BITMAP P, Point u)
{
	PrepPTMacro(P.width,P.height);
	int i,j;
	double vy=0,vx=0;
	int pc=0;
	for(j=0;j<P.height;j++)
		for(i=0;i<P.width;i++)
		{
			RGB_t PTX = GetPoint(P.data,i,j);
			if(PTX.BLUE==0 && PTX.GREEN==0 & PTX.RED==255)
			{
				vy+=(j-u.y)*(j-u.y);
				vx+=(i-u.x)*(i-u.x);
				pc++;
			}
			else
			{
				GetPoint(P.data,i,j)=RRGB(0,0,0);
			}
		}
	vy=sqrt(vy/pc);
	vx=sqrt(vx/pc);
	Point ret;
	ret.x=(int)vx;
	ret.y=(int)vy;
	return ret;
}
