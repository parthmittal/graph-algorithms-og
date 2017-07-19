CXX = g++
CXXFLAGS = -D __BRANDES__
override CXXFLAGS += -Wall -std=c++14 -O3 -fopenmp

OBJECTS = bwc_brandes.o graph.o test.o
EXEC = test

.PHONY: clean all

all : $(EXEC)

#This is required because some .cpp files
#don't have corresponding headers.
.SECONDEXPANSION:
$(OBJECTS) : %.o : %.cpp $$(wildcard $$*.hpp)
	$(CXX) -c $(CXXFLAGS) $< -o $@

$(EXEC) : % : $(OBJECTS)
	$(CXX) $(CXXFLAGS) $^ -o $@

clean:
	rm -f $(OBJECTS) $(EXEC)
