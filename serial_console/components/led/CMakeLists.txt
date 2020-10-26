idf_component_register(SRCS "led_on.c" "led_off.c" "led_pulse.c" "led.h"
        INCLUDE_DIRS "."
        REQUIRES mylib uart_console
        )