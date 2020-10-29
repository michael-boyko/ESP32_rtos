#include "oled.h"

void app_main() {
    uint8_t **screen = create_arr();
    char str[] = "Hello, World!";

    power_on_oled();
    init_i2c();
    init_display();
    fill_screen_0_or_1((char **)screen, 0);
    str_in_screen(str, screen);
    screen_onto_display(screen);
}
