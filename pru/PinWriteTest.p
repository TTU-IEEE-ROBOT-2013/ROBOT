 .origin 8
#include "Base.hp"
#include "Init.hp"
 //;let r29 be temp register
 #define TMP r29
 #define T_WAIT 100000000
 //;let r1 be set/clear register for GPIO2
 #define GPIO2_R r1
 mov r1,0x0 //line 9
 //;GPIO 2: 2,3,5,4
 //;enable these using file i/o in linux befor loading this
 mov r5, 0x0
 mov r1.b3, 0x26
 LoopMain:
   BitSetGPIO2 TMP, GPIO2_R
   mov r3,0
   mov r4,T_WAIT
 LoopDelay:
   sub r3,r3,1
   qbge LoopDelay, r4, r3 //; wait for 200,000,000/200,000,000 seconds
   BitClrGPIO2 TMP,GPIO2_R
   mov r3,0
   mov r4,T_WAIT
 LoopDelay2:
   sub r3,r3,1
   qbge LoopDelay2,r4,r3 //; wait another second
   add r5,r5,1
   qbge NoMore, r5, 10
   jmp LoopMain
 NoMore:
   
   //; wait 1 sec.