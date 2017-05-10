NVCC=nvcc
OBJS=atomic_queue.o

$(OBJECTS) : %.o : %.cu %.cuh
	$(NVCC) -c $< -o $@
