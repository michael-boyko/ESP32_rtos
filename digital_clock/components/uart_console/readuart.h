#ifndef READUART_H
#define READUART_H

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"
#include "driver/ledc.h"
#include "esp_err.h"
#include "driver/uart.h"
#include "freertos/queue.h"
#include "../command_api/command_api.h"
#include "../mylib/mylib.h"
#include "../oled/oled.h"
#include "freertos/semphr.h"
#include "driver/timer.h"

#define UART_NUM UART_NUM_1

QueueHandle_t uart0_queue;
QueueHandle_t set_time_queue;
volatile xSemaphoreHandle xMutex;
TaskHandle_t xTaskToNotify;

typedef struct s_dht11 {
    int temperature;
    int humidity;
    TickType_t time;
} t_dht11;

typedef struct s_flags {
    int count_str_size;
    int position;
} t_flag;

static struct buttons
{
    char enter[5];
    uint8_t backspace[3];
    uint8_t left[3];
    uint8_t right[3];
};

static struct buttons buttons = {
        "\n\r>", // Enter
        {0x08, ' ', 0x08,},  // Backspace
        {0x08, '[', 'D',},  // Left button
        {27, '[', 'C',},  // Right button
};

static uint8_t insert_one_space[3] = { 27, '[', '@',}; // insert one space

void uart_event_handler();
void error_output(char *argv);

#endif