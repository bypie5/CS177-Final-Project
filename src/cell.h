#ifndef __CELL_H__
#define __CELL_H__

class Cell {
private:
	facility* pavement;
	int owner;

public:
	Cell() {
		pavement = new facility("cell");
		owner = -1;
	};

public:
	facility* getPavement() { return pavement; };
	bool isBusy() {
		return pavement->num_busy() > 0 ? true : false;
	};
	void occupy(int requester_id) { 
		pavement->reserve(); 
		owner = requester_id;
	};
	void free(int requester_id) {
		if (requester_id == owner) {
			pavement->release(); 
			owner = -1;
		}
	};
};

#endif

