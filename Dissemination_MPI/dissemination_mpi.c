#include "dissemination_mpi.h"

void gtmpi_barrier(){
	int num_procs;
	int rounds;
	MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
	rounds = ceil(log(num_procs)/log(2));
	int k, send_to, recv_from, sense, pid, recv_sense;
	sense = 1;
	MPI_Comm_rank(MPI_COMM_WORLD, &pid);

	for(k=0; k<rounds; k++){
		send_to = (pid+(int)pow(2,k))%num_procs;
		MPI_Send(&sense, 1, MPI_INT, send_to, 0, MPI_COMM_WORLD );
		
		recv_from = (pid - (int)pow(2,k));
		if (recv_from < 0)
			recv_from = recv_from + num_procs;
		MPI_Recv(&recv_sense, 1, MPI_INT, recv_from, 0, MPI_COMM_WORLD , MPI_STATUS_IGNORE);		
	}	
	//reset
	sense = !sense;
}




	
