#include "cell.h"

#ifndef __DROPOFFZONE_H__
#define __DROPOFFZONE_H__

class DropoffZone {
private:
	facility_ms* zones;
	int zoneCnt;	
	Cell* cells;

public:
	DropoffZone(int zoneCnt) {
		this->zoneCnt = zoneCnt;
		zones = new facility_ms("zones", (long) zoneCnt);
		cells = new Cell[zoneCnt * DROPOFFLEN];
	};

	void reserveMe(int id) {
		zones->reserve();
	}

	void releaseMe(int id) {
		zones->release();
	} 

private:
	// Turns zone index into what cells (i, i+1) need to be reserved
	int cellBins(int zoneIndex) {
		return zoneIndex + 1; // lol... why do I need this???
	}

};

#endif

