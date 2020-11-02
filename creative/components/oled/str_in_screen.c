#include "oled.h"

static uint8_t *get_symbol_code(int num_char) {
    uint8_t *symbol = malloc(sizeof(uint8_t) * 6);

    int start = (num_char - 32) * 6;

    for (int i = 0; i < 6; ++i) {
        symbol[i] = font6x8[start + i];
    }
    return symbol;
}

//void str_in_screen(char *str, uint8_t **screen) {   //small
//    uint8_t *symbol = NULL;
//    int start_point = (128 - strlen(str) * 6) / 2;
//
//    for (int j = 0; str[j] != '\0'; ++j) {
//        symbol = get_symbol_code(str[j]);
//        for (int i = 0; i < 6; ++i) {
//            screen[3][start_point + (j * 6) + i] = symbol[i];
//        }
//        free(symbol);
//    }
//}

static void up_to(uint8_t part_symbol, uint8_t **screen, int start_point) {
    for (int i = 3; i >= 0; --i) {
        for (int j = 0; j < 2; ++j) {
            if ((part_symbol >> i & 1) == 0) {
                screen[2][start_point] <<= 1;
            }
            else {
                screen[2][start_point] <<= 1;
                screen[2][start_point] += 1;
            }
        }
    }
    for (int i = 7; i >= 4; --i) {
        for (int j = 0; j < 2; ++j) {
            if ((part_symbol >> i & 1) == 0) {
                screen[3][start_point] <<= 1;
            }
            else {
                screen[3][start_point] <<= 1;
                screen[3][start_point] += 1;
            }
        }
    }
}

void str_in_screen(char *str, uint8_t **screen) {   //big
    uint8_t *symbol = NULL;
    int start_point = (128 - strlen(str) * 12) / 2;

    for (int j = 0; str[j] != '\0'; ++j) {
        symbol = get_symbol_code(str[j]);
        for (int i = 0; i < 6; ++i) {
            up_to(symbol[i], screen, (start_point + (j * 6) + i));
            up_to(symbol[i], screen, (start_point + (j * 6) + i) + 1);
            start_point++;
        }
        free(symbol);
    }
}

