SOURCES := Process.cpp
HEADERS := $(SOURCES:.cpp=.hpp)
OBJECTS := $(SOURCES:.cpp=.o) Pipe.hpp
LDFLAGS := `pkg-config --libs opencv` -lboost_thread -lboost_system
CFLAGS := `pkg-config --cflags opencv` -g -O3

all: $(OBJECTS)
	
%.o: %.cpp $(HEADERS)
	g++ -c -o $@ $< $(CFLAGS)
	
clean:
	rm *.o -f