all: ROBOT
	rm *.o
ROBOT: Beagle.o GunShot.o Main.o Nav.o Target.o
	mkdir -p out
	rm -f ./out/ROBOT
	g++ Beagle.o GunShot.o Main.o Nav.o Target.o -o ./out/ROBOT
	cp ./out/ROBOT /bin/ROBOT
Beagle.o: Beagle.cpp
	g++ -c Beagle.cpp
GunShot.o: GunShot.cpp
	g++ -c GunShot.cpp
Main.o: Main.cpp
	g++ -c Main.cpp
Nav.o: Nav.cpp
	g++ -c Nav.cpp
Target.o: Target.cpp
	g++ -c Target.cpp
