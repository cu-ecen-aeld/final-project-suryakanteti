SRC := client.c
TARGET = client
#OBJS := $(SRC:.c=.o)

CC=gcc
LDFLAGS= -pthread  -lpthread

all: client

$(TARGET) : $(OBJS)
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(INCLUDES) $(OBJS) $(SRC) -o $(TARGET) $(LDFLAGS)

clean:
	-rm -f *.o $(TARGET) *.elf *.map
