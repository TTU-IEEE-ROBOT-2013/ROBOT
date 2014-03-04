#!/bin/bash
echo Building Application
g++ -o a.out Beagle.cpp MotherShip.cpp `pkg-config --libs opencv`
echo Mothership Built
g++ -o /bin/start Beagle.cpp T_BOOT.cpp
echo Pin Fix Built
echo done 
