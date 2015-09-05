#include <omp.h>
#include <stdio.h>
#include "mcs.h"

#define NUM_BARRIERS 10
int main(int argc, char** argv){
int num_threads;
struct timeval start, end1, end2;
long time_passed=0;
long total_time = 0;
int num_barriers=NUM_BARRIERS;  
/* Prevents runtime from adjusting the number of threads. */
  omp_set_dynamic(0);

  /* Making sure that it worked. */
  if (omp_get_dynamic())
    printf("Warning: dynamic adjustment of threads has been set\n");


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
int loop;
for(loop=0; loop<NUM_BARRIERS; loop++){
	int thread_id = omp_get_thread_num();
    gettimeofday(&start, NULL);

	gtmp_barrier();
	
	gettimeofday(&end1,NULL);
	time_passed = (end1.tv_sec - start.tv_sec)*1000000 + (end1.tv_usec - start.tv_usec);
	if (time_passed<0)
		num_barriers = num_barriers-1;
	else
		total_time = total_time + time_passed;
	printf("exiting! Barrier %d took time %ld\n", loop, time_passed);
	}
   }
printf("Average time taken by each barrier %ld\n", total_time/num_barriers);
   return(0);
}
