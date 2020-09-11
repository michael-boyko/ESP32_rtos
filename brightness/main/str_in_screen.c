#include "oled.h"

static uint8_t *get_symbol_code(int num_char) {
    uint8_t *symbol = malloc(sizeof(uint8_t) * 6);
    int start = (num_char - 32) * 6;

    for (int i = 0; i < 6; ++i) {
        symbol[i] = font6x8[start + i];
    }
    return symbol;
}

void str_in_screen(char *str, uint8_t **screen) {
    uint8_t *symbol = NULL;
    int start_point = (128 - strlen(str) * 6) / 2;

    for (int j = 0; str[j] != '\0'; ++j) {
        symbol = get_symbol_code(str[j]);
        for (int i = 0; i < 6; ++i) {
            screen[3][start_point + (j * 6) + i] = symbol[i];
        }
        free(symbol);
    }
}

