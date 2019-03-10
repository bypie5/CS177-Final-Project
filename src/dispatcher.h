#include "cpp.h"

#ifndef __DISPATCHER_H__ 
#define __DISPATCHER_H__

class Dispatcher {
private:
	int zoneCnt;
	event* trafficLight;
	event* doneUnloading;
	event* driveAway;

public:
	Dispatcher(int zoneCnt) {
		// Data setup
		this->zoneCnt = zoneCnt;
		trafficLight = new event("traffic light");
		doneUnloading = new event("done unloading");
		driveAway = new event("drive away");	
	};

	void dispatchLoop();

	event* getTrafficLight() {
		return this->trafficLight;
	}

	event* getDoneUnloading() {
		return this->doneUnloading;
	}

	event* getDriveAway() {
		return this->driveAway; 
	}
};

#endif

