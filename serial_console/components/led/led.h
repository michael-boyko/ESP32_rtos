#ifndef LED_H
#define LED_H

#include "driver/gpio.h"
#include "../mylib/mylib.h"
#include "../uart_console/readuart.h"

#define GPIO_LED1 27
#define GPIO_LED2 26
#define GPIO_LED3 33

void led_on(char *argv);
void led_off(char *argv);
void led_pulse(char *argv);

#endif