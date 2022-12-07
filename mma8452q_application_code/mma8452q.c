/**
 * @file: mma8452q.c
 * @brief: Contains application code for mma8452q.c 
 * @description: This code uses linux/i2c-dev.h https://www.kernel.org/doc/Documentation/i2c/dev-interface
 *               to perform i2c read write operations. This code sets the sensitivity to +/-2g and reads 
 *               the X, Y, Z acclerometer values
 * @author: Peter Braganza
 * 
*/


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

/**
 * @brief: populates a buffer with accelerometer data
 * @param buf: buffer in which to store data
 * @param n: number of available bytes left in buffer
 * @param i2c_fd: file descriptor for the opened i2c device port
 * @return: number of bytes written into the buffer
*/
size_t populate_accl_data(void *buf, size_t n, int i2c_fd)
{
    int num_of_bytes = 0;
    num_of_bytes = get_accl_x_y_z(i2c_fd);
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

    return i;
}

/**
 * @brief: opens the i2c device port and returns file descriptor
 * @return: file descriptor of the opened i2c device port
*/
int open_i2c_port()
{
    int i2c_fd;
    int ret_val = 0;
    char *bus = MMA8452Q_FILE;
	i2c_fd = open(bus, O_RDWR);
    if(i2c_fd < 0)
        perror("open()");
	// Get I2C device, MMA8452Q I2C address is 0x1D(29)
	ret_val = ioctl(i2c_fd, I2C_SLAVE, MMA8452Q_ADDR);
	if(ret_val < 0)
        perror("ioctl()");

    return i2c_fd;
}


/**
 * @brief initialized mma8452q and gets accelerometer data
 * @return: number of bytes written into the buffer
*/
int get_accl_x_y_z(int i2c_fd)
{

    int ret_val = 0;
    int num_of_bytes = 0;

	// Select mode register(0x2A)
	// Standby mode(0x00)
	char config[2] = {0};
	config[0] = 0x2A;
	config[1] = 0x00;
	ret_val = write(i2c_fd, config, 2);
    if(ret_val != 2)
	{
		printf("partial write, num of bytes written: %d", ret_val);
	}
        

	// Select mode register(0x2A)
	// Active mode(0x01)
	config[0] = 0x2A;
	config[1] = 0x01;
	write(i2c_fd, config, 2);
	if(ret_val != 2)
	{
		printf("partial write, num of bytes written: %d", ret_val);
	}

	// Select configuration register(0x0E)
	// Set range to +/- 2g(0x00)
	config[0] = 0x0E;
	config[1] = 0x00;
	write(i2c_fd, config, 2);
    if(ret_val != 2)
	{
		printf("partial write, num of bytes written: %d", ret_val);
	}

	// Read 6 bytes of data(0x01)
	// xAccl msb, xAccl lsb, yAccl msb, yAccl lsb, zAccl msb, zAccl lsb
	char reg[1] = {0x00};
	write(i2c_fd, reg, 1);
	char data[7] = {0};
	if(read(i2c_fd, data, 7) != 7)
	{
		printf("partial read, num of bytes read: %d", ret_val);
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
		// printf("Acceleration in X-Axis : %d \n", xAccl);
		// printf("Acceleration in Y-Axis : %d \n", yAccl);
		// printf("Acceleration in Z-Axis : %d \n", zAccl);

		//storing intergeters to buffer
        num_of_bytes = sprintf(accl_string, "%d %d %d", xAccl, yAccl, zAccl);
	}

    return num_of_bytes;

}


