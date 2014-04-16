SOURCES := Process.cpp InputProcess.cpp V4L2LoopbackProcess.cpp
HEADERS := $(SOURCES:.cpp=.hpp) Pipe.hpp
OBJECTS := $(SOURCES:.cpp=.o)
LDFLAGS := `pkg-config --libs opencv` -lboost_thread -lboost_system
CFLAGS := `pkg-config --cflags opencv` -g -O3

passthrough: $(OBJECTS) passthrough.cpp
	g++ -o passthrough passthrough.cpp $(OBJECTS) $(LDFLAGS) $(CFLAGS)

all: $(OBJECTS)
	
%.o: %.cpp $(HEADERS)
	g++ -c -o $@ $< $(CFLAGS)
	
clean:
	rm *.o -f