CXX = clang++
CXXFLAGS = -Wall -std=c++14 -O3

OBJECTS = ear-decomposition-dfs.o graph.o
EXEC = ear-decomposition

.PHONY: clean all

all : $(EXEC)

$(OBJECTS) : %.o : %.cpp %.hpp
	$(CXX) -c $(CXXFLAGS) $< -o $@

$(EXEC) : % : $(OBJECTS)
	$(CXX) $(CXXFLAGS) $^ -o $@

clean:
	rm -f $(OBJECTS) $(EXEC)
