#include <stdio.h>
#include <mpi.h>
#include <sys/time.h>
#include "tournament.h"

#define NUM_BARRIERS 10

int main(int argc, char **argv)
{
	struct timeval start, end;
	int my_id, num_processes;
	int num_barriers = NUM_BARRIERS;
	long total_time=0;
	long time_passed=0;
	MPI_Init(&argc, &argv);

	MPI_Comm_size(MPI_COMM_WORLD, &num_processes);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_id);

	int loop=0;
	for(loop=0; loop<NUM_BARRIERS; loop++){
		gettimeofday(&start, NULL);
		
		gtmpi_barrier();

		gettimeofday(&end,NULL);
		time_passed = (end.tv_sec-start.tv_sec)*1000000 + (end.tv_usec-start.tv_usec);
		if(time_passed<0)
			num_barriers = num_barriers-1;
		else
			total_time = total_time + time_passed;
		printf("Barrier %d took time %ld\n", loop, time_passed);
	}
	printf("Average time for a tournament barrier %ld\n",(total_time/num_barriers));
	MPI_Finalize();
	return 0;
}
