# Default compiler
CC = gcc

all: mma8452q

# mma8452q application
mma8452q: clean mma8452q.o
	$(CROSS_COMPILE)$(CC) mma8452q.o -o mma8452q

# mma8452q.o files	
mma8452q.o: mma8452q.c
	$(CROSS_COMPILE)$(CC) -c mma8452q.c
	
# Clean all binaries
clean:
	rm -f *.o mma8452q