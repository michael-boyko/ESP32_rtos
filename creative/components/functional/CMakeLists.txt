idf_component_register(SRCS "led_on.c" "led_off.c" "led_pulse.c" "functional.h" "set_time.c"
        INCLUDE_DIRS "."
        REQUIRES mylib uart_console
        )