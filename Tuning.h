/* note: greater than is > not >= (same for less than)
	this offers some performance increase*/
#ifndef Tuning
#define Tuning
//gpio all exist in this folder

//definitions of some common pins for i/o (if it is defined 0, it is NOT DEFINED!)
#define ION 0//a pin for second "Power Switch" to enable the  robot on the track 
//(avoids problem with boot time as it is booted before it is put on the track
				//all LT's on header p8
#define LT1 66 //7	2[2]	32*2+2
#define LT2 67 //8	2[3]
#define LT3 69 //9	2[5]
#define LT4 68 //10	2[4]
#define LT5 45 //11	1[13]
#define LT6 44 //12	1[12]
#define LT7 26 //14	0[26]
#define LT8 47 //15	1[15]
#define LT9 46 //16	1[14]
#define LT10 59 //17	1[27]
//motor direction 0=fwd, 1=rev 
#define LeftExite 65 //p8_18 2[1] 65
#define RightExite 61//p8_26 1[29] 61
//motor speed controled by pwm
#define LeftDrive 8,13
#define RightDrive 8,19
//definitions for pwm output
#define PERIOD  250000 //250000 ns = 1/4000 (for 4000 hz frequency)
#define MAX_DUTY 250000 //if it is too fast, lower this to lower the speed. 
//reserved macros (not used in code, but specify pins used by other interfaces)
//reserved of color-sense
#define NO_USE_COLOR_SENSE 9,21;9,22 //other i2c devices can go here
#define NO_USE_MMC 8,{22 23 24 3 4 5 6 20 21 25}
#define AVOID_USE_HDMI 8,{27-46} //we shouldn't use them
//usable are p8 { 7,8,9,10,11,12,13,14,15,16,17,18,19,26 }
//			 p9 { 11-24,26-31,41,42 }
//free on p8 (none) (p9 all but 21,22 <i2c>
//special pins are {9,21;9,22} i2c: 8,13;8,19;9,14;9,15;9,42;9,29;9,31; PWM
//					9,24;9,26 UART 1
//text pin definitions
#define LED0 "/sys/class/leds/beaglebone:green:usr0/brightness" //turned on at start
#define LED1 "/sys/class/leds/beaglebone:green:usr1/brightness" //can be used for debuging/error reporting 
#define LED2 "/sys/class/leds/beaglebone:green:usr2/brightness" //can be used for debuging (when run starts)
#define LED3 "/sys/class/leds/beaglebone:green:usr3/brightness" //reserved for heartbeat.
#define ADCT "/sys/bus/iio/devices/iio:device0/in_voltage%d_raw"
#define BEAGLE_CAPE_SLOTS "/sys/devices/bone_capemgr.*/slots"

#define GPIO_EXPORT 




//definitions of some common values required
//value from 1 to 10 specifies line width. anything over is considered to be intersection
#define NavMaxLineWidth 2 
//speed at which to turn. this is in reference to MAX_DUTY / 16  which is( (TurnSpeed*Max_DUTY/(16*PERIOD))*3.3)V
#define TurnSpeed 8
//RGB limits for block detection.
#define NAV_RL 50
#define NAV_BL 50
#define NAV_GL 50
#define NAV_RH 1024
#define NAV_BH 1024
#define NAV_GH 1024
//GPIO ADDRESS FOR USE IN PRU CODE (yes, not this code)
/*
#define GPIO0 0x44E05000
#define GPIO1 0x4804c000 //gpio address
#define GPIO2 0X481AC000 
#define GPIO3 0x481AE000
#define GPIO_CLEARDATAOUT 0x190 //GPIO high
#define GPIO_SETDATAOUT 0x194	//GPIO low
*/
#endif