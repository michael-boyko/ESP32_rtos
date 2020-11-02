#include "readuart.h"

void error_output(char *argv) {
    char error[] = "bad argument: ";

    uart_write_bytes(UART_NUM, error, strlen(error));
    uart_write_bytes(UART_NUM, argv, strlen(argv));
    uart_write_bytes(UART_NUM, (char *)buttons.enter, 4);
}
