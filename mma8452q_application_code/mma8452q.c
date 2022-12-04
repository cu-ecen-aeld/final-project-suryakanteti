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
#define WHO_AM_I 0x0D
#define CTRL_REG 0x2A

uint8_t ctrl_reg[2] = {CTRL_REG, 0x01};
uint8_t out_x_msb = 0x01;


int main()
{

    int ret_val = 0;
    uint8_t who_am_i_reg = 0x0D;
    uint8_t buf[2];
    uint8_t reg_val;
    uint8_t accl[6];

    int16_t acc_x, acc_y, acc_z;     //+/-2g


    int i2c_fd = open(MMA8452Q_FILE, O_RDWR);
    if(i2c_fd < 0)
        perror("open()");

    ret_val = ioctl(i2c_fd, I2C_SLAVE, MMA8452Q_ADDR);
    if(ret_val < 0)
    perror("ioctl()");

    ret_val = write(i2c_fd, &who_am_i_reg, 1);
    if(ret_val != 1)
        perror("write()");

    ret_val = read(i2c_fd, buf, 1);
    if(ret_val != 1)
        perror("read()");
    
    //reg_val = buf[1] | (buf[0] << 8);
    //reg_val = buf[0];
    
    printf("Who am I reg: 0x%02x\n", buf[0]);

    // ret_val = write(i2c_fd, &ctrl_reg, 1);
    // if(ret_val != sizeof(ctrl_reg))
    //     perror("write()");

    // while(1){

    //     ret_val = read(i2c_fd, buf, sizeof(who_am_i_reg));
    //     if(ret_val != sizeof(who_am_i_reg))
    //         perror("read()");
    
    //     printf("Who am I reg: 0x%02x\n", buf[0]);
    //     ret_val = write(i2c_fd, &out_x_msb, sizeof(uint8_t));
    //     if(ret_val != sizeof(out_x_msb))
    //         perror("write()");


    //     ret_val = read(i2c_fd, accl, sizeof(accl));
    //     if(ret_val != sizeof(accl))
    //         perror("read()");

    //     acc_x = (accl[0] << 8) | accl[1];
    //     acc_y = (accl[2] << 8) | accl[3];
    //     acc_z = (accl[4] << 8) | accl[5];

    //     printf("X: %d, Y: %d, Z: %d\r\n", acc_x/1024, acc_y/1024, acc_y/1024);

    // }

    return 0;

}