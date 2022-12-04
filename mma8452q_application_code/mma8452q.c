#include <stdio.h>
#include <stdlib.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <syslog.h>
#include <unistd.h>
#include <errno.h>
#include <stdint.h>


#define MMA8452Q_FILE "/dev/i2c-2"
#define MMA8452Q_ADDR 0x1D



void main() 
{
	// Create I2C bus
	int i2c_fd;
    int ret_val = 0;

	i2c_fd = open(MMA8452Q_FILE, O_RDWR);
    if(i2c_fd < 0)
        perror("open()");
	// Get I2C device, MMA8452Q I2C address is 0x1C(28)
	ret_val = ioctl(i2c_fd, I2C_SLAVE, 0x1C);
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

	// Read 7 bytes of data(0x00)
	// staus, xAccl msb, xAccl lsb, yAccl msb, yAccl lsb, zAccl msb, zAccl lsb
	char reg[1] = {0x01};
	write(i2c_fd, reg, 1);
	char data[6] = {0};
	if(read(i2c_fd, data, 6) != 6)
	{
		printf("Error : Input/Output error \n");
	}
	else
	{
		// Convert the data to 12-bits
		int xAccl = ((data[0] * 256) + data[1]) / 16;
		if(xAccl > 2047)
		{
			xAccl -= 4096;
		}

		int yAccl = ((data[2] * 256) + data[3]) / 16;
		if(yAccl > 2047)
		{
			yAccl -= 4096;
		}

		int zAccl = ((data[4] * 256) + data[5]) / 16;
		if(zAccl > 2047)
		{
			zAccl -= 4096;
		}

		// Output data to screen
		printf("Acceleration in X-Axis : %d \n", xAccl);
		printf("Acceleration in Y-Axis : %d \n", yAccl);
		printf("Acceleration in Z-Axis : %d \n", zAccl);
	}
}