#include "dispatcher.h"

void Dispatcher::dispatchLoop() {
	trafficLight->set();

	// The "zoneCnt"th car will set this event
	doneUnloading->wait();

	driveAway->set();
	hold(0.1); // Wait small amount of time
	
	// Reset data for next run
	trafficLight->clear();
	doneUnloading->clear();
	driveAway->clear();
}

