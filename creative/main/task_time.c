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

static void create_str_from_dht_data(char *str, t_dht11 *data_t_h) {
    sprintf(str, "t %dC   h %d%%", data_t_h->temperature, data_t_h->humidity);
}

void digital_clock() {
    uint8_t **screen = create_arr();
    uint32_t time_in_sec = 86395;
    uint32_t set_alarm = 86401;
    char *time_in_str = NULL;
    uint32_t t = 0;
    char str_dht[] = "t 23C   h 34%";
    t_dht11 data_t_h = {0, 0, 0};
    int alarma = 1;


    fill_screen_0_or_1((uint8_t **)screen, 0);
    while (1) {
        if (time_in_sec == 86400) {
            time_in_sec = 0;
        }
        xTaskNotifyWait(0, 0, &t, (portTickType) portMAX_DELAY);
        if (xQueueReceive(set_time_queue, &time_in_sec, ( TickType_t ) 0));
        if (xQueueReceive(dht_on_oled_queue, &data_t_h, ( TickType_t ) 0));
        if (xQueueReceive(set_alarm_queue, &set_alarm, ( TickType_t ) 0));

        if (time_in_sec == set_alarm) {
            xQueueSend(alarm_queue, &alarma, ( TickType_t ) 0);
        }
        create_str_from_dht_data(str_dht, &data_t_h);
        time_in_str = correct_time_format(time_in_sec);
        fill_screen_0_or_1(screen, 0);
        str_in_screen(time_in_str, screen, 1);
        if (data_t_h.temperature == 0 && data_t_h.humidity == 0) {
            str_in_screen("t --C   h --%", screen, 0);
        } else {
            str_in_screen(str_dht, screen, 0);
        }
        screen_onto_display(screen);
        free(time_in_str);
        time_in_sec++;
    }
    vTaskDelete(NULL);
}