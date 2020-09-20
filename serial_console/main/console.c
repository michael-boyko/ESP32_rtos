#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"
#include "driver/gpio.h"
#include "esp_err.h"
#include "string.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define UART_NUM UART_NUM_1
#define GPIO_LED1 27
#define GPIO_LED2 26
#define GPIO_LED3 33

static QueueHandle_t uart0_queue;

struct flags
{
    int count_str_size;
    int position;
};

struct buttons
{
    uint8_t enter[4];
    uint8_t backspace[3];
    uint8_t left[3];
    uint8_t right[3];
};

const struct buttons buttons = {
    {27, '[', 'E', '>',}, // Enter
    {0x08, ' ', 0x08,},  // Backspace
    {0x08, '[', 'D',},  // Left button
    {27, '[', 'C',},  // Right button
};

uint8_t e[3] = { 27, '[', '@',}; // insert one space
int tmp = 0;

////////////////////////////////////////////////////////////               split
char *mx_strnew(const int size)
{
    char *s = NULL;

    if (size < 0)
    {
        return NULL;
    }
    s = malloc(sizeof(char) * (size + 1));
    for(int i = 0; i < size + 1; i++)
    {
        s[i] = '\0';
    }
    return s;
}

int mx_count_words(const char *str, char c)
{
    int count = 0;

    if (str == NULL)
    {
        return -1;
    }
    while(*str != '\0')
    {
        if (*str != c)
        {
            count++;
            while(*str != c && *str != '\0')
            {
                str++;
            }
                if (*str == '\0')
            {
                return count;
            }
        }
        str++;
    }
    return count;
}

int strlen_mod(const char *s, char c)
{
    int count = 0;

    while(*s != c && *s != '\0')
    {
        count++;
        s++;
    }
    return count;
}
char **mx_strsplit(const char *s, char c)
{
    int size = 0;
    int j = 0;
    char **arr = NULL;

    if (s == NULL)
    {
        return NULL;
    }
    size = mx_count_words(s, c);
    arr = malloc((size + 1) * sizeof(char *));
    for(int i = 0; i < size; i++)
    {
        j = 0;
        while(*s == c && *s != '\0')
        {
            s++;
        }
        arr[i] = mx_strnew(strlen_mod(s, c));
        while(*s != c && *s != '\0')
        {
            arr[i][j] = *s;
            s++;
            j++;
        }
    }
    arr[size] = NULL;
    return arr;
}
///////////////////////////////////////////////////////////

void print(void *str) {
    printf("%s\n", (char *)str);
}

typedef struct a_s {
    
    void (*name) (void *data);
} a_t;

void led_on(void *flag) {
    _Bool f = (_Bool)flag;

    gpio_set_direction(GPIO_LED1, GPIO_MODE_OUTPUT);
    if (f == 1) {
        gpio_set_level(GPIO_LED1, 1);
    }
    else {
        gpio_set_level(GPIO_LED1, 0);
    }
}

void command_handler(char *str)
{
    char ttt[] = "asdfdafdfaffasfasf";
    a_t test[2];
    test[0].name = print;
    test[1].name = led_on;
    test[0].name((void *)ttt);
    test[1].name((void *)1);
    char **arr = mx_strsplit(str, ' ');


    for (int i = 0; arr[i] != NULL; i++) {
        printf("%s\n", arr[i]);
    }
}

void del_symbol_inside_str(char *str, int position)
{
    int len = strlen(str);

    for (int i = position; i <= len; i++)
    {
        str[i - 1] = str[i];
    }
}

void add_symbol_inside_str(char *str, int position, char c)
{
    char tmp = '\0';
    int len = strlen(str);

    for (int i = position; i <= len; i++)
    {
        tmp = str[i];
        str[i] = c;
        c = tmp;
    }
}

