#include "car.h"
#include <math.h>
#include <cmath>

// Helper function definitions
#ifndef __HELPERS__
#define __HELPERS__
bool double_equals(double a, double b, double epsilon);
#endif


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

	// Pull into the school dropoff...
	if (head >= 0)
		precells[head]->occupy();
	if (tail >= 0)
		precells[tail]->occupy();

	// Driving state machine
	while (tail <= lenPre) {
		getLocation();
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
	/*while (tail < lenPost) {
		getLocation();
		driveSM(postcells, lenPost);
	}*/
}

void Car::driveSM(Cell** r, int len) {	
	// Similar to a frame rate
	double portionFraction = 10;
	double distanceDrifted = 0; // Used for reaction tim	
	roadway->printRoadway(r, len);
	switch(state) {
		case STOPPED:
			// Actions
			currSpeed = 0;
			hold(0.1); // Allow simulation time to advance while stopped
			// Transitions
			if (!obstacle(r, len)) {
				currSpeed = 1;
				printf("%f(Car %d): STOPPED->DRIVING\n", clock, id);
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
				printf("%f (Car %d): DRIVING->REACTING\n", clock, id);
				state = REACTING;
			} else {
				state = DRIVING;
			}
		break;
		case REACTING:
			// Actions 
			distanceDrifted = portionDriven;
			// Drive at current speed during delayed reaction time (1 second)
			for (int i = 0; i < portionFraction; i++) {
				double carPerSec_portion = (double) (1/secPerCar(currSpeed))/(2*portionFraction);
				
				portionDriven += carPerSec_portion;
				distanceDrifted += carPerSec_portion;
				// Update head and tail if portionDriven overflows or when needed
				if (distanceDrifted >= 0.5) {
					distanceDrifted -= 0.5; // Trim the difference
					head++; tail++;
					// printf("Drifted 1/2 a car length\n");
				}	
				portionDriven = portionDriven >= 1.0 ? portionDriven - 1.0 : portionDriven;
				
				if (head >= 0 && head < len && !r[head]->isBusy()) {
					r[head]->occupy();
				}
				if (tail >= 0 && tail < len && !r[tail-1]->isBusy()) {
					r[tail]->occupy();
				}
				// Advance simulation time
				hold(1/portionFraction);
				if (tail - 1 >= 0 && tail <= len && r[tail-1]->isBusy()) {
					r[tail-1]->free();
				}
			}
	
			// Transitions
			printf("%f (Car %d): REACTING->STOPPING\n", clock, id);
			distanceDrifted = 0; // Reset for next time
			state = STOPPING;
		break;
		case STOPPING:
			// Actions
			decreaseSpeed();
			driveCarLenPortion(r, len, portionFraction);		
	
			// Transitions
			if (currSpeed == 0) {
				printf("%f(Car %d): STOPPING->STOPPED\n", clock, id);
				state = STOPPED;
			} else if (currSpeed != 0 && !obstacle(r, len)) {
				printf("%f (Car %d): STOPPING->DRIVING\n", clock, id);
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
	for (int i = head+1; i <= head+monitorLen+1; i++) {
		if (i < len && path[i]->isBusy()) {
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

	if (head >= 0 && head < pathLen && !path[head]->isBusy()) {
		path[head]->occupy();
	}
	if (tail >= 0 && tail < pathLen && !path[tail]->isBusy()) {
		path[tail]->occupy();
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
		
		// Overflow protection
		if (currSpeed > TARGETSPEED) currSpeed = TARGETSPEED;
	}
}

void Car::decreaseSpeed() {
	if ((double_equals(portionDriven, 1.0) || double_equals(portionDriven, 0.5)) && currSpeed != 0) {
		currSpeed--;
		
		// Overflow protection
		if (currSpeed < 0) currSpeed = 0;
	}
}

