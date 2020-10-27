#ifndef READUART_H
#define READUART_H

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"
#include "driver/ledc.h"
#include "esp_err.h"
#include "driver/uart.h"
#include "../command_api/command_api.h"
#include "../mylib/mylib.h"
#include "../dht11/dht11.h"

#define UART_NUM UART_NUM_1

QueueHandle_t uart0_queue;

typedef struct s_flags {
    int count_str_size;
    int position;
} t_flag;

static struct buttons
{
    uint8_t enter[4];
    uint8_t backspace[3];
    uint8_t left[3];
    uint8_t right[3];
};

static struct buttons buttons = {
        {27, '[', 'E', '>',}, // Enter
        {0x08, ' ', 0x08,},  // Backspace
        {0x08, '[', 'D',},  // Left button
        {27, '[', 'C',},  // Right button
};

static uint8_t insert_one_space[3] = { 27, '[', '@',}; // insert one space

void uart_event_handler();
void error_output(char *argv);

#endif