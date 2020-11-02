#include "oled.h"

uint8_t **create_arr() {
    uint8_t **arr = malloc(sizeof(uint8_t *) * 8);
    
    for (int i = 0; i < 8; i++) {
        arr[i] = malloc(sizeof(uint8_t) * 128);
    }
    return arr;
}
