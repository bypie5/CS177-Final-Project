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
	int startLoc;
	int id;
	int currSpeed;	
	int head;
	int tail;
	int monitorLen; // By # of cells
	int state;
	double portionDriven; // What fraction of a carlen has been driven?

	Cell** precells;
	int lenPre;
	Dropoff** zones;
	int lenZones;
	Cell** postcells;
	int lenPost;

public:
	Car(Roadway * r, int startLoc, int id) {
		// Data setup
		this->id = id;
		this->roadway = r;
		this->startLoc = startLoc;
		head = startLoc;
		tail = startLoc-1;
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
		printf("%f (Car %d): head: %d, tail: %d Speed(%d)\n", clock, id, head, tail, currSpeed);
	}
private:
	void driveSM(Cell** r, int len);
	double secPerCar(int s);
	bool obstacle(Cell** path, int len);
	void driveCarLenPortion(Cell** p, int pl, double portionFraction, bool accelerate);
	void increaseSpeed();
	void decreaseSpeed();
};

#endif

