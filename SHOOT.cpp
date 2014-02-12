#include <fstream>
#include <iostream>
#include <cstdio>
#include <cstdlib>

using namespace std;
#define PWM1duty "/sys/devices/ocp.2/pwm_test_P8_13.14/duty"
#define PWM1period "/sys/devices/ocp.2/pwm_test_P8_13.14/duty"
#define PWM2duty "/sys/devices/ocp.2/pwm_test_P8_13.14/duty"
#define PWM2period "/sys/devices/ocp.2/pwm_test_P8_13.14/duty"

#define GPIO_1_28_N 60
#define GPIO_1_16_N 48
#define GPIO_1_28    "/sys/class/gpio/gpio60"
#define GPIR_1_16    "/sys/class/gpio/gpio48"
#define GPIO_EXPORT  "/sys/class/gpio/export"
#define GPIO_UEXPORT "/sys/class/gpio/unexport"

//1_28 gpio = 32 + 28

#define slots "/sys/devices/bone_capemgr.8/slots"
void Set_Pwm(int duty1, int duty2)
{
	fstream EXPORT;
	EXPORT.open(PWM2duty,ios::out | ios::in);
	EXPORT << duty1;
	EXPORT.flush();
	EXPORT.close();
	EXPORT.open(PWM1duty,ios::out | ios::in);
	EXPORT << duty2;
	EXPORT.flush();
	EXPORT.close();
}
void init()
{
	//create our pwms
	fstream EXPORT;
	EXPORT.open(slots,ios::out | ios::in);
	EXPORT << "am335x_pwm";
	EXPORT.flush();
	EXPORT.close();
	EXPORT.open(slots,ios::out | ios::in);
	EXPORT << "bone_pwm_P8_13";
	EXPORT.flush();
	EXPORT.close();
	EXPORT.open(slots,ios::out | ios::in);
	EXPORT << "bone_pwm_P9_14";
	EXPORT.flush();
	EXPORT.close();	
	EXPORT.open(slots,ios::out | ios::in);
	EXPORT << "ADAFRUIT-UART4";
	EXPORT.flush();
	EXPORT.close();	
	//Create our 2 gpio
	FILE * GPO;
	GPO=fopen(GPIO_EXPORT,"a");
	//cout<<(int)GPO;
	//cout.flush();
	fprintf(GPO,"%d",GPIO_1_28_N);
	fclose(GPO);
	GPO=fopen(GPIO_EXPORT,"a");
	//cout<<(int)GPO;
	//cout.flush();
	fprintf(GPO,"%d",GPIO_1_16_N);
	fclose(GPO);
	EXPORT.open(PWM2period,ios::out | ios::in);
	EXPORT << 20000000;
	EXPORT.flush();
	EXPORT.close();
	EXPORT.open(PWM1period,ios::out | ios::in);
	EXPORT << 20000000;
	EXPORT.flush();
	EXPORT.close();
	
	
}
void DeInit()
{
	FILE * GPO;
	GPO=fopen(GPIO_UEXPORT,"a");
	//cout<<(int)GPO;
	//cout.flush();
	fprintf(GPO,"%d",GPIO_1_28_N);
	fclose(GPO);
	GPO=fopen(GPIO_UEXPORT,"a");
	//cout<<(int)GPO;
	//cout.flush();
	fprintf(GPO,"%d",GPIO_1_16_N);
	fclose(GPO);

}
int main()
{
	init();
}