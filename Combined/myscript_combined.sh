#PBS -q cs6210
#PBS -l nodes=4:sixcore
#PBS -l walltime=00:05:00
#PBS -N test_combined
cd $PBS_O_WORKDIR
MPI_MCA_mpi_yield_when_idle=0
for i in 2 4 8; do
	for j in 2 4 8; do
		make clean;
		make;
		echo "Starting job"
		/opt/openmpi-1.4.3-gcc44/bin/mpirun -np $i ./combined $j
		echo "ending job"
	done;
done

