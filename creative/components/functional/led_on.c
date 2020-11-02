#include "functional.h"

void led_on(char *argv) {
    if (argv != NULL) {
        if (strlen(argv) == 1) {
            switch ((int)(argv[0] - 48)) {
                case 1:
                    gpio_set_direction(GPIO_LED1, GPIO_MODE_OUTPUT);
                    gpio_set_level(GPIO_LED1, 1);
                    break;
                case 2:
                    gpio_set_direction(GPIO_LED2, GPIO_MODE_OUTPUT);
                    gpio_set_level(GPIO_LED2, 1);
                    break;
                case 3:
                    gpio_set_direction(GPIO_LED3, GPIO_MODE_OUTPUT);
                    gpio_set_level(GPIO_LED3, 1);
                    break;
                default:
                    error_output(argv);
            }
        } else {
            error_output(argv);
        }
    } else {
        error_output("NULL");
    }
}