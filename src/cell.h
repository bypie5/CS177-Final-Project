#ifndef __CELL_H__
#define __CELL_H__

class Cell {
private:
	facility* pavement;

public:
	Cell() {
		pavement = new facility("cell");
	};

public:
	facility* getPavement() { return pavement; };
	bool isBusy() {
		return pavement->num_busy() > 0 ? true : false;
	};
	void occupy() { pavement->reserve(); };
	void free() { pavement->release(); };
};

#endif

