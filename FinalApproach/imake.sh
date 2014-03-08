#!/bin/bash
echo Building Application
g++ -o IT.out Beagle.cpp ImageShip.cpp `pkg-config --libs opencv`
echo ImageShip Built
echo done 
