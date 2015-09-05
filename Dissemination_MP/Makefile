OMPFLAGS = -fopenmp
OMPLIBS = -lgomp

CC = gcc -w
CPPFLAGS = -g -Wall $(OMPFLAGS)
LDFLAGS = -g -Wall 
LDLIBS = $(OMPLIBS)

MPICC = /opt/openmpi-1.4.3-gcc44/bin/mpicc
MPICH = /opt/openmpi-1.4.3-gcc44
CFLAGS = -I$(MPICH)/include

all: test_dissemination_omp
	
test_dissemination_omp: test_dissemination_omp.o dissemination_omp.o
	$(CC) -o $@ $(LDFLAGS) $^ $(LDLIBS) -lm
	
clean:
	rm -f dis_mp.* test_dissemination_omp.o dissemination_omp.o
