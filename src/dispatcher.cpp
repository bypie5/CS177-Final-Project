#include "dispatcher.h"

void Dispatcher::dispatchLoop() {
	create("dispatcher");

	trafficLight->set();

	// The "zoneCnt"th car will set this event
	doneUnloading->wait();
	
	hold(1); // Simulate cars syncing up to drive	
	driveAway->set();

	// Reset data for next run
	trafficLight->clear();
	doneUnloading->clear();
	driveAway->clear();
}

