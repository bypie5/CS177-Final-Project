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

extern "C" void sim() {
	trace_on();
	create("sim");
	Roadway* r = new Roadway(10, 10, 1);
	new Car(r);
	hold(100);
	trace_off();
}

