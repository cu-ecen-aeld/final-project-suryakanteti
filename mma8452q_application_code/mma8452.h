/**
 * @file: mma8452q.h
 * @brief: Contains application code for mma8452q.c 
 * @description: This code uses linux/i2c-dev.h https://www.kernel.org/doc/Documentation/i2c/dev-interface
 *               to perform i2c read write operations. This code sets the sensitivity to +/-2g and reads 
 *               the X, Y, Z acclerometer values
 * @author: Peter Braganza
 * 
*/

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

size_t populate_accl_data(void *buf, size_t n, int i2c_fd);
int get_accl_x_y_z(int i2c_fd);
int open_i2c_port();