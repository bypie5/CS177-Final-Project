#ifndef __CELL_H__
#define __CELL_H__

class Cell {
private:
	facility* pavement;

public:
	Cell() {
		pavement = new facility("cell");
	};

	facility* getPavement() { return pavement; }
};

#endif

