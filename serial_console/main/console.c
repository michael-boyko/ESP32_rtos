#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"
#include "esp_err.h"
#include "string.h"
#include "stdio.h"

static QueueHandle_t uart0_queue;

struct flags
{
    int count_str_size;
    int position;
};



void add_symbol_to_str(char *str, struct flags *f) 
{
    uint8_t *buf = NULL;
    int readed = 0;
    size_t buf_size = 0;
    char big_str[] = "Your command is to bid";
    uint8_t enter[3] = {'\n', 13, 0};
    uint8_t backspace[3] = {27, 91, 68};
    char space = ' ';

    uart_get_buffered_data_len(UART_NUM_1, &buf_size);
    if (buf_size > 30) 
    {
        uart_write_bytes(UART_NUM_1, (char *)enter, 3);
        uart_write_bytes(UART_NUM_1, big_str, strlen(big_str));
        enter[2] = '>';
        uart_write_bytes(UART_NUM_1, (char *)enter, 3);
        uart_flush(UART_NUM_1);
        f->position = 0;
        f->count_str_size = 0;
    } 
    else 
    {
        buf = malloc(sizeof(uint8_t) * (buf_size + 1));
        memset(buf, '\0', buf_size + 1);
        readed = uart_read_bytes(UART_NUM_1, buf, buf_size + 1, buf_size);
        if (readed == 3)
        {
            if (buf[2] == 'D' && f->position > 0) 
            {
                uart_write_bytes(UART_NUM_1, (char *)buf, buf_size);
                printf("%d %d\n", buf[1], buf[2]);
                f->position--;
            }
            else if (buf[2] == 'C' && f->position < f->count_str_size)
            {
                uart_write_bytes(UART_NUM_1, (char *)buf, buf_size);
                f->position++;
            }
        }
        else
        {
            if (buf[0] == 13)
            {
                enter[2] = '>';
                uart_write_bytes(UART_NUM_1, (char *)enter, 3);
                uart_flush(UART_NUM_1);
                f->position = 0;
                f->count_str_size = 0;
            } 
            else if (buf[0] == 127)
            {
                if (f->position == f->count_str_size && f->position != 0)
                {
                    uart_flush_input(UART_NUM_1);
                    uart_write_bytes(UART_NUM_1, (char *)backspace, 3);
                    uart_write_bytes(UART_NUM_1, &space, buf_size);
                    uart_write_bytes(UART_NUM_1, (char *)backspace, 3);
                    f->count_str_size -= buf_size;
                    f->position = f->count_str_size;
                }
            }
            else
            {
                uart_write_bytes(UART_NUM_1, (char *)buf, buf_size);
                f->count_str_size += buf_size;
                f->position = f->count_str_size;
            }
        }
        free(buf);
    }
}

void task_read_bytes() {
    uart_event_t event;
    char str[1024];
    uint8_t start[3] = {'\n', 13, '>'};
    struct flags f;
    f.count_str_size = 0;
    f.position = 0;

    memset(str, '\0', 1024);
    uart_write_bytes(UART_NUM_1, (char *)start, 3);
    while (true) {
        if (xQueueReceive(uart0_queue, (void * )&event, (portTickType)portMAX_DELAY)) {
            if (event.type == UART_DATA) {
                add_symbol_to_str(str, &f);
                printf("C = %d\n P = %d\n", f.count_str_size, f.position);
                // data_in_screen(str);
            }
        }
    }
}

void app_main() {
    uart_config_t uart_config = {
        .baud_rate = 9600,
        .data_bits = UART_DATA_8_BITS,
        .parity    = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
    };
    
    uart_driver_install(UART_NUM_1, 2048, 2048, 20, &uart0_queue, 0);
    uart_param_config(UART_NUM_1, &uart_config);

    uart_set_pin(UART_NUM_1, 17, 16, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    uart_pattern_queue_reset(UART_NUM_1, 20);

    xTaskCreate(task_read_bytes, "task_read_bytes", 12048, NULL, 10, NULL);

}
