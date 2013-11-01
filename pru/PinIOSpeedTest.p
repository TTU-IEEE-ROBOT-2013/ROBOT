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
 mov r5, T_WAIT * 30 //30 seconds
 mov r6, 0xFFFFFFFF  //Dummy subtraction to get 50% duty cycle
 mov r3, 0x0
 
 mov r1.b3, 0x26
 LoopMain:
   BitSetGPIO2 TMP, GPIO2_R
   sub r6,r6,1
   qble NoMore, r6, 0 //never taking this branch, but it takes its cycle like the second branch
   jmp Stupid_Jump
 Stupid_Jump:
   BitClrGPIO2 TMP,GPIO2_R
   sub r5,r5,1
   qble NoMore, r5, 0
   jmp LoopMain
 NoMore:
   //technically, this is not maximum speed,
   //but, theoretically, the time for the write/read to pass through will be much more than these anyway
   //otherwise, we've used same # of cycles, so we can still read time taken uing the scope.