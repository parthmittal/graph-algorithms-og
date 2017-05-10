#include "atomic_queue.cuh"

__device__ void atomic_push(queue_t Q, int element)
{
	int idx = atomicAdd(Q.head, 1);
	Q.data[idx] = element;
}