void add_symbol_to_str(char *str, struct flags *f) 
{
    uint8_t *buf = NULL;
    int readed = 0;
    size_t buf_size = 0;
    char big_str[] = ">Your command is to bid";

    uart_get_buffered_data_len(UART_NUM, &buf_size);
    printf("%d\n", buf_size);
    if (buf_size > 30) 
    {
        uart_write_bytes(UART_NUM, (char *)buttons.enter, 3);
        uart_write_bytes(UART_NUM, big_str, strlen(big_str));
        uart_write_bytes(UART_NUM, (char *)buttons.enter, 4);
        uart_flush(UART_NUM);
        f->position = 0;
        f->count_str_size = 0;
    } 
    else 
    {
        buf = malloc(sizeof(uint8_t) * (buf_size + 1));
        memset(buf, '\0', buf_size + 1);
        readed = uart_read_bytes(UART_NUM, buf, buf_size + 1, buf_size);
        if (buf[0] == 27)
        {
            if (buf[2] == 'D' && f->position > 0) 
            {
                uart_write_bytes(UART_NUM, (char *)buttons.left, 1);
                f->position--;
            }
            else if (buf[2] == 'C' && f->position < f->count_str_size)
            {
                uart_write_bytes(UART_NUM, (char *)buttons.right, 3);
                f->position++;
            }
        }
        else
        {
            if (buf[0] == 13)
            {
                uart_write_bytes(UART_NUM, (char *)buttons.enter, 4);
                uart_flush(UART_NUM);

                command_handler(str);
                if (strcmp("clear", str) == 0)
                {
                    uint8_t clear[8] = {27, '[', '2', 'J', 27, '[', 'H', '>'};
                    uart_write_bytes(UART_NUM, (char *)clear, 8);
                }

                memset(str, '\0', strlen(str));
                f->position = 0;
                f->count_str_size = 0;
            } 
            else if (buf[0] == 127)
            {
                if (f->position == f->count_str_size && f->position != 0)
                {
                    uart_write_bytes(UART_NUM, (char *)buttons.backspace, 3);
                    f->count_str_size -= buf_size;
                    f->position = f->count_str_size;
                    str[f->count_str_size] = '\0'; //delete last symbol from str
                }
                else if (f->position != 0)
                {
                    uint8_t esc1[4] = {0x08, 27, '[', 'P'}; // true backspace  
                    uart_write_bytes(UART_NUM_1, (char *)esc1, 4);
                    del_symbol_inside_str(str, f->position);
                    f->count_str_size--;
                    f->position--;
                }
            }
            else
            {
                    if (tmp % 3 == 0 && (buf_size >= 2 && buf_size <=6)) 
                    {
                        printf("ESC\n");
                    }
                    else if (f->position != f->count_str_size)
                    {
                        if (readed == 1)
                        {
                            uart_write_bytes(UART_NUM, (char *)e, 3);
                            uart_write_bytes(UART_NUM, (char *)buf, buf_size);
                            add_symbol_inside_str(str, f->position, (char)buf[0]);
                            f->count_str_size += buf_size;
                            f->position++;
                        }
                        else
                        {
                            char *c = (char *)buf;
                            for (int i = 0; i < readed; i++)
                            {
                                char *t = &c[i];
                                uart_write_bytes(UART_NUM, (char *)e, 3);
                                uart_write_bytes(UART_NUM, t, 1);
                                add_symbol_inside_str(str, f->position, c[i]);
                                f->count_str_size++;
                                f->position++;
                            }
                        }
                    }
                    else
                    {
                        strcat(str, (char *)buf);
                        uart_write_bytes(UART_NUM, (char *)buf, buf_size);
                        f->count_str_size += buf_size;
                        f->position = f->count_str_size;
                    }
            }
        }
        tmp = buf_size;
        free(buf);
    }
}

void task_read_bytes() {
    uart_event_t event;
    char str[1024];
    uint8_t start[3] = {'>'};
    struct flags f;
    f.count_str_size = 0;
    f.position = 0;
    uint8_t clear[4] = {27, '[', '2', 'J'}; // clear command
    uint8_t enter[] = {27, '[', 'H'}; // to home

    memset(str, '\0', 1024);
    uart_write_bytes(UART_NUM, (char *)clear, 4);
    uart_write_bytes(UART_NUM, (char *)enter, 3);
    uart_write_bytes(UART_NUM, (char *)start, 1);
    while (true) {
        if (xQueueReceive(uart0_queue, (void * )&event, (portTickType)portMAX_DELAY)) {
            // uart_disable_pattern_det_intr(UART_NUM);
            if (event.type == UART_DATA) {
                add_symbol_to_str(str, &f);
                printf("C = %d\n P = %d\n", f.count_str_size, f.position);
            }
        }
        printf("%s\n", str);
        // uart_pattern_queue_reset(UART_NUM, 20); 
    }
}

void app_main() 
{
    uart_config_t uart_config = {
        .baud_rate = 9600,
        .data_bits = UART_DATA_8_BITS,
        .parity    = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
    };
    
    uart_driver_install(UART_NUM, 2048, 2048, 20, &uart0_queue, 0);
    uart_param_config(UART_NUM, &uart_config);

    uart_set_pin(UART_NUM, 17, 16, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    uart_pattern_queue_reset(UART_NUM, 20);

    xTaskCreate(task_read_bytes, "task_read_bytes", 12048, NULL, 10, NULL);

}
