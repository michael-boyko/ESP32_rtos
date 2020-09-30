#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"
#include "driver/gpio.h"
#include "driver/ledc.h"
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
static QueueHandle_t led_queue;
static QueueHandle_t pulse_queue;

struct flags
{
    int count_str_size;
    int position;
};
    
struct led_on_off
{
    int status;
    int num;
};

struct led_pulse
{
    int frequency;
    int num;
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

void mx_free_arr(void **arr) {
    if (arr != NULL) {
        for (int i = 0; arr[i] != NULL; i++) {
            free(arr[i]);
        }
        free(arr);
    }
}
///////////////////////////////////////////////////////////

void wrong_command(char *str) {
    uart_write_bytes(UART_NUM, "command not found: ", strlen("command not found: "));
    uart_write_bytes(UART_NUM, str, strlen(str));
    uart_write_bytes(UART_NUM, (char *)buttons.enter, 4);
}

int arr_size(char **arr) {
    int size = 0;

    if (arr == NULL) {
        return -1;
    }

    for (int i = 0; arr[i] != NULL; i++) {
        size++;
    }
    return size;
}

void command_handler(char *str) {
    int data = 0;
    char **arr = mx_strsplit(str, ' ');
    struct led_on_off *led = (struct led_on_off *) malloc(sizeof(struct led_on_off) * 1);
    struct led_pulse *pulse = (struct led_pulse *) malloc(sizeof(struct led_pulse) * 1);

    data = arr_size(arr);
    if (data > 0 && data < 5) {

    if (strcmp(arr[0], "led") == 0) {
        if (strcmp(arr[1], "on") == 0) {
            data = atoi(arr[2]);
            if (data > 0 && data < 4) {
                led->status = 1;
                led->num = data;
                xQueueSend( led_queue, ( void * ) &led, ( TickType_t ) 0 );
            }
            else {
                wrong_command(arr[2]);
            }
        } else if (strcmp(arr[1], "off") == 0) {
            data = atoi(arr[2]);
            if (data > 0 && data < 4) {
                led->status = 0;
                led->num = data;
                xQueueSend( led_queue, ( void * ) &led, ( TickType_t ) 0 );
            }
            else {
                wrong_command(arr[2]);
            }
        } else if (strcmp(arr[1], "pulse") == 0) {
            data = atoi(arr[2]);

            if (data > 0 && data < 4) {
                pulse->num = data;
                data = atoi(arr[3]);
                pulse->frequency = data;
                xQueueSend(pulse_queue, (void *) &pulse, ( TickType_t ) 0);
            }
            else {
                wrong_command(arr[2]);
            }
        }
        else {
            wrong_command(arr[1]);
        }
    }
    else {
        wrong_command(arr[0]);
    }
    }
    else {
        // wrong_command(" ");
    }
    mx_free_arr((void **)arr);
    free(led);
    free(pulse);
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
    // printf("%d\n", buf_size);
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
                // printf("C = %d\n P = %d\n", f.count_str_size, f.position);
            }
        }
        // printf("%s\n", str);
        // uart_pattern_queue_reset(UART_NUM, 20); 
    }
}

