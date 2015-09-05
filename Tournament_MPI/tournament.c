#include "tournament.h"
#include <math.h>

void gtmpi_barrier() {
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
