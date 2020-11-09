#include "functional.h"

static void print_time(TickType_t time) {
    TickType_t current_time = xTaskGetTickCount();

    char char_time[10] = {0,0,0,0,0,0,0,0,0,0,};

    if (time > 0) {
        itoa((current_time - time) / 100, char_time, 10);
        uart_write_bytes(UART_NUM, "   ", 3);
        uart_write_bytes(UART_NUM, char_time, strlen(char_time));
        uart_write_bytes(UART_NUM, " sec ago", 8);
    }
}

void print_log_data_dht11(char *argv) {
    t_dht11 data_t_h = {0, 0, 0};
    uint8_t size = uxQueueMessagesWaiting(dht_queue);
    char t[10] = {0,0,0,0,0,0,0,0,0,0,};
    char h[10] = {0,0,0,0,0,0,0,0,0,0,};

    if (argv == NULL) {
        xSemaphoreTake(xMutex, (portTickType)portMAX_DELAY);
        for (int i = 0; i < size; i++) {
            xQueueReceive(dht_queue,  &data_t_h,( TickType_t ) 0);
            uart_write_bytes(UART_NUM, "Temperature ", 12);
            itoa(data_t_h.temperature, t, 10);
            uart_write_bytes(UART_NUM, t, strlen(t));
            uart_write_bytes(UART_NUM, "   ", 3);
            uart_write_bytes(UART_NUM, "Humidity ", 9);
            itoa(data_t_h.humidity, h, 10);
            uart_write_bytes(UART_NUM, h, strlen(h));
            print_time(data_t_h.time);
            uart_write_bytes(UART_NUM, (char *)buttons.enter, 5);
            xQueueSend(dht_queue,  &data_t_h,( TickType_t ) 0);
        }
        xSemaphoreGive(xMutex);
    } else {
        error_output(argv);
    }
}