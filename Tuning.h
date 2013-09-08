/* note: greater than is > not >= (same for less than)
	this offers some performance increase*/
#ifndef Tuning
#define Tuning
//gpio all exist in this folder

//definitions of some common pins for i/o (if it is defined 0, it is NOT DEFINED!)
#define ION 0//a pin for second "Power Switch" to enable the  robot on the track 
//(avoids problem with boot time as it is booted before it is put on the track
#define LT1 0 
#define LT2 0
#define LT3 0
#define LT4 0
#define LT5 0
#define LT6 0
#define LT7 0
#define LT8 0
#define LT9 0
#define LT10 0
//motor speed controled by 3 bit DAC (separate circuit)
#define LeftDrive {0,0,0,0}
#define RightDrive {0,0,0,0}
//text pin definitions

#define LED0 "/sys/class/leds/beaglebone:green:usr0/brightness" //turned on at start
#define LED1 "/sys/class/leds/beaglebone:green:usr1/brightness" //can be used for debuging/error reporting 
#define LED2 "/sys/class/leds/beaglebone:green:usr2/brightness" //can be used for debuging (when run starts)
#define LED3 "/sys/class/leds/beaglebone:green:usr3/brightness" //reserved for heartbeat.
#define ADCT "/sys/bus/iio/devices/iio:device0/in_voltage%d_raw"
#define BEAGLE_CAPE_SLOTS "/sys/devices/bone_capemgr.*/slots"

#define GPIO_EXPORT 
//motor direction controled by excitation. (digital out 0 = positive, digital out 1 = negative)
//motor driver will be designed accordingly. 
#define LeftExite 0
#define RightExite 0



//definitions of some common values required
#define NavMaxLineWidth 2 
//value from 1 to 10 specifies line width. anything over is considered to be intersection
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