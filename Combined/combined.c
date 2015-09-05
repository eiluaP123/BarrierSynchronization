#include <omp.h>
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <malloc.h>
#include <sys/time.h>

#define NUM_THREADS 4
#define NUM_BARRIERS 10

int num_threads;
int rounds;

void tournament_barrier(){
	int round=1;
	int num_procs;
	MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
	int num_rounds = ceil(log2(num_procs));
	
	int pid,send_to,recv_from;
	MPI_Comm_rank(MPI_COMM_WORLD, &pid);

	int sense = 0;
	int private_sense = 0;
	
	//Signal
	while(round<=num_rounds){
		//1. loser
		if(round>0 && ((pid%(int)pow(2,round)) == (int)pow(2,round-1)))	
		{
			send_to = pid - (int)pow(2,round-1);
			MPI_Send(&private_sense, 1, MPI_INT, send_to, 0, MPI_COMM_WORLD);
			recv_from = send_to;
			MPI_Recv(&sense, 1, MPI_INT, recv_from, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			break;
		}

		//2.Winner
		else if(round>0 && (pid%(int)pow(2,round) == 0) && (pid+(int)pow(2,round-1))<num_procs && (int)pow(2,round)<num_procs){
			recv_from = pid + (int)pow(2,round-1);
			MPI_Recv(&private_sense, 1, MPI_INT, recv_from, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		}		

		//3.Bye
		else if(round>0 && (pid%(int)pow(2,round)==0) && (pid+(int)pow(2,round-1)>=num_procs)) {

		}	

		//4. Champion
		else if(round>0 && pid==0 && (int)pow(2,round)>=num_procs) {
			recv_from = pid + (int)pow(2,round-1);
			MPI_Recv(&private_sense, 1, MPI_INT, recv_from, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			sense = 1;
			send_to = recv_from;
			MPI_Send(&sense, 1, MPI_INT, send_to, 0, MPI_COMM_WORLD);
		}
		//5.dropout
		else {
			printf("Impossible\n");
		}
	round=round+1;
	}

	//wake up
	while(round>1) {
		round = round-1;
		//1. loser
		if(round>0 && (pid%(int)pow(2,round) == (int)pow(2,round-1)))	
		{
			printf("Impossible to come here\n");
		}

		//2. Winner
		else if(round>0 && (pid%(int)pow(2,round)) == 0 && (pid+(int)pow(2,round-1))<num_procs && ((int)pow(2,round))<num_procs){
			send_to = pid + (int)pow(2,(round-1));
			MPI_Send(&sense, 1, MPI_INT, send_to, 0, MPI_COMM_WORLD);
		}
		
		//3.Bye 
		else if(round>0 && (pid%(int)pow(2,round)==0) && pid+(int)pow(2,round-1)>=num_procs) {

		}

		//4. Champion
		else if(round>0 && pid==0 && (int)pow(2,round)>=num_procs) {

		}
		//5. dropout
		else {
			break;
		}
	}
}

int main(int argc, char **argv)
{
    struct timeval start, end, startAll;
    gettimeofday(&startAll,NULL);
    printf("Starting time for all: %ld\n",startAll.tv_usec);

	MPI_Init(&argc, &argv);
	long total_time = 0;
    long time_passed = 0;
	int num_barriers = NUM_BARRIERS;

	int proc_id, num_processes, num_threads;
	MPI_Comm_size(MPI_COMM_WORLD, &num_processes);
	MPI_Comm_rank(MPI_COMM_WORLD, &proc_id);

	if(argc >1) {
		num_threads = atoi(argv[1]);
	}
	else {
		num_threads = NUM_THREADS;
	}

	omp_set_num_threads(num_threads);
	rounds = ceil(log(num_threads)/log(2));
	int i,j;
	int msg_status[(int)ceil(log(NUM_THREADS)/log(2))][NUM_THREADS];
	for (i = 0; i < rounds; i++){
		for(j=0; j<num_threads; j++)
			msg_status[i][j] = 0;
	}
	printf("Running for %d num of processes at %d time\n",num_processes, (int)(startAll.tv_usec));
	rounds = ceil(log(num_threads)/log(2));

	#pragma omp parallel shared(msg_status)
	{ 
        gettimeofday(&start,NULL);

		int tid = omp_get_thread_num();
		int b=0;
		for(b=0; b<NUM_BARRIERS;b++)
		{
			int k,tid, send_to;
			tid = omp_get_thread_num();

			for(k=0; k<rounds; k++){
				send_to = (tid+(int)pow(2,k))%num_threads;
				while(msg_status[k][send_to]==1);
				msg_status[k][send_to] = 1;
				while(msg_status[k][tid] == 0);
				msg_status[k][tid] = 0;
			}
			if(tid==0){
				tournament_barrier();
			}
			tid = omp_get_thread_num();
			for(k=0; k<rounds; k++){
				send_to = (tid+(int)pow(2,k))%num_threads;
				while(msg_status[k][send_to]==1);
				msg_status[k][send_to] = 1;
				while(msg_status[k][tid] == 0);
				msg_status[k][tid] = 0;
			}
				printf("Process %d Thread %d barr:%d\n",proc_id,tid,b);

		}
		gettimeofday(&end,NULL);
		time_passed = (end.tv_sec-start.tv_sec)*1000000 + (end.tv_usec-start.tv_usec);
		if (time_passed<0)
			num_barriers = num_barriers-1;
		else
			total_time = total_time + time_passed;
		printf("Ending time for process%d thread %d barr %d : %ld\n",proc_id, tid, b, time_passed);

	}
	printf("Goodbye\n");
	printf("Average time for barrier: %ld\n",(total_time/num_barriers));
	MPI_Finalize();
	return 0;
}
