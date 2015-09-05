OMPFLAGS = -fopenmp
OMPLIBS = -lgomp

CC = gcc
CPPFLAGS = -g -Wall $(OMPFLAGS)
LDFLAGS = -g -Wall 
LDLIBS = $(OMPLIBS)

MPICC = /opt/openmpi-1.4.3-gcc44/bin/mpicc
MPICH = /opt/openmpi-1.4.3-gcc44
CFLAGS = -I$(MPICH)/include

all: combined
	
combined: combined.o
	$(MPICC) -o $@ $(LDFLAGS) $^ $(LDLIBS) -lm
	
clean:
	rm -f combined.o test_combined.*
