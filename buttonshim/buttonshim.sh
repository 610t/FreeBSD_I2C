#!/bin/sh

I2C=/home/mutoh/newi2c/pi2c
DEV=/dev/iic1
ADDR=0x3f

outbin() {
    printf '\'`echo "ibase=16;obase=8 ;"$1 | bc `                          
}

outbin 1F | sudo ${I2C} -f ${DEV} -a ${ADDR} -d w -c 1 -v -b -o 0x3
outbin 00 | sudo ${I2C} -f ${DEV} -a ${ADDR} -d w -c 1 -v -b -o 0x2
outbin 00 | sudo ${I2C} -f ${DEV} -a ${ADDR} -d w -c 1 -v -b -o 0x1

while true
do
    sudo ${I2C} -f ${DEV} -a ${ADDR} -d r -c 1
    sleep 1
done
