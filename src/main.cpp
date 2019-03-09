#include <iostream>
#include <cstdlib>
#include <stdio.h>
#include "cpp.h"
#include <string.h>

// SIM specific code
#include "constants.h"
#include "roadway.h"
#include "car.h"

using namespace std;

bool simulating;
int carCount;

void carSpawner(Roadway* r) {
	create("Spawner");
	
	while(simulating) {
		hold(exponential(17.4));
		new Car(r, 0, carCount);
		carCount++;
	}
}

extern "C" void sim() {
	//trace_on();
	create("sim");
		
	simulating = true;
	carCount = 0;

	// Setup
	Roadway* r = new Roadway(8, 14, 1);
	
	// Main behavior
	carSpawner(r);

	hold(SIMTIME);
	simulating = false;
	//trace_off();
	report();
}

