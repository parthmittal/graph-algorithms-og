typedef struct __queue_t {
	int *data;
	int *head;
} queue_t;

__device__ void atomic_push(int element);
