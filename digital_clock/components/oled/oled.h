#ifndef OLED_H
#define OLED_H

#include <dirent.h>
#include "esp_system.h"
#include "driver/gpio.h"
#include "driver/i2c.h"
#include "driver/spi_master.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
//#include "esp_spi_flash.h"
#include "driver/dac.h"
#include <string.h>
#include <unistd.h>
#include "font6x8.h"


#define GPIO_EN_OLED  32
#define GPIO_EN_ACCE  23
#define GPIO_SDA      21
#define GPIO_SCL      22
#define DISPLAY_PORT  I2C_NUM_0
#define DISPLAY_ADDR  0x3C 

//oled
void init_i2c();
void power_on_oled();
void init_display();
void screen_onto_display(uint8_t **screen);
void str_in_screen(char *str, uint8_t **screen);
void fill_screen_0_or_1(uint8_t **screen, _Bool on_off);
uint8_t **create_arr();

#endif
