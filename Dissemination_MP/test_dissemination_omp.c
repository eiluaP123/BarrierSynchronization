#include <omp.h>
#include <stdio.h>
#include "dissemination_omp.h"
#include <sys/time.h>

#define NUM_BARRIERS 10

int main(int argc, char** argv){
	int num_threads;
	long total_time=0;
	int num_barriers = NUM_BARRIERS;
	struct timeval start, end;

	  /* Preventing runtime from adjusting the number of threads. */
	  omp_set_dynamic(0);

	  /* Checking if it worked. */
	  if (omp_get_dynamic())
		printf("Warning: dynamic adjustment of threads has been set\n");

	/* For if the inputs are given from the command prompt */
	if(argc>1){
		num_threads = atoi(argv[1]);
	}
	else {
		num_threads = 4;
	}

	/* Setting number of threads */
	omp_set_num_threads(num_threads);

	gtmp_init(num_threads);
	
	/* Code in the block following the #pragma directive is run in parallel */
	#pragma omp parallel 
	   {

		 /*Some code before barrier ..... nt)*/
		int thread_id = omp_get_thread_num();
		int loop;
		for(loop=0;loop<NUM_BARRIERS; loop++)
		{
			gettimeofday(&start,NULL);
			gtmp_barrier();
			gettimeofday(&end,NULL);
			long time_passed = (end.tv_sec - start.tv_sec)* 1000000 + (end.tv_usec - start.tv_usec);
			printf("exiting thread %d barrier %d took time %ld\n", thread_id, loop, time_passed);
			if (time_passed<0)
				num_barriers = num_barriers-1;
			else
				total_time = total_time + time_passed;
		}
	}
	printf("The average time taken by all threads for %d barriers is %ld\n",NUM_BARRIERS, (total_time/num_barriers));

    return(0);
}
