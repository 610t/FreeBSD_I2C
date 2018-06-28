#include <sys/cdefs.h>

#include <err.h>
#include <errno.h>
#include <sysexits.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/sysctl.h>
#include <dev/iicbus/iic.h>

#define I2C_DEV                 "/dev/iic1"
#define I2C_ADDR		0x3f

static int
i2c_write(uint8_t data,int offset)
{
  int fd,i;
  int bufsize=1;
  struct iic_msg msg;
  struct iic_rdwr_data rdwr;
  uint8_t *buf,*dbuf;
	

  fd = open(I2C_DEV, O_RDWR);

  dbuf = malloc( 2 * sizeof(uint8_t) );

  dbuf[0]=offset & 0xff;
  dbuf[1]=data;

  msg.slave = I2C_ADDR << 1 | !IIC_M_RD;
  msg.flags = !IIC_M_RD;
  msg.len = 2 * sizeof( uint8_t );                          
  msg.buf = dbuf;
  rdwr.msgs = &msg;
  rdwr.nmsgs = 1;

  ioctl(fd, I2CRDWR, &rdwr);
	
  close(fd);
  return (0);
}

static int
i2c_read()
{
  int fd;
  struct iic_msg *msg;
  struct iic_rdwr_data rdwr;
  uint8_t *i2c_buf;
  uint8_t buf[0];

  fd = open(I2C_DEV, O_RDWR);

  buf[0] = 0;

  i2c_buf = malloc(2);
  msg = malloc( 2 * sizeof(struct iic_msg) );

  msg[0].slave = I2C_ADDR << 1 | !IIC_M_RD;                                   
  msg[0].flags = !IIC_M_RD;
  msg[0].len = sizeof( uint8_t );
  msg[0].buf = &buf[0];

  msg[1].slave = I2C_ADDR << 1 | IIC_M_RD;                            
  msg[1].flags = IIC_M_RD;
  msg[1].len = sizeof( uint8_t );                                         
  msg[1].buf = &i2c_buf[0];

  rdwr.msgs = msg;
  rdwr.nmsgs = 2;

  ioctl(fd, I2CRDWR, &rdwr);

  printf("Data:%02x \n",i2c_buf[0]);

  close(fd);

	
  return (0);
}

int
main(int argc, char** argv)
{
  /* Initialize */
  i2c_write(0x1f,0x03);
  i2c_write(0x00,0x02);
  i2c_write(0x00,0x01);

  /* Read forever */
  while (1)
    {
      i2c_read();
      sleep(1);
    }
}
