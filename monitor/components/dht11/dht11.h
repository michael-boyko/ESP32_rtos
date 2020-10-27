#ifndef DHT11_H
#define DHT11_H

#include "driver/gpio.h"
#include "../uart_console/readuart.h"

#define GPIO_DHT11    4
#define GPIO_EN_DHT11 2

int *get_data_dht11();
void init_dht11();

#endif