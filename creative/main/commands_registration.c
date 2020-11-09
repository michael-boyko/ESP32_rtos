#include "creative.h"

static void clear_command(char *argv) {
    uint8_t clear[8] = {27, '[', '2', 'J', 27, '[', 'H', '>'};

    if (argv == NULL) {
        uart_write_bytes(UART_NUM, (char *)clear, 8);
    } else {
        error_output(argv);
    }
}

void commands_registration(t_pars_tree **commands) {
    t_command_config cc0 = {
            .command = "clear",
            .name_func = clear_command,
    };
    t_command_config cc1 = {
            .command = "led on",
            .name_func = led_on,
    };
    t_command_config cc2 = {
            .command = "led off",
            .name_func = led_off,
    };
    t_command_config cc3 = {
            .command = "led pulse",
            .name_func = led_pulse,
    };
    t_command_config cc4 = {
            .command = "set time",
            .name_func = set_time,
    };
    t_command_config cc5 = {
            .command = "log",
            .name_func = print_log_data_dht11,
    };
    t_command_config cc6 = {
            .command = "set alarm",
            .name_func = set_alarm,
    };
    t_command_config cc7 = {
            .command = "play on",
            .name_func = play_on,
    };
    t_command_config cc8 = {
            .command = "play off",
            .name_func = play_off,
    };

    command_regist(&cc0, commands);
    command_regist(&cc1, commands);
    command_regist(&cc2, commands);
    command_regist(&cc3, commands);
    command_regist(&cc4, commands);
    command_regist(&cc5, commands);
    command_regist(&cc6, commands);
    command_regist(&cc7, commands);
    command_regist(&cc8, commands);
}