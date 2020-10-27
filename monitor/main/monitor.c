#include "monitor.h"



void ts() {
//    TickType_t x = 0;
    int *arr = NULL;

    while (true) {

//        x = xTaskGetTickCount();
//        printf("%d\n", x / 100);
        vTaskDelay(5000 / portTICK_PERIOD_MS);
        arr = get_data_dht11();
        printf("h = %d\nt = %d\n", arr[0], arr[2]);

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

    uart_driver_install(UART_NUM, 2048, 2048, 20, &uart0_queue, 0);
    uart_param_config(UART_NUM, &uart_config);

    uart_set_pin(UART_NUM, 17, 16, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    uart_pattern_queue_reset(UART_NUM, 20);

    init_dht11();
    xTaskCreate(uart_event_handler, "read_bytes_from_uart", 12004, NULL, 10, NULL);
    xTaskCreate(ts, "ts", 4208, NULL, 10, NULL);
}