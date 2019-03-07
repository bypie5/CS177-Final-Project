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

void carSpawner(Roadway* r) {
	new Car(r, 0, 1);
	new Car(r, 2, 2);
}

extern "C" void sim() {
	//trace_on();
	create("sim");

	// Setup
	Roadway* r = new Roadway(10, 10, 1);
	
	// Road block
	r->getPrecells()[10]->occupy(5);
	r->getPrecells()[11]->occupy(5);
	
	// Main behavior
	carSpawner(r);

	hold(SIMTIME);
	//trace_off();
	//report();
}

