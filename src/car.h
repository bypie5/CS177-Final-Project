#include "cpp.h"
#include "constants.h"
#include "roadway.h"

#ifndef __CAR_H__
#define __CAR_H__

#define STOPPED  0
#define DRIVING  1
#define REACTING 2
#define STOPPING 3

class Car {
private:
	Roadway* roadway;
	int currSpeed;	
	double head;
	double tail;
	int monitorLen; // By # of cells
	int state;
	double portionDriven;

	Cell** precells;
	int lenPre;
	Dropoff** zones;
	int lenZones;
	Cell** postcells;
	int lenPost;

public:
	Car(Roadway * r) {
		// Data setup
		this->roadway = r;
		head = 0;
		tail = -1;
		monitorLen = 2;
		state = STOPPED;
		portionDriven = 0;

		// Cars are created at start of the roadway with 0 speed per document
		currSpeed = 0;

		// Enter sim
		this->simCar();
	};

public:
	void simCar();
	void getLocation() {
		printf("%f: head: %f, tail: %f\n", clock, head, tail);
	}
private:
	void driveSM();
	double secPerCar(int s);
	bool obstacle();
	void driveCarLenPortion(Cell** p, int pl, double portionFraction);
};

#endif

