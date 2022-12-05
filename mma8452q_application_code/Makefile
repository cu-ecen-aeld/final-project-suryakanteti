#selecting complier
CROSS_COMPILE=
CC ?=$(CROSS_COMPILE)gcc

#selecting Targer file
TARGET ?= mma8452q

#setting flags
#CFLAGS  ?= -Werror -Wall
#LDFLAGS ?= -pthread -lrt

#selecting all needed source files
SRC_FILES = $(TARGET).c main.c

all: $(TARGET)
#clean:
#	rm -f *.o writer

$(TARGET): $(TARGET).c
	$(CC) $(CFLAGS) $(SRC_FILES) -o $(TARGET) $(LDFLAGS)
clean:
	rm -f *.o $(TARGET)
