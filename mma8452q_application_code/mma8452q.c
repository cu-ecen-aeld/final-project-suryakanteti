


#include <stdio.h>
#include <stdlib.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <syslog.h>
#include <unistd.h>
#include <errno.h>
#include <stdint.h>


#define MMA8452Q_FILE "/dev/i2c-1"
#define MMA8452Q_ADDR 0x1D
#define WHO_AM_I 0x0D

int main()
{

    int ret_val = 0;
    uint16_t who_am_i_reg = WHO_AM_I;
    char buf[2];
    uint16_t reg_val;


    int i2c_fd = open(MMA8452Q_FILE, O_RDWR);
    if(i2c_fd < 0)
        perror("open()");

    ret_val = ioctl(i2c_fd, I2C_SLAVE, MMA8452Q_ADDR);
    if(ret_val < 0)
    perror("ioctl()");

    ret_val = write(i2c_fd, &who_am_i_reg, sizeof(who_am_i_reg));
    if(ret_val != sizeof(who_am_i_reg))
        perror("write()");

    ret_val = read(i2c_fd, buf, sizeof(who_am_i_reg));
    if(ret_val != sizeof(who_am_i_reg))
        perror("read()");
    
    reg_val = buf[1] | (buf[0] << 8);
    printf("Who am I reg: 0x%02x", reg_val);

}