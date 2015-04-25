XBGI = /usr/lib/libXbgi.a

CC = gcc
CFLAGS = -O2
AR = ar
ARFLAGS = rc
LIBS = -lX11 -lm

PROGRAMS = conways.out

all: $(PROGRAMS)

conways.out: conways.c $(XBGI)
	$(CC) $(CFLAGS) -o conways.out conways.c $(XBGI) $(LIBS)

clean:
	rm -f $(PROGRAMS)
