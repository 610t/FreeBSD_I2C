#!/bin/sh

I2C=i2c
DEV=/dev/iic0
ADDR=0x52

while true
do
    sudo ${I2C} -f ${DEV} -a ${ADDR} -d r -c 3 -m tr
    sleep 1
done
