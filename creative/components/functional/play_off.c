#include "functional.h"

void play_off(char *argv) {
    if (argv == NULL) {
        i2s_stop(0);
    } else {
        error_output(argv);
    }
}