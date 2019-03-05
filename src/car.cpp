#include "car.h"
#include <math.h>
#include <cmath>

// Lifetime and behavior of a car
void Car::simCar() {
	create("Car");
	
	// Get parts of the road for later usage (roadway.h explains parts of road)
	precells = roadway->getPrecells();
	lenPre = roadway->getPrecellsLen();
	zones = roadway->getDropoffs();
	lenZones = roadway->getDropoffCount();
	postcells = roadway->getPostcells();
	lenPost = roadway->getPostcellsLen();

	// Pull into the roadway
	precells[0]->occupy();

	// Driving state machine
	while (tail <= lenPre) {
		driveSM(precells, lenPre);
	}

	// Reset for next drive
	head = 0;
	tail = -1;
	state = STOPPED;	

	// Reserve and drop off at furthest zone
	zones[0]->reserveMe();
	hold(9.5);
	zones[0]->releaseMe();

	// Drive off and exit world via postcells
	while (tail < lenPost) {
		driveSM(postcells, lenPost);
	}	
}

void Car::driveSM(Cell** r, int len) {	
	double portionFraction = 10;
	
	switch(state) {
		case STOPPED:
			// Actions
			currSpeed = 0;

			// Transitions
			if (!obstacle(r, len)) {
				currSpeed = 1;
				state = DRIVING;
			} else {
				state = STOPPED;
			}
		break;
		case DRIVING:
			// Actions
			increaseSpeed(); // Speed increases every carlen if possible
			driveCarLenPortion(r, len, portionFraction);			
		
			// Transitions
			if (obstacle(r, len)) {
				state = REACTING;
			} else {
				state = DRIVING;
			}
		break;
		case REACTING:
			// Actions 
			
			// Transitions
			state = STOPPING;
		break;
		case STOPPING:
			// Actions
		
			// Transitions
			if (currSpeed == 0) {
				state = STOPPED;
			} else if (currSpeed != 0 && !obstacle(r, len)) {
				state = DRIVING;
			}
		break;
		default:
		break;
	}
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

bool Car::obstacle(Cell** path, int len) {
	// Calculate monitoring distance
	switch(currSpeed) {
		case 5:
			monitorLen = CARLEN*4;
		break;
		case 4:
			monitorLen = CARLEN*3;
		break;
		case 3:
			monitorLen = CARLEN*2;
		break;
		case 2:
			monitorLen = CARLEN*2;
		break;
		case 1:
			monitorLen = CARLEN;
		break;
		default:
			monitorLen = CARLEN;
		break;
	}	

	// Check cells for monitor distance
	for (int i = head+1; i < monitorLen; i++) {
		if (i < len) {
			if (path[i]->isBusy())
				return true;
		}
	}

	return false;
}

// Because floating point comparisons aren't perfect...
bool double_equals(double a, double b, double epsilon = 0.001) {
	return std::abs(a - b) < epsilon;
}

void Car::driveCarLenPortion(Cell** path, int pathLen, double portionFraction) {
	// How long does it take to travel that portion of a car len 
	double portionTime = (double) (secPerCar(currSpeed)/portionFraction);

	portionDriven = portionDriven >= 1.0 ? 0 : portionDriven;
	// Increase head and tail
	if (double_equals(portionDriven, 0.5) || double_equals(portionDriven, 0.9)) {
		head++;
		tail++;
	}

	if (head < pathLen && !path[head]->isBusy()) {
		path[head]->occupy();
	}
	hold(portionTime);	
	if (tail - 1 >= 0 && tail <= pathLen && path[tail-1]->isBusy()) {
		path[tail-1]->free();
	}	
	
	portionDriven += ((double)1.0/portionFraction);
}

void Car::increaseSpeed() {
	if (double_equals(portionDriven, 1.0) && currSpeed != TARGETSPEED) {
		currSpeed++;
	}
}

