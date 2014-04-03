SOURCES := test.cpp ColorFilter.cpp PoolBall.cpp ImageProc.cpp TableState.cpp LogicProc.cpp StreamProc.cpp
LDFLAGS := `pkg-config --libs opencv` -lboost_thread -lboost_system -lm
CFLAGS := `pkg-config --cflags opencv` -g -O3

all: PoolVisor

PoolVisor: $(SOURCES)
	g++ -o PoolVisor $(SOURCES)  $(LDFLAGS) $(CFLAGS)
	
clean:
	rm PoolVisor -f
	rm *.o -f
