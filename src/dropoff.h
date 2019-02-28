#include "cell.h"

#ifndef __DROPOFF_H__
#define __DROPOFF_H__

class Dropoff {
private: 
	Cell* cells[DROPOFFLEN];

public:
	Dropoff() {
		for (int i = 0; i < DROPOFFLEN; i++) {
			cells[i] = new Cell();
		}
	}
};

#endif

