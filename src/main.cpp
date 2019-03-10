#include <iostream>
#include <cstdlib>
#include <stdio.h>
#include "cpp.h"
#include <string.h>

// SIM specific code
#include "constants.h"
#include "roadway.h"
#include "car.h"
#include "dispatcher.h"

using namespace std;

bool simulating;
int carCount;

// Simulates imperfect human drivers
void carSpawner(Roadway* r) {
	create("car spawner");
	
	while(simulating) {
		hold(exponential(17.4));
		new Car(r, 0, carCount);
		carCount++;
	}
}

// Spawn autonomous cars to in waves to pick up children
void smartSchool(Roadway* r, Dispatcher* d) {
	int zoneCnt = r->getDropoffCount();
	create("smart school spawner");
	while (simulating) {
		hold(exponential(17.4));
		#ifdef DEBUG
		printf("%f: New batch of %d cars...\n", clock, zoneCnt);
		#endif
		
		// Spawn new cars when old cars start to drive away
		for (int i = 0; i < zoneCnt; i++) {
			#ifdef DEBUG
			printf("New car spawned\n");
			#endif
			int newPos = CARLEN - ((zoneCnt - i) * CARLEN);
			new Car(r, newPos, carCount, d);
			carCount++;
		}
		
		// Main behavior
		d->dispatchLoop();
		
		// Current (soon to be old) cars are driving away
		d->getDriveAway()->wait();
	}
}

extern "C" void sim(int argc, char** argv) {
	if (argc != 4) {	
		printf("\nERROR! Usage: ./dropoff_sim <car lengths before dropoff zones> <# of dropoff zones> <car lengths after dropoff zones>\n");
		printf("Suggested (Based on University Heights Middle School in Riverside, CA) \'./dropoff_sim 8 14 3\'\n\n");
	} else {
		// Parse user input
		int pre = atoi(argv[1]);
		int zone = atoi(argv[3]);
		int post = atoi(argv[2]);		

		create("sim");

		#ifdef DEBUG
		trace_on();	
		#endif

		#ifdef AUTOPILOT
		printf("\n\n");
		for (int i = 0; i < 80; i++) { printf("*"); } printf("\n");
		printf("IMPORTANT: Using auto-pilot\n");
		for (int i = 0; i < 80; i++) { printf("*"); } printf("\n\n");
		#endif

		simulating = true;
		carCount = 0;

		// Setup
		Roadway* r = new Roadway(pre, post, zone);
		#ifdef AUTOPILOT
		Dispatcher* d = new Dispatcher(zone);
		#endif	

		// Main behavior
		#ifndef AUTOPILOT
		carSpawner(r);
		#endif
	
		#ifdef AUTOPILOT
		smartSchool(r, d);
		#endif

		hold(SIMTIME);
		simulating = false;
		
		#ifdef DEBUG	
		trace_off();
		#endif

		report();
	}
}

