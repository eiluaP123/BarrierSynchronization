OMPFLAGS = -fopenmp
OMPLIBS = -lgomp

CC = gcc
CPPFLAGS = -g -Wall
LDFLAGS = -g -Wall 
LDLIBS = $(OMPLIBS)

MPICC = /opt/openmpi-1.4.3-gcc44/bin/mpicc
MPICH = /opt/openmpi-1.4.3-gcc44
CFLAGS = -I$(MPICH)/include

all: test_tournament_mpi
	
test_tournament_mpi: test_tournament_mpi.o tournament.o
	$(MPICC) -o $@ $(LDFLAGS) $^ $(LDLIBS) -lm
	
clean:
	rm -f tournament_mpi.* test_tournament_mpi.o tournament.o
