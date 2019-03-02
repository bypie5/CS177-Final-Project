#include "car.h"

// Lifetime and behavior of a car
void Car::simCar() {
	create("Car");
	
	// Get parts of the road for later usage (roadway.h explains parts of road)
	Cell** precells = roadway->getPrecells();
	int lenPre = roadway->getPrecellsLen();
	Dropoff** zones = roadway->getDropoffs();
	int lenZones = roadway->getDropoffCount();
	Cell** postcells = roadway->getPostcells();
	int lenPost = roadway->getPostcellsLen();
	
	// Car pulls into the roadway (ie head is zero)...
	precells[head]->occupy();

	// Drive down precells
	while (tail <= lenPre) {
		// Adjust target speed (if able to do so)
		if (currSpeed < TARGETSPEED)
			currSpeed++;

		double velocity = secPerCar(currSpeed);
		// Move at current speed
		if (velocity > 0) {
			for (int i = 0; i < 2; i++) {
				// Move half car len
				head++; tail++;
				if (head < lenPre)
					precells[head]->occupy();
				hold(velocity/2);

				// Release cell behind tail
				if (tail - 1 >= 0 && tail <= lenPre)
					precells[tail-1]->free();
				}
		}
	}

	// Reset for next drive
	head, tail = 0;

	// Reserve and drop off at furthest zone
	zones[0]->reserveMe();
	hold(9.5);
	zones[0]->releaseMe();

	// Drive off and exit world via postcells
	
}

// Defined in documentation
double Car::secPerCar(int s) {
	switch(s) {
		case 0:
			return 0; // Stopped car
		break;
		case 1:
			return 3;
		break;
		case 2:
			return 11.0/6.0;
		break;
		case 3:
			return 1;
		break;
		case 4:
			return 2.0/3.0;
		break;
		case 5:
			return 0.5;
		break;
		default:
			return 0;
		break;
	}
}

