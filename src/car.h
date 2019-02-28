#include "cpp.h"
#include "constants.h"
#include "roadway.h"

#ifndef __CAR_H__
#define __CAR_H__

class Car {
private:
	Roadway* roadway;

public:
	Car(Roadway * r) {
		// Data setup
		this->roadway = r;

		// Enter sim
		this->simCar();
	};

public:
	void simCar();
};

#endif

