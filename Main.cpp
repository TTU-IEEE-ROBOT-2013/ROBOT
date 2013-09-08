#include "Beagle.h"
#include "Tuning.h"
#include <stdio.h>
#include <stdlib.h>
int main()
{
	WriteF(LED0,"1");
	//gpio 2_5 = 32*2+5=69
	//
	c_bin_io A(3);
	A.SetIP();
	printf("%d",(int)A.Read());
}