#include "creative.h"

static char *add_0_to_str_time(uint8_t num) {
    char *c = mx_strnew(2);

    if (num < 10) {
        c[0] = '0';
        c[1] = num + 48;
    } else {
        itoa(num, c, 10);
    }

    return c;
}

static char *correct_time_format(uint32_t time_in_sec) {
    char *hours = NULL;
    char *minutes = NULL;
    char *seconds = NULL;
    char *str = mx_strnew(8);

    hours = add_0_to_str_time(time_in_sec / 3600);
    minutes = add_0_to_str_time((time_in_sec % 3600) / 60);
    seconds = add_0_to_str_time((time_in_sec % 3600) % 60);
    sprintf(str,"%s:%s:%s", hours, minutes, seconds);
    free(hours);
    free(minutes);
    free(seconds);

    return str;
}

void digital_clock() {
    uint8_t **screen = create_arr();
    uint32_t time_in_sec = 86365;
    char *time_in_str = NULL;
    uint32_t t = 0;

    fill_screen_0_or_1((uint8_t **)screen, 0);
    while (1) {
        if (time_in_sec == 86400) {
            time_in_sec = 0;
        }
        xTaskNotifyWait(0, 0, &t, (portTickType) portMAX_DELAY);
        if (xQueueReceive(set_time_queue, &time_in_sec, ( TickType_t ) 0));

        time_in_str = correct_time_format(time_in_sec);
        fill_screen_0_or_1((uint8_t **)screen, 0);
        str_in_screen((uint8_t **)time_in_str, screen);
        screen_onto_display(screen);
        free(time_in_str);
        time_in_sec++;
    }
    vTaskDelete(NULL);
}