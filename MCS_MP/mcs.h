#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include <omp.h>

#define READY 1
#define NOT_READY 0

typedef struct node {
	int parent_sense;
	int parent_pointer;
	int child_pointers[2];
	int have_child[4];
	int child_not_ready[4];
}node;

struct node* shared_nodes;

void gtmp_init(int barrierThreads);

void gtmp_barrier();
