#include "dissemination_omp.h"

int num_threads;
int rounds;
int** local_sense;

void gtmp_init(int nThreads){
	num_threads = nThreads;
	rounds = ceil(log(num_threads)/log(2));
	int i;

	local_sense = (int**) malloc(rounds * sizeof(int*));
	for (i = 0; i < rounds; i++){
		local_sense[i] = (int*) calloc(num_threads, sizeof(int));
	}
}

void gtmp_barrier(){
	int k,i, send_to;
	i = omp_get_thread_num();

	for(k=0; k<rounds; k++){
		send_to = (i+(int)pow(2,k))%num_threads;
		while(local_sense[k][send_to]==1);
		local_sense[k][send_to] = 1;
		while(local_sense[k][i] == 0);
		local_sense[k][i] = 0;
	}	
}




	
