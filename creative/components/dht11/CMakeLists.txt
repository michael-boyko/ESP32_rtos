idf_component_register(SRCS "get_data_dht11.c" "dht11.h" "init_dht11.c"
                       INCLUDE_DIRS "."
                       REQUIRES uart_console
                      )