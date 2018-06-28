#!/bin/sh

I2C=/home/mutoh/newi2c/pi2c
DEV=/dev/iic1
ADDR=0x23

outbin() {
    printf '\'`echo "ibase=16;obase=8 ;"$1 | bc `                          
}

outbin 10 | sudo ${I2C} -f ${DEV} -a ${ADDR} -d w -v -b
sleep 0.1

while true
do
    sudo ${I2C} -f ${DEV} -a ${ADDR} -d r -c 2 -v 
    sleep 1
done
