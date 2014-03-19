all: PoolVisor

PoolVisor: test.cpp
	g++ -o PoolVisor test.cpp  `pkg-config --cflags --libs opencv`
	
clean:
	rm PoolVisor -f
	rm *.o -f