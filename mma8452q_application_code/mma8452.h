


#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

size_t populate_accl_data(void *buf, size_t n, int i2c_fd);
int get_accl_x_y_z(int i2c_fd);
int open_i2c_port();