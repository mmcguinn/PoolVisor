

SOURCES := test.cpp ColorFilter.cpp PoolBall.cpp

all: PoolVisor

PoolVisor: $(SOURCES)
	g++ -o PoolVisor $(SOURCES)  `pkg-config --cflags --libs opencv`
	
clean:
	rm PoolVisor -f
	rm *.o -f