#include "mcs.h"

int num_threads;

void gtmp_init(int nThreads){
//Make tree
//Initialize values
num_threads = nThreads;
shared_nodes = (node *) malloc (num_threads * sizeof(node));
int i, j;

for ( i=0; i<num_threads; i++){
	for( j=1; j<=4; j++){
		if(4*i+j < num_threads){
			shared_nodes[i].have_child[j-1] = 4*i+j;
			shared_nodes[i].child_not_ready[j-1] = 0;
		}
		else {
			shared_nodes[i].have_child[j-1] = -1;
			shared_nodes[i].child_not_ready[j-1] = -1;
		}
	}
	
	//rootnode
	if(i==0) {
		shared_nodes[i].parent_pointer = -1;
		shared_nodes[i].parent_sense = 1;
	}
	else {
		shared_nodes[i].parent_sense = 0;
		shared_nodes[i].parent_pointer = (i-1)/4;
	}
	
	if((2*i+1) > num_threads)
	shared_nodes[i].child_pointers[0] = -1;
	else
	shared_nodes[i].child_pointers[0] = 2*i+1;
	
	if((2*i+2) > num_threads)
	shared_nodes[i].child_pointers[1] = -1;
	else
	shared_nodes[i].child_pointers[1] = 2*i+2;
}
}

void gtmp_barrier(){
	int flag=NOT_READY;
	int k, t, l;
	int tid = omp_get_thread_num();
	
	while ( flag == NOT_READY){
		int count=0;
		for ( k=0; k<4; k++){
			if (shared_nodes[tid].child_not_ready[k] == READY)
				count++;
			else if(shared_nodes[tid].have_child[k] == -1){
				shared_nodes[shared_nodes[tid].parent_pointer].child_not_ready[((tid-1)%4)] == READY;
				count++;
			}
			else {
				break;
			}
		}
		if (count==4) {
			flag = READY;
			for (t=0; t<4; t++){
				if(shared_nodes[tid].have_child[t] != -1)
					shared_nodes[tid].child_not_ready[t] = NOT_READY;
			}
		}
	}
	//if root, signal. Else set to ready.
	if(shared_nodes[tid].parent_pointer !=-1) {
		shared_nodes[shared_nodes[tid].parent_pointer].child_not_ready[((tid-1)%4)] = READY;
	}
	//printf("while flag exiting\n");
	//if not root, wait for parent to be signal
	while (shared_nodes[tid].parent_sense==NOT_READY);
	//printf("exiting parent sense loop\n");

	//RESET FLAG
	if (shared_nodes[tid].parent_pointer != -1)
		shared_nodes[tid].parent_sense=NOT_READY;

	
	//When signalled, children get READY
	for ( l=0; l<2; l++){
		if(shared_nodes[tid].child_pointers[l] != -1)
			shared_nodes[shared_nodes[tid].child_pointers[l]].parent_sense = READY;
	}
	
}




	
