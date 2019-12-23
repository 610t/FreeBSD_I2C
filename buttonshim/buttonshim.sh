#!/bin/sh

I2C=/usr/sbin/i2c
DEV=/dev/iic0
ADDR=0x3f
COM_OPT="-m tr -f ${DEV} -a ${ADDR} -c 1 -v -b "

outbin() {
    printf '\'`echo "ibase=16;obase=8 ;"$1 | bc `                          
}

outbin 1F | sudo ${I2C} ${COM_OPT} -d w -o 0x3
outbin 00 | sudo ${I2C} ${COM_OPT} -d w -o 0x2
outbin 00 | sudo ${I2C} ${COM_OPT} -d w -o 0x1

while true
do
    sudo ${I2C} ${COM_OPT} -d r -c 1
    sleep 1
done
