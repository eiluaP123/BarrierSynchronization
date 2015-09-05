#PBS -q cs6210
#PBS -l nodes=4:sixcore
#PBS -l walltime=00:05:00
#PBS -N tournament_mpi
MPI_MCA_mpi_yield_when_idle=0
cd $PBS_O_WORKDIR
for i in 2 4 8; do
	make clean;
	make;
	echo "Starting job"
	/opt/openmpi-1.4.3-gcc44/bin/mpirun -np $i ./test_tournament_mpi
	echo "ending job"
done
