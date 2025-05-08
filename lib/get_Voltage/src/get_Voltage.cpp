#include <stdio.h>
#include <stdlib.h>
#include <Arduino.h>
#include "get_Voltage.h"
#include <config.h>


/*
get_raw_voltage
Returns BATTERY_GPIO raw analog value
*/
uint16_t get_raw_voltage(uint16_t BATTERY_GPIO){
    uint16_t rawVoltage = analogRead(BATTERY_GPIO);
    return rawVoltage;
}
/*
get_mv_gpio
Returns BATTERY_GPIO analog value in milivolts
*/
uint16_t get_mv_voltage(uint16_t BATTERY_GPIO){
    uint16_t mvVoltage = analogReadMilliVolts(BATTERY_GPIO);
    return mvVoltage;
}