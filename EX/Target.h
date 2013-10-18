#pragma once
//no longer used. if you remove, refer to end of this file.
#include "Tuning.h"
#ifndef ___Target_finder_h
#define ___Target_finder_h
//sturcture for rgb colors. in bitmap image, it was stored as GBR for some reason.
struct RGB_t{
	unsigned char GREEN;
	unsigned char BLUE;
	unsigned char RED;
};
typedef unsigned char Tiny;
//sturcture to hold info about the actual image (image capture code responsible for creating)
//size is size in byte, length is # of RGB values (probably unused, thus safe to set 0)
//width and height are used intensivly and it is important they are set correctly
//data is the actual data (allocated by malloc/new and removed by free/delete
struct RAW_BITMAP
{
	int width;
	int height;
	int size;
	int length;
	RGB_t * data;
};
//simple point, for average func ect.
struct Point
{
	int x;
	int y;
};
//boundary stuct (currently unused, so you can modify/use/remove if you need)
struct BDS{
	Point UR;
	Point UL;
	Point BL;
	Point BR;
};

Point SD(RAW_BITMAP P, Point u);
Point ProcRGBtoRMGM(RAW_BITMAP,RGB_t *, int);
RGB_t RRGB(Tiny R,Tiny G,Tiny B);
RGB_t operator+(RGB_t A,RGB_t B);
RGB_t operator-(RGB_t A,RGB_t B);
bool inline IsColRange(RGB_t D,RGB_t L,RGB_t H);

// Macros to help. (Macros are faster than functions as they are placed directly.
//inline functions are not guarenteed to be inline, macros will be every time


//!call this first to guarentee we got width/height (or just name those variables like so)
#define PrepPTMacro(w,h) int __width=w;int __height=h;
//get/set RGB from/to a point (assumes 0 based system starting at 0,0 going to width-1,height-1
//GetPoint requires a previous call to PrepPTMacro (which defines variables __width and __height)
#define GetPoint(r,x,y) (((RGB_t *)(r))[(y)*__width+(x)])
//X=GetPoint(r,x,y) and GetPoint(r,x,y)=X are both valid.
//Get Point can be used with any pointer as it casts first. so you can use this macro with byte/char array or void* as well as RGB_t *


//below is UNUSED
//new style works better by comparing values to expected values in an array
//old style was to hit and miss. if you remove tuning.h remove these following 3 lines
//obsolete code following
#define IsRed(rgb)		((rgb.RED > T_R_R) && (rgb.BLUE < T_R_B) && (rgb.GREEN < T_R_G))
#define IsWhite(rgb)	((rgb.RED > T_W_R) && (rgb.BLUE > T_W_B) && (rgb.GREEN > T_W_G))
#define IsBlue(rgb)		((rgb.RED < T_B_R) && (rgb.BLUE > T_B_B) && (rgb.GREEN < T_B_G))



#endif