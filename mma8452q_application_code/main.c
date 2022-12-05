


#include "mma8452.h"
#include <string.h>


char accl_data[50];

int main()
{
    int fd = open_i2c_port();
    populate_accl_data(accl_data, 50, fd);

    printf("%s\n", accl_data);

    return 0;
}