#include <iostream>
#include <cstdlib>
#include <stdio.h>
#include "cpp.h"
#include <string.h>
#include "constants.h"
#include "roadway.h"

using namespace std;

extern "C" void sim() {
	new Roadway(10, 1);
}

