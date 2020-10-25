#include "console.h"

void app_main() {
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

    xTaskCreate(uart_event_handler, "read_bytes_from_uart", 120048, NULL, 10, NULL);
//    xTaskCreatePinnedToCore( uart_event_handler, "read_bytes_from_uart", 12048, NULL, 10, 0x7FFFFFFF);
    // xTaskCreate(pulse_led_use_PWM, "pulse_led_use_PWM", 12048, NULL, 10, NULL);
}


