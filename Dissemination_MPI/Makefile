OMPFLAGS = -fopenmp
OMPLIBS = -lgomp

CC = gcc
CPPFLAGS = -g -Wall
LDFLAGS = -g -Wall 
LDLIBS = $(OMPLIBS)

MPICC = /opt/openmpi-1.4.3-gcc44/bin/mpicc
MPICH = /opt/openmpi-1.4.3-gcc44
CFLAGS = -I$(MPICH)/include

all: test_dissemination_mpi
	
test_dissemination_mpi: test_dissemination_mpi.o dissemination_mpi.o
	$(MPICC) -o $@ $(LDFLAGS) $^ $(LDLIBS) -lm
	
clean:
	rm -f dis_mpi.* *.o
