#include "cell.h"
#include "dropoff.h"

#ifndef __ROADWAY_H__
#define __ROADWAY_H__

class Roadway {
private:
	Cell** cells;
	int length; // Car lengths (2 cells per car)
	int zones; // Number of dropoff areas

public:
	Roadway(int length, int zones) {
		this->length = length;
		this->zones = zones;

		// Create the facilities for the roadway
		cells = (Cell**) malloc(sizeof(Cell*) * length * CARLEN);
		for (int i = 0; i < length * CARLEN; i++) {
			cells[i] = new Cell();
		}
	};


};

#endif

