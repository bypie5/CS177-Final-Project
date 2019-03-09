#ifndef __DISPATCHER_H__ 
#define __DISPATCHER_H__

class Dispatcher {
private:
	int zoneCnt;

public:
	Dispatcher(int zoneCnt) {
		this->zoneCnt = zoneCnt;
	}

	void dispatchLoop();
};

#endif

