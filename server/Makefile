#selecting complier
CROSS_COMPILE=
CC ?=$(CROSS_COMPILE)gcc

#selecting Targer file
TARGET ?= server

#selecting all needed source files
SRC_FILES = server.c ../gps_application_code/gps.c ../gps_application_code/misc.c ../mma8452q_application_code/mma8452q.c

all: $(TARGET)
#clean:
#	rm -f *.o writer

$(TARGET): $(TARGET).c
	$(CC) $(CFLAGS) $(SRC_FILES) -o $(TARGET) $(LDFLAGS)
clean:
	rm -f *.o $(TARGET)