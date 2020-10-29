#include "clock.h"

static char *add_0_to_str_time(uint8_t num) {
    char *c = mx_strnew(2);

    if (num < 10) {
        c[0] = '0';
        c[1] = num + 48;
    } else {
        itoa(num, c, 10);
    }

    return c;
}

static char *correct_time_format(uint32_t time_in_sec) {
    char *hours = NULL;
    char *minutes = NULL;
    char *seconds = NULL;
    char *str = mx_strnew(8);

    hours = add_0_to_str_time(time_in_sec / 3600);
    minutes = add_0_to_str_time((time_in_sec % 3600) / 60);
    seconds = add_0_to_str_time((time_in_sec % 3600) % 60);
    sprintf(str,"%s:%s:%s", hours, minutes, seconds);
    free(hours);
    free(minutes);
    free(seconds);

    return str;
}

void sec_to_data() {
    uint8_t **screen = create_arr();
    uint32_t time_in_sec = 86365;
    char *time_in_str =NULL;

    fill_screen_0_or_1((uint8_t **)screen, 0);
    while (1) {
        if (time_in_sec == 86400) {
            time_in_sec = 0;
        }
        if (xQueueReceive(set_time_queue, &time_in_sec, ( TickType_t ) 0));

        time_in_str = correct_time_format(time_in_sec);
        fill_screen_0_or_1((uint8_t **)screen, 0);
        str_in_screen((uint8_t **)time_in_str, screen);
        screen_onto_display(screen);
        free(time_in_str);
        time_in_sec++;
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    vTaskDelete(NULL);
}

void app_main() {
    uart_config_t uart_config = {
            .baud_rate = 9600,
            .data_bits = UART_DATA_8_BITS,
            .parity    = UART_PARITY_DISABLE,
            .stop_bits = UART_STOP_BITS_1,
            .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
    };

    set_time_queue = xQueueCreate(1, sizeof(uint32_t));
    power_on_oled();
    init_i2c();
    init_display();
    uart_driver_install(UART_NUM, 2048, 2048, 20, &uart0_queue, 0);
    uart_param_config(UART_NUM, &uart_config);

    uart_set_pin(UART_NUM, 17, 16, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    uart_pattern_queue_reset(UART_NUM, 20);

    xTaskCreate(uart_event_handler, "read_bytes_from_uart", 12004, NULL, 10, NULL);
    xTaskCreate(sec_to_data, "sec", 12004, NULL, 10, NULL);

}