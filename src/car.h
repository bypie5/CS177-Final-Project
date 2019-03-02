#include "cpp.h"
#include "constants.h"
#include "roadway.h"

#ifndef __CAR_H__
#define __CAR_H__

class Car {
private:
	Roadway* roadway;
	int currSpeed;	
	int head;
	int tail;

public:
	Car(Roadway * r) {
		// Data setup
		this->roadway = r;
		head = 0;
		tail = -1;

		// Cars are created at start of the roadway with 0 speed per document
		currSpeed = 0;

		// Enter sim
		this->simCar();
	};

public:
	void simCar();

private:
	double secPerCar(int s);
};

#endif

