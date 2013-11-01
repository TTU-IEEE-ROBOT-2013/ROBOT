void EnableGPIO(int inPin)
{
	TV=new char[10];
	FILE * TP=fopen("/sys/class/gpio/export", "a");
	//sprintf(TV,"%d",Pin);
	if(TP!=0)
	{
		fprintf(TP,"%d",inPin);
		fclose(TP);
	}
	OK=true;

}
void DisableGPIO(int IPin)
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
//#define GPIOI_J(I,J) ((32*I)+J)