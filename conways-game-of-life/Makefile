XBGI = /usr/lib/libXbgi.a
OPENMPI1 = -I/usr/lib/openmpi/include
OPENMPI2 = -I/usr/lib/openmpi/include/openmpi
OPENMPI3 = -L/usr/lib/openmpi/lib -lmpi -lopen-rte -lopen-pal -ldl -Wl,--export-dynamic -lnsl -lutil -lm -ldl

CC = gcc
CFLAGS = -O2
AR = ar
ARFLAGS = rc
LIBS = -lX11 -lm
PTHREAD = -pthread

PROGRAMS = conways.out mpi_conways.out

all: $(PROGRAMS)

conways.out: conways.c $(XBGI)
	$(CC) $(CFLAGS) -o conways.out conways.c $(XBGI) $(LIBS)

mpi_conways.out: mpi_conways.c $(XBGI)
	$(CC) $(CFLAGS) -o mpi_conways.out mpi_conways.c $(XBGI) $(LIBS) $(OPENMPI1) $(OPENMPI2) $(OPENMPI3) $(PTHREAD)

clean:
	rm -f $(PROGRAMS)

run_mpi_conways:
	mpirun -np 1 ./mpi_conways.out
