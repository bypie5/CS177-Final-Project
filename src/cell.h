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
	void occupy() { pavement->reserve(); };
	void free() { pavement->release(); };
};

#endif

