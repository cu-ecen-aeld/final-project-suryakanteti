SRC1 := client.c
SRC2 :=  parser.c
TARGET = client
#OBJS := $(SRC:.c=.o)

CC=gcc
LDFLAGS= -pthread  -lpthread

all: client

$(TARGET) : $(OBJS)
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(INCLUDES) $(OBJS) $(SRC1) $(SRC2) -o $(TARGET) $(LDFLAGS)

clean:
	-rm -f *.o $(TARGET) *.elf *.map
