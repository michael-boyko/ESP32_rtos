#include "functional.h"

void play_on(char *argv) {
    int alarma = 1;

    if (argv == NULL) {
        i2s_start(0);
        xQueueSend(alarm_queue, &alarma, ( TickType_t ) 0);
    } else {
        error_output(argv);
    }
}