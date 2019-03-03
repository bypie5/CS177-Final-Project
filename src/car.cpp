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
		getLocation();
		driveSM();
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
	
}

void Car::driveSM() {	
	double portionFraction = 10;
	
	switch(state) {
		case STOPPED:
			// Actions
			currSpeed = 0;

			// Transitions
			if (!obstacle()) {
				state = DRIVING;
			} else {
				state = STOPPED;
			}
		break;
		case DRIVING:
			currSpeed = 1;
			// Actions
			driveCarLenPortion(precells, lenPre, portionFraction);			
		
			// Transitions
			if (obstacle()) {
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
			} else if (currSpeed != 0 && !obstacle()) {
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

bool Car::obstacle() {
	return false;
}

bool double_equals(double a, double b, double epsilon = 0.001) {
	return std::abs(a - b) < epsilon;
}

void Car::driveCarLenPortion(Cell** path, int pathLen, double portionFraction) {
	// How long does it take to travel that portion of a car len 
	double portionTime = (double) (secPerCar(currSpeed)/portionFraction);

	// Increase head and tail
	if (double_equals(portionDriven, 0.5) || double_equals(portionDriven, 1.0)) {
		head++; // (double) (2.0/portionFraction);
		tail++; // (double) (2.0/portionFraction);
	}

	// Reserve head and tail (if new whole #)
	int iHead = (int) floor(head);
	int iTail = (int) floor(tail);
	
	printf("por: %f, iHead: %d, iTail: %d\n\n", portionDriven, iHead, iTail);	

	if (iHead < pathLen && !path[iHead]->isBusy()) {
		path[iHead]->occupy();
	}
	hold(portionTime);	
	if (iTail - 1 >= 0 && iTail <= pathLen && path[iTail-1]->isBusy()) {
		path[iTail-1]->free();
	}	
	
	portionDriven += ((double)1.0/portionFraction);
	portionDriven = (portionDriven >= 1.0) ? 0 : portionDriven;
}

