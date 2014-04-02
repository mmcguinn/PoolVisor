SOURCES := test.cpp ColorFilter.cpp PoolBall.cpp ImageProc.cpp TableState.cpp StreamProc.cpp
LDFLAGS := `pkg-config --libs opencv` -lboost_thread -lboost_system
CFLAGS := `pkg-config --cflags opencv` -g

all: PoolVisor

PoolVisor: $(SOURCES)
	g++ -o PoolVisor $(SOURCES)  $(LDFLAGS) $(CFLAGS)
	
clean:
	rm PoolVisor -f
	rm *.o -f