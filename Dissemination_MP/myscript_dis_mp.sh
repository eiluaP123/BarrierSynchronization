#PBS -q cs6210
#PBS -l nodes=1:fourcore
#PBS -l walltime=00:05:00
#PBS -N dis_mp
MPI_MCA_mpi_yield_when_idle=0
cd $PBS_O_WORKDIR
for i in 2 4 8; do
	make clean;
	make;
	echo "Starting job"
	./test_dissemination_omp $i
	echo "ending job"
done

