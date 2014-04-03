SOURCES := ColorFilter.cpp PoolBall.cpp ImageProc.cpp TableState.cpp LogicProc.cpp StreamProc.cpp
HEADERS := $(SOURCES:.cpp=.hpp)
OBJECTS := $(SOURCES:.cpp=.o)
LDFLAGS := `pkg-config --libs opencv` -lboost_thread -lboost_system
CFLAGS := `pkg-config --cflags opencv` -g -O3

all: PoolVisor

PoolVisor: test.cpp $(OBJECTS)
	g++ -o PoolVisor test.cpp $(OBJECTS)  $(LDFLAGS) $(CFLAGS)
	
%.o: %.cpp $(HEADERS)
	g++ -c -o $@ $< $(CFLAGS)
	
clean:
	rm PoolVisor -f
	rm *.o -f