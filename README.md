opROBOT
=====
g++ -o OP pic.cpp `pkg-config --libs opencv`
All c code / pru asm used in our robot
The new beggining

TODO:   Create pru test for navigation:
		Create c++ test for navigation (first test)
		Finish target finding test for Target Acquisition
		Do some HSL tests, with varying lighting and such.
		Generate simple GPIO / PWM / I2C / UART tests
		they won't do any more than generate dummy signals
		to read from 'scope.  also read a constant input
		just attach a switch to 3.3V and a pull resistor to GND.