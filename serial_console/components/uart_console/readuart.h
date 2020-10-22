#ifndef READUART_H
#define READUART_H

#include "../../main/console.h"

struct buttons
{
    uint8_t enter[4];
    uint8_t backspace[3];
    uint8_t left[3];
    uint8_t right[3];
};

static struct buttons buttons = {
        {27, '[', 'E', '>',}, // Enter
        {0x08, ' ', 0x08,},  // Backspace
        {0x08, '[', 'D',},  // Left button
        {27, '[', 'C',},  // Right button
};

static uint8_t insert_one_space[3] = { 27, '[', '@',}; // insert one space

void uart_event_handler();

#endif