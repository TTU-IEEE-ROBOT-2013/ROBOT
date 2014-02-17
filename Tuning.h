#ifndef Tuning
#define Tuning
//definition for motor speed (max 255)
#define TARG_SPEED 64
//definition for value to int speed conversion
//note that this does not affect steady state, but may 
//make transients more desirable if set correctly.
//also changing this sign does negation through out the chain.
//(if polarity is invalid, the bot will go into left field,
//so the polarity is the most important feature)
#define SPD_FACT 10.0
#define AVG_SPEED 127
#ifndef NULL
#define NULL 0
#endif
//Definitios for adaptive PID controller
//when to use MAX_*
#define APID_THROW 1.0
//When to use MIN_*
#define APID_FALL 0.0
//rate to decrease integral (1 is instant, 0 is never)
#define APID_INTEGRAL_CURE .5
//P,I,and D constants for PID controller for Navigator
#define NAV_P 1
#define NAV_I 1
#define NAV_D .0001
//FIR length for pure adaptive LMS control system
#define FIR_LEN 50
//gpio all exist in this folder
//motor //9_13 (uart 4)
#define UART_4 "ADAFRUIT-UART4"
//definitions for Pan and Tilt
//use as PWMAccumulator(PAN_PWM)
#define DELTA_ACC .001  //decrease to increase stability

//8_13 pan
#define PAN_PWM_PIN 8,13
//9_14 tilt
#define TILT_PWM_PIN 9,14
#define PAN_TILT_FS   20000000
#define PAN_INIT   900000
#define TILT_INIT  900000
#define PAN_PWM  PAN_PWM_PIN,	PAN_TILT_FS, PAN_INIT
#define TILT_PWM TILT_PWM_PIN,	PAN_TILT_FS, TILT_INIT
//Flywheels
//8_10  68 (2-4)
#define FW_GPIO       68
//Linear Actuators
#define LA_GPIO       26
#define LA_CLOSED     false
#define LA_OPEN       true
//LEDs
#define LED_GPIO		
#define CX_SCN         360/2
#define CY_SCN         240/2
#define CX_EPS         10
#define CY_EPS         10
//LINE SENSOR defines
//the first 4 may need updated
#define DP 0
#define DM 1
#define TP1 2 //error and line test
#define TP2 3
#define OUT_OF_BOUNDS -1e100
#define TURN_TIME 1e100
#define TURN_THRESH 1.5 //We know to turn when
#define EZ_APPROXIMATION 1.1
//reserved macros (not used in code, but specify pins used by other interfaces)
//reserved of color-sense
#define NO_USE_COLOR_SENSE 9,21;9,22 //other i2c devices can go here
#define NO_USE_MMC 8,{22 23 24 3 4 5 6 20 21 25}
#define AVOID_USE_HDMI 8,{27-46} //we shouldn't use them
//usable are p8 { 7,8,9,10,11,12,13,14,15,16,17,18,19,26 }
//			 p9 { 11-24,26-31,41,42 }
//free on p8 (none) (p9 all but 21,22 <i2c> and 11,12
//special pins are {9,21;9,22} i2c: 8,13;8,19;9,14;9,15;9,42;9,29;9,31; PWM
//					9,24;9,26 UART 1
//text pin definitions
#define LED0 "/sys/class/leds/beaglebone:green:usr0/brightness" //turned on at start
#define LED1 "/sys/class/leds/beaglebone:green:usr1/brightness" //can be used for debuging/error reporting 
#define LED2 "/sys/class/leds/beaglebone:green:usr2/brightness" //can be used for debuging (when run starts)
#define LED3 "/sys/class/leds/beaglebone:green:usr3/brightness" //reserved for heartbeat.
#define ADCT "/sys/devices/ocp.2/helper.14/AIN%d"
#define BEAGLE_CAPE_SLOTS "/sys/devices/bone_capemgr.8/slots" 




//definitions of some common values required
//speed at which to turn. this is in reference to N/256*MAX_SPEED
//the motors will turn at slightly different speeds, so we need to adjust them somewhat.

//start motor speed
#define DEF_SPEED_L 100
#define DEF_SPEED_R 100

//TurnSpeed (Avoid using to "FINE TUNE")
#define TurnSpeedR 110
#define TurnSpeedL -120
#define TurnSpeedT TurnSpeedR,TurnSpeedL  //this may need flipped
#define TurnSpeedT2 TurnSpeedR/2,TurnSpeedL/2
#define RotationsPer90dTurn 1 //assuming 3" wheels and 12" axle length, 2*pi*3/2*pi*12=1/4 (1/4 of a circle is 90d)
//RGB limits for block detection.
//change these based on testing!!!!
//There may be 2 (one for blue, one for red)
#define NAV_RL 1000
#define NAV_BL 1000
#define NAV_GL 1000
#define NAV_RH 1024
#define NAV_BH 1024
#define NAV_GH 1024
//change this to include both tests, if a second is required
#define IS_BLOCK(red,green,blue) (NAV_RL < red   && red   < NAV_RH  && \
				NAV_BL < blue  && blue  < NAV_BL && \
				NAV_GL < green && green < NAV_GL)
				
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