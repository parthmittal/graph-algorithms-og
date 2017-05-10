NVCC=nvcc
OBJECTS=atomic_queue.o

.PHONY: all clean

all : $(OBJECTS)

$(OBJECTS) : %.o : %.cu %.cuh
	$(NVCC) -c $< -o $@

clean :
	rm -f $(OBJECTS)
