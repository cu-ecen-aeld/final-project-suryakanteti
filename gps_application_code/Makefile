# Default compiler
CC = gcc

all: gps

# gps application
gps: clean main.o gps.o misc.o
	$(CROSS_COMPILE)$(CC) main.o gps.o misc.o -o gps

# gps files	
main.o: gps.c
	$(CROSS_COMPILE)$(CC) -c main.c
	
gps.o: gps.c
	$(CROSS_COMPILE)$(CC) -c gps.c
	
misc.o: gps.c
	$(CROSS_COMPILE)$(CC) -c misc.c
	
# Clean all binaries
clean:
	rm -f *.o gps