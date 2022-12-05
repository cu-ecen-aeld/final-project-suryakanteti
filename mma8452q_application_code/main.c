


#include "mma8452.h"
#include <string.h>


char accl_data[50];

int main()
{
    populate_accl_data(accl_data, 50);

    printf("%s", accl_data);

    return 0;
}