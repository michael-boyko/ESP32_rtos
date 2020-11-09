idf_component_register(SRCS "led_on.c" "led_off.c" "led_pulse.c" "functional.h" "set_time.c" "print_log_dht11.c"
                            "set_alarm.c" "play_on.c" "play_off.c"
        INCLUDE_DIRS "."
        REQUIRES mylib uart_console
        )