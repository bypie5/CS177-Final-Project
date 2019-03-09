CC = g++
CSIMFLAGS_1 = -DCPP -DGPP -I/usr/csshare/pkgs/csim_cpp-19.0/lib -m32
SOURCES = $(wildcard ./src/*.cpp)
CSIMFLAGS_2 = /usr/csshare/pkgs/csim_cpp-19.0/lib/csim.cpp.a -lm 
OBJECTS = $(SOURCES:./src/.cpp=./src/.o)

all:
	mkdir -p ./bin
	$(CC) $(CSIMFLAGS_1) $(OBJECTS) $(CSIMFLAGS_2) -o ./bin/dropoff_sim

%.o: %.cpp
	$(CC) -c $(CC_FLAGS) $< -o $@

debug:
	mkdir -p ./bin
	$(CC) $(CSIMFLAGS_1) -DDEBUG $(OBJECTS) $(CSIMFLAGS_2) -o ./bin/dropoff_sim

clean:
	rm -rf ./bin
