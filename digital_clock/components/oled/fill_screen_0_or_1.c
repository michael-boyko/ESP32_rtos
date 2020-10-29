#include "oled.h"

void fill_screen_0_or_1(uint8_t **screen, _Bool on_off) {
    for (uint8_t i = 0; i < 8; i++) {
        for (uint8_t j = 0; j < 128; j++) {
            if (on_off == true)
                screen[i][j] = 0xFF;
            else
                screen[i][j] = 0x00;
        }
    }
}
