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
		precells[head]->occupy(id);
	if (tail >= 0)
		precells[tail]->occupy(id);

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
	zones[0]->reserveMe(id);
	hold(9.5);
	zones[0]->releaseMe(id);

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
	double totalDrifted = 0;	
	
	// See the traffic on the roadway
	roadway->printRoadway(r, len);
	
	switch(state) {
		case STOPPED:
			// Actions
			currSpeed = 0;
			hold(0.1); // Allow simulation time to advance while stopped
			// Transitions
			if (!obstacle(r, len)) {
				printf("%f(Car %d): STOPPED->DRIVING\n", clock, id);
				currSpeed = 1;
				state = DRIVING;
			} else {
				state = STOPPED;
			}
		break;
		case DRIVING:
			// Actions
			driveCarLenPortion(r, len, portionFraction, true);			
		
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
			distanceDrifted = (portionDriven > 0.5) ? portionDriven - 0.5 : portionDriven;
			totalDrifted = 0;
			
			// Drive at current speed during delayed reaction time (1 second)
			printf("EXPECTED DRIFT: %f\n",1./secPerCar(currSpeed));	
			for (int i = 0; i < portionFraction; i++) {
				double carPerSec_portion = (double) (1/secPerCar(currSpeed))/(portionFraction);
				
				portionDriven += carPerSec_portion;
				distanceDrifted += carPerSec_portion;
				totalDrifted += carPerSec_portion;
				// Update head and tail if portionDriven overflows or when needed
				if (distanceDrifted >= 0.5) {
					distanceDrifted -= 0.5; // Trim the difference
					head++; tail++;
				}	
				portionDriven = portionDriven >= 1.0 ? portionDriven - 1.0 : portionDriven;
				
				if (head >= 0 && head < len && !r[head]->isBusy()) {
					r[head]->occupy(id);
				}
				if (tail >= 0 && tail < len && !r[tail]->isBusy()) {
					r[tail]->occupy(id);
				}
				// Advance simulation time
				hold(1/portionFraction);
				if (tail - 1 >= 0 && tail <= len && r[tail-1]->isBusy()) {
					r[tail-1]->free(id);
				}
			}
	
			printf("Car %d drifted: %f\n", id, totalDrifted);

			// Transition
			printf("%f (Car %d): REACTING->STOPPING\n", clock, id);
			distanceDrifted = 0; // Reset for next time
			currSpeed--;
			if (currSpeed < 0) currSpeed = 0;
			state = STOPPING;
		break;
		case STOPPING:
			// Actions
			driveCarLenPortion(r, len, portionFraction, false);		

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
	for (int i = head; i <= head+monitorLen; i++) {
		if (i < len && path[i]->isBusy() && path[i]->getOwner() != id) {
			printf("Obstacle detected %d cells ahead while going %d speed\n", i-head, currSpeed);
			return true;
		}
	}

	return false;
}

// Because floating point comparisons aren't perfect...
bool double_equals(double a, double b, double epsilon = 0.001) {
	return std::abs(a - b) < epsilon;
}

double truncateToTenths(double val) {
	if (abs(val - (floor(val*10.)/10.)) < abs(val - (ceil(val*10.)/10.)))	{
		return floor(val * 10.)/10.; 
	} else {
		return ceil(val*10.)/10.;
	}
}

void Car::driveCarLenPortion(Cell** path, int pathLen, double portionFraction, bool accelerate) {
	// How long does it take to travel that portion of a car len 
	double portionTime = (double) (secPerCar(currSpeed)/portionFraction);
	portionDriven = truncateToTenths(portionDriven);	
	
	portionDriven = portionDriven > 1.0 ? 0 : portionDriven;
	printf("Car %d portionDriven: %f\n", id, portionDriven);

	// Increase head and tail
	if (double_equals(portionDriven, 0.5) || double_equals(portionDriven, 1.0)) {
		if (!obstacle(path, pathLen)) {
			head++; tail++;
		}

		// Increase speed 1 unit per car length
		if (accelerate && double_equals(portionDriven, 1.0) && !obstacle(path, pathLen)) {
			currSpeed++;
			if (currSpeed > TARGETSPEED) currSpeed = TARGETSPEED;
		}
		
		// Decrease speed 2 units per car length
		if (!accelerate) {
			currSpeed--;
			if (currSpeed < 0) currSpeed = 0;	
		}	
	}

	if (head >= 0 && head < pathLen && !path[head]->isBusy()) {
		path[head]->occupy(id);
	}
	if (tail >= 0 && tail < pathLen && !path[tail]->isBusy()) {
		path[tail]->occupy(id);
	}
	hold(portionTime);	
	if (tail - 1 >= 0 && tail <= pathLen && path[tail-1]->isBusy()) {
		path[tail-1]->free(id);
	}	
	
	portionDriven += ((double)1.0/portionFraction);
}

