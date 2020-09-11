#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"
#include "esp_err.h"
#include "string.h"
#include "stdio.h"

void app_main() {
	uint8_t buf[1024];
	char str2[1024];
	memset(buf, '\0', 1024);
	memset(str2, '\0', 1024);
	char *str = "\e[1;41mRED\e[1;49m \e[1;42mGREEN\e[1;49m \e[1;46mBLUE\e[1;49m DEFAULT\r\n";
    uart_config_t uart_config = {
        .baud_rate = 9600,
        .data_bits = UART_DATA_8_BITS,
        .parity    = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
    };
    uart_param_config(UART_NUM_1, &uart_config);
    uart_set_pin(UART_NUM_1, 17, 16, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    uart_driver_install(UART_NUM_1, 1024, 0, 0, NULL, 0);
    printf("afdasfasfafddafdafdafafafsadfsfsafsafsdfsadf\n");
    uart_write_bytes(UART_NUM_1, str, strlen(str));
    int asc = 0;
    while (true) {
	    while ((asc = uart_read_bytes(UART_NUM_1, buf, 1024, 1)) == 0) {

    	}
    	if (asc == 8) {
    		printf("bachspace\n");
    	}
    	uart_write_bytes(UART_NUM_1, (char *)buf, 1);
	}
    printf("%s\n", buf);
}
