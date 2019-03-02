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
	
	int cursor = 0; // Where is the car relative to end of strip?

	// Drive down precells
	while (cursor <= lenPre) {
		cursor++;
	} cursor = 0; // Reset for next drive

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
			return -1; // Stopped car
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
			return -1;
		break;
	}
}

