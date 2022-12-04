#selecting complier
CROSS_COMPILE=
CC ?=$(CROSS_COMPILE)gcc

#selecting Targer file
TARGET ?= gps

#setting flags
#CFLAGS  ?= -Werror -Wall
#LDFLAGS ?= -pthread -lrt

#selecting all needed source files
SRC_FILES = $(TARGET).c main.c misc.c

all: $(TARGET)
#clean:
#	rm -f *.o writer

$(TARGET): $(TARGET).c main.c misc.c
	$(CC) $(CFLAGS) $(SRC_FILES) -o $(TARGET) $(LDFLAGS)
clean:
	rm -f *.o $(TARGET)