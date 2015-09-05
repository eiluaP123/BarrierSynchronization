#include "dissemination_mpi.h"
#include <sys/time.h>

#define NUM_BARRIERS 10

int main(int argc, char **argv)
{
	struct timeval start,end;
	int my_id, num_processes;
	long total_time=0;
	long time_passed=0;
	int num_barriers = NUM_BARRIERS;
	MPI_Init(&argc, &argv);

	MPI_Comm_size(MPI_COMM_WORLD, &num_processes);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_id);

	printf("Number of Processes %d\n",num_processes);

	int loop=0;
	for(loop=0;loop<NUM_BARRIERS; loop++){
		gettimeofday(&start,NULL);
		gtmpi_barrier();
		gettimeofday(&end,NULL);
		time_passed = (end.tv_sec-start.tv_sec)*1000000 + (end.tv_usec-start.tv_usec);
		if(time_passed<0)
			num_barriers=num_barriers-1;
		else
			total_time = total_time + time_passed;
		printf("Goodbye World1 from process %d of %d time taken %ld\n", my_id, num_processes, time_passed);
	}
	printf("Average time taken by %d number of barriers is %ld\n",NUM_BARRIERS, (total_time/num_barriers));
	MPI_Finalize();
	return 0;
}
