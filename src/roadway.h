#include "cell.h"
#include "dropoffZone.h"

#ifndef __ROADWAY_H__
#define __ROADWAY_H__

class Roadway {
private:
	Cell** precells;
	//Dropoff** dropoffs;
	DropoffZone* dropoffZone;
	Cell** postcells;
	int prelength; // Car lengths (2 cells per car)
	int zones; // Number of dropoff areas
	int postlength;

public:
	// Precells   Zones       Postcells
	// [*****]|...|...|...|[*************]
	Roadway(int prelength, int postlength, int zones) {
		this->prelength = prelength;
		this->zones = zones;
		this->postlength = postlength;

		// Create the facilities for the roadway (2 cells per length unit)
		precells = (Cell**) malloc(sizeof(Cell*) * prelength * CARLEN);
		for (int i = 0; i < prelength * CARLEN; i++) {
			precells[i] = new Cell();
		}

		dropoffZone = new DropoffZone(this->zones);

		postcells = (Cell**) malloc(sizeof(Cell*) * postlength * CARLEN);
		for (int i = 0; i < postlength * CARLEN; i++) {
			postcells[i] = new Cell();
		}
	};

public:
	Cell** getPrecells() { return this->precells; };
	int getPrecellsLen() { return this->prelength*CARLEN; };

	DropoffZone* getDropoffs() { return this->dropoffZone; };
	int getDropoffCount() { return this->zones; };

	Cell** getPostcells() { return this->postcells; };
	int getPostcellsLen() { return this->postlength*CARLEN; };

	void printRoadway(Cell** r, int len) {
		for (int i = 0; i < len; i++) {
			if (r[i]->isBusy()) {
				printf("[x]");
			} else {
				printf("[%d]", i);
			}
		}
		printf("\n\n");
	}
};

#endif

