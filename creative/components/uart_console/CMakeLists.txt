idf_component_register(SRCS "read_bytes_from_uart.c" "readuart.h" "error_output.c"
                       INCLUDE_DIRS "."
                       REQUIRES command_api oled dht11 functional main
                      )