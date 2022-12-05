#include <stdio.h>
#include <stdlib.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <syslog.h>
#include <unistd.h>
#include <errno.h>
#include <stdint.h>
#include "mma8452.h"


#define MMA8452Q_FILE "/dev/i2c-2"
#define MMA8452Q_ADDR 0x1D


char accl_string[50];

size_t populate_accl_data(void *buf, size_t n)
{
    int num_of_bytes = 0;
    num_of_bytes = get_accl_x_y_z();
    if(n < num_of_bytes)
    {
        printf("Buffer full\n");
        return 0;
    }

    int i;
    for(i = 0; i < num_of_bytes; i++)
    {
        *(char *)(buf+i) = *(accl_string+i);
    }

    return i+1;
}

int get_accl_x_y_z()
{
	// Create I2C bus
	int i2c_fd;
    int ret_val = 0;
    int num_of_bytes = 0;

    char *bus = "/dev/i2c-2";
	i2c_fd = open(bus, O_RDWR);
    if(i2c_fd < 0)
        perror("open()");
	// Get I2C device, MMA8452Q I2C address is 0x1D(29)
	ret_val = ioctl(i2c_fd, I2C_SLAVE, MMA8452Q_ADDR);
	if(ret_val < 0)
        perror("ioctl()");
	// Select mode register(0x2A)
	// Standby mode(0x00)
	char config[2] = {0};
	config[0] = 0x2A;
	config[1] = 0x00;
	ret_val = write(i2c_fd, config, 2);
    if(ret_val != 2)
        perror("write()");

	// Select mode register(0x2A)
	// Active mode(0x01)
	config[0] = 0x2A;
	config[1] = 0x01;
	write(i2c_fd, config, 2);
	if(ret_val != 2)
        perror("write()");
	// Select configuration register(0x0E)
	// Set range to +/- 2g(0x00)
	config[0] = 0x0E;
	config[1] = 0x00;
	write(i2c_fd, config, 2);
    if(ret_val != 2)
        perror("write()");
	sleep(0.5);

	// Read 6 bytes of data(0x01)
	// xAccl msb, xAccl lsb, yAccl msb, yAccl lsb, zAccl msb, zAccl lsb
	char reg[1] = {0x00};
	write(i2c_fd, reg, 1);
	char data[7] = {0};
	if(read(i2c_fd, data, 7) != 7)
	{
		printf("Error : Input/Output error \n");
	}
	else
	{
		// Convert the data to 12-bits
		int xAccl = ((data[1] * 256) + data[2]) / 16;
		if(xAccl > 2047)
		{
			xAccl -= 4096;
		}

		int yAccl = ((data[3] * 256) + data[4]) / 16;
		if(yAccl > 2047)
		{
			yAccl -= 4096;
		}

		int zAccl = ((data[5] * 256) + data[6]) / 16;
		if(zAccl > 2047)
		{
			zAccl -= 4096;
		}

		// Output data to screen
		printf("Acceleration in X-Axis : %d \n", xAccl);
		printf("Acceleration in Y-Axis : %d \n", yAccl);
		printf("Acceleration in Z-Axis : %d \n", zAccl);
        num_of_bytes = sprintf(accl_string, "X: %d, Y: %d, Z: %d", xAccl, yAccl, zAccl);
	}

    return num_of_bytes;
}


