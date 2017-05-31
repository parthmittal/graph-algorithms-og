CXX = clang++
CXXFLAGS = -D __TARJAN_LCA__ -D __UNION_BY_RANK__ -D __PATH_COMPRESSION__
override CXXFLAGS += -Wall -std=c++14 -O3

OBJECTS = ear-decomposition-dfs.o graph.o test.o union-find.o lca.o ear-decomposition-bfs.o
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
