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

public:
	bool isBusy() {
		int num_busy = 0;

		for (int i = 0; i < DROPOFFLEN; i++) {
			if (cells[i]->isBusy()) num_busy++;
		}		

		if (num_busy != 0) {
			return true;
		} else {
			return false;
		}
	}

	void reserveMe(int id) {
		for (int i = 0; i < DROPOFFLEN; i++) {
			cells[i]->occupy(id);
		}		
	}

	void releaseMe(int id) {
		for (int i = 0; i < DROPOFFLEN; i++) {
			cells[i]->free(id);
		}
	}
};

#endif

