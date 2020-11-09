#include "functional.h"

static _Bool is_all_time_line_digit(char **arr) {
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; arr[i][j] != '\0'; ++j) {
            if (!isdigit(arr[i][j])) {
                return false;
            }
        }
    }
    return true;
}

static void check_correct_time_setting(char *argv, char **arr, int arr_size) {
    uint32_t time = 0;
    uint8_t hours = 0;
    uint8_t minutes = 0;
    uint8_t seconds = 0;

    if (arr_size == 3) {
        if (is_all_time_line_digit(arr) == true) {
            hours = atoi(arr[0]);
            minutes = atoi(arr[1]);
            seconds = atoi(arr[2]);
            if (hours < 24 && minutes < 60 && seconds < 60) {
                time = (hours * 3600 + minutes * 60 + seconds);
                xQueueSend(set_alarm_queue, &time, 0);
            } else {
                error_output(argv);
            }
        } else {
            error_output(argv);
        }
    } else {
        error_output(argv);
    }
}

void set_alarm(char *argv) {
    char **arr = NULL;

    if (argv != NULL) {
        if (strlen(argv) == 8) {
            arr = mx_strsplit(argv, ':');
            check_correct_time_setting(argv, arr, mx_count_words(argv, ':'));
        } else {
            error_output(argv);
        }
    } else {
        error_output("NULL");
    }
}