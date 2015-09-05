OMPFLAGS = -fopenmp
OMPLIBS = -lgomp

CC = gcc -w
CPPFLAGS = -g -Wall $(OMPFLAGS)
LDFLAGS = -g -Wall 
LDLIBS = $(OMPLIBS)

MPICC = /opt/openmpi-1.4.3-gcc44/bin/mpicc
MPICH = /opt/openmpi-1.4.3-gcc44
CFLAGS = -I$(MPICH)/include

all: test_mcs
	
test_mcs: test_mcs.o mcs.o
	$(CC) -o $@ $(LDFLAGS) $^ $(LDLIBS) -lm
	
clean:
	rm -f mcs_mp.* mcs.o test_mcs.o
