#include "Beagle.h"
#include "Tuning.h"
#include <stdio.h>
#include <stdlib.h>
#include "Main.h"
int main()
{
	WriteF(LED0,"1");
	//gpio 2_5 = 32*2+5=69
	Nav_init(); //init navigational structures
	GunInit(); //init structures for gun
	Navigate();
	GunDone();
	//calls to ShootTarget come from within Navigate
}