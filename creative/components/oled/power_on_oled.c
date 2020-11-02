#include "oled.h"

void power_on_oled() {
    gpio_set_direction(GPIO_EN_OLED, GPIO_MODE_OUTPUT);
    gpio_set_level(GPIO_EN_OLED, 1);
    sleep(1);
}