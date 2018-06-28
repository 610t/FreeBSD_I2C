#include <sys/types.h>
#include <sys/ioctl.h>
#include <dev/iicbus/iic.h>

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

#define IICBUS	"/dev/iic1"
#define IICADDR	0x23

static int
i2c_write(int fd, uint8_t data,int offset)
{
  uint8_t cmdbuff[12] = {0};
  struct iic_msg write_cmd[1] = {
				 {
				  .slave = IICADDR << 1 | !IIC_M_RD,
				  .flags = !IIC_M_RD,
				  .len = sizeof (uint8_t),
				  .buf = cmdbuff,
				 }, 
  };

  struct iic_rdwr_data cmd = {
			      .msgs =  write_cmd,
			      .nmsgs = 1,
  };

  cmdbuff[0]=data;

  ioctl(fd, I2CRDWR, &cmd);
  sleep(1);
  return 0;
}

int main(int argc, char *argv[])
{
  int i;
  int fd;
  uint8_t cmdbuff[12] = {0};
  uint8_t buf[1]={0};
  uint8_t retbuf[4]={0};
  struct iic_msg write_cmd[1] = {
				 {
				  .slave = IICADDR << 1 | IIC_M_RD,
				  .flags = IIC_M_RD,                                        
				  .len = sizeof(cmdbuff),
				  .buf = cmdbuff,                                           
				 },
  };
  struct iic_rdwr_data cmd = {
			      .msgs =  write_cmd,
			      .nmsgs = 2,
  };
  if ((fd = open(IICBUS, O_RDWR)) == -1) {
    fprintf(stderr, "%d Error %s\n", __LINE__, strerror(errno));
    return 1;
  }

  /* Initialize */
  i2c_write(fd,0x10,0x00);

  while (1) {
    int ret=0;
    int ldata=0;
    float light=0;

    ioctl(fd, I2CRDWR, &cmd);
    ldata=cmdbuff[0]<<8|cmdbuff[1];
    light=ldata/1.2;
    printf("cmdbuf:%02x %02x %02x %02x\n", cmdbuff[0], cmdbuff[1], cmdbuff[2], cmdbuff[3]);
    printf("light:%f, ldata:%d\n",light,ldata);
		
    sleep(1);
  }

  close(fd);
  return 0;
}

