CXX = clang++
CXXFLAGS = -Wall -std=c++14 -O3

OBJECTS = ear-decomposition-dfs.o graph.o test.o
EXEC = test

.PHONY: clean all

all : $(EXEC)

.SECONDEXPANSION:
$(OBJECTS) : %.o : %.cpp $$(wildcard $$*.hpp)
	$(CXX) -c $(CXXFLAGS) $< -o $@

$(EXEC) : % : $(OBJECTS)
	$(CXX) $(CXXFLAGS) $^ -o $@

clean:
	rm -f $(OBJECTS) $(EXEC)
