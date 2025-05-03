#include <stdio.h>
#include <stdlib.h>
#include <Arduino.h>
#include "get_Voltage.h"
#ifndef config
#include <config.h>
#endif

/*
get_raw_gpio
Returns BATTERY_GPIO raw analog value

uint16_t get_raw_voltage(){
    rawVoltage = analogRead(BATTERY_GPIO);
    return rawVoltage;
}
    */