void pulse_led_use_PWM() {
    struct led_pulse *pulse = NULL;
    ledc_timer_config_t led_timer = {
        
            .speed_mode = LEDC_HIGH_SPEED_MODE,
            .timer_num = LEDC_TIMER_0,
            .freq_hz = 5000,
            .duty_resolution = LEDC_TIMER_13_BIT,
            .clk_cfg = LEDC_AUTO_CLK,   
        };

    ledc_channel_config_t channel[3] = {
        {
            .channel    = LEDC_CHANNEL_0,
            .duty       = 0,
            .gpio_num   = 26,
            .speed_mode = LEDC_HIGH_SPEED_MODE,
            .hpoint     = 0,
            .timer_sel  = LEDC_TIMER_0
        },
        {
            .channel    = LEDC_CHANNEL_1,
            .duty       = 0,
            .gpio_num   = 27,
            .speed_mode = LEDC_HIGH_SPEED_MODE,
            .hpoint     = 0,
            .timer_sel  = LEDC_TIMER_0
        },
        {
            .channel    = LEDC_CHANNEL_2,
            .duty       = 0,
            .gpio_num   = 33,
            .speed_mode = LEDC_HIGH_SPEED_MODE,
            .hpoint     = 0,
            .timer_sel  = LEDC_TIMER_0
        },
    };


    ledc_fade_func_install(0);
    for (int i = 0; i < 3; i++) {
        
        ledc_channel_config(&channel[i]);
    }
    ledc_timer_config(&led_timer);
    while (1) {
        for (int i = 0; i < 3; i++) {
            ledc_set_fade_with_time(channel[i].speed_mode, channel[i].channel, 3000, 100);
            ledc_fade_start(channel[i].speed_mode, channel[i].channel, LEDC_FADE_NO_WAIT);
        }
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        for (int i = 0; i < 3; i++) {
            ledc_set_fade_with_time(channel[i].speed_mode, channel[i].channel, 0, 100);
            ledc_fade_start(channel[i].speed_mode, channel[i].channel, LEDC_FADE_NO_WAIT);
        }
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }

    // int frequency = 0;

    // ledc_timer_config(&led_timer);
    // ledc_channel_config(&channel[0]);
    // ledc_fade_func_install(0);
    // while (1) {
    //     // printf("11111111111-----11111111111\n");
    //     if (xQueueReceive(pulse_queue, (void * )&pulse, (portTickType)portMAX_DELAY)) {
    //         gpio_set_level(GPIO_LED1, 0);
    //         ledc_timer_config(&led_timer);
    // ledc_channel_config(&channel[0]);
    // ledc_fade_func_install(0);
    // frequency = pulse->frequency;
    //         // gpio_set_direction(GPIO_LED3, GPIO_MODE_DISABLE);
            
    //         while(1) {
    //             ledc_set_fade_with_time(channel[0].speed_mode, channel[0].channel, 3000, 100);
    //             ledc_fade_start(channel[0].speed_mode, channel[0].channel, LEDC_FADE_NO_WAIT);
    //                             if (xQueueReceive(pulse_queue, (void * )&pulse, 10)) {
    //                                 frequency = pulse->frequency;
    //                                 gpio_set_level(GPIO_LED1, 0);
    //                 ledc_timer_config(&led_timer);
    //         ledc_channel_config(&channel[0]);
    //         ledc_fade_func_install(0);
    //                 printf("111111111111\n");
    //             }
    //             ledc_set_fade_with_time(channel[0].speed_mode, channel[0].channel, 0, 100);
    //             ledc_fade_start(channel[0].speed_mode, channel[0].channel, LEDC_FADE_NO_WAIT);
    //             // vTaskDelay(4000 / portTICK_PERIOD_MS);
    //             printf("1\n");
    //             if (xQueueReceive(pulse_queue, (void * )&pulse, 10)) {
    //                 gpio_set_level(GPIO_LED1, 0);
    //                 ledc_timer_config(&led_timer);
    //         ledc_channel_config(&channel[0]);
    //         ledc_fade_func_install(0);
    //                 printf("222222222222\n");
    //             }
    //         }
    //     }
    // }
    
}

void led_on_off() {
    struct led_on_off *led = NULL;

    while (true) {
        if (xQueueReceive(led_queue, (void * )&led, (portTickType)portMAX_DELAY)) {
            switch (led->num) {
                case 1:
                gpio_set_direction(GPIO_LED1, GPIO_MODE_OUTPUT);
                gpio_set_level(GPIO_LED1, led->status);
                break;

                case 2:
                gpio_set_direction(GPIO_LED2, GPIO_MODE_OUTPUT);
                gpio_set_level(GPIO_LED2, led->status);
                break;

                case 3:
                gpio_set_direction(GPIO_LED3, GPIO_MODE_OUTPUT);
                gpio_set_level(GPIO_LED3, led->status);
                break;
            }
        }
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
    // gpio_set_direction(33, GPIO_MODE_OUTPUT);
    // gpio_set_level(33, 1);
    gpio_set_pull_mode(33, GPIO_PULLUP_ONLY);
    pulse_led_use_PWM(); // delete
    
    // uart_driver_install(UART_NUM, 2048, 2048, 20, &uart0_queue, 0);
    // uart_param_config(UART_NUM, &uart_config);

    // uart_set_pin(UART_NUM, 17, 16, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    // uart_pattern_queue_reset(UART_NUM, 20);

    // led_queue = xQueueCreate( 10, sizeof( struct led_on_off * ) );
    // pulse_queue = xQueueCreate( 10, sizeof( struct led_pulse * ) );
    // if (pulse_queue == NULL)
    //     printf("pulse_queue ------\n");

    // xTaskCreate(task_read_bytes, "task_read_bytes", 12048, NULL, 10, NULL);
    // xTaskCreate(pulse_led_use_PWM, "pulse_led_use_PWM", 12048, NULL, 10, NULL);
    // xTaskCreate(led_on_off, "led_on_off", 1048, NULL, 10, NULL);

}


