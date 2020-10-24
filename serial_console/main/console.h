#ifndef CONSOLE_H
#define CONSOLE_H

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"
#include "driver/gpio.h"
#include "driver/ledc.h"
#include "esp_err.h"
#include "driver/uart.h"
#include "../components/mylib/mylib.h"
#include "../components/uart_console/readuart.h"
#include "../components/command_api/command_api.h"

#define UART_NUM UART_NUM_1
#define GPIO_LED1 27
#define GPIO_LED2 26
#define GPIO_LED3 33

QueueHandle_t uart0_queue;
//QueueHandle_t led_queue;
//QueueHandle_t pulse_queue;

typedef struct s_flags {
    int count_str_size;
    int position;
} t_flag;
//
//struct led_on_off
//{
//    int status;
//    int num;
//};
//
//struct led_pulse
//{
//    int frequency;
//    int num;
//};
//

//int tmp;

#endif