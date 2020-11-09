#ifndef FUNCTIONAL_H
#define FUNCTIONAL_H

#include "driver/ledc.h"
#include "driver/gpio.h"
#include "../mylib/mylib.h"
#include "../uart_console/readuart.h"


#define GPIO_LED1 27
#define GPIO_LED2 26
#define GPIO_LED3 33

void led_on(char *argv);
void led_off(char *argv);
void led_pulse(char *argv);
void set_time(char *argv);
void print_log_data_dht11(char *argv);
void set_alarm(char *argv);
void play_on(char *argv);
void play_off(char *argv);

#endif