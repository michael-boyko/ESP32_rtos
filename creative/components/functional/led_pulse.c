#include "functional.h"

static _Bool is_argv1_correct(char *argv1) {
    _Bool correct = 1;

    while (*argv1 != 0) {
        if (!isdigit(*argv1)) {
            correct = 0;
        }
        argv1++;
    }
    return correct;
}

static _Bool is_argv2_correct(char *argv2) {
    _Bool correct = 1;
    int count = 0;

    while (*argv2 != 0) {
        if (!isdigit(*argv2) && *argv2 != '.') {
            correct = 0;
        }
        if (*argv2 == '.') {
            count++;
        }
        argv2++;
    }
    if (count > 1) {
        correct = 0;
    }
    return correct;
}

static void led_pulse_on(int num_led, double freq) {
    ledc_timer_config_t led_timer1 = {
            .speed_mode = LEDC_HIGH_SPEED_MODE,
            .timer_num = LEDC_TIMER_0,
            .freq_hz = (int)(50 * freq),
            .duty_resolution = LEDC_TIMER_8_BIT,
            .clk_cfg = LEDC_AUTO_CLK,
    };
    ledc_timer_config_t led_timer2 = {
            .speed_mode = LEDC_HIGH_SPEED_MODE,
            .timer_num = LEDC_TIMER_1,
            .freq_hz = (int)(50 * freq),
            .duty_resolution = LEDC_TIMER_8_BIT,
            .clk_cfg = LEDC_AUTO_CLK,
    };
    ledc_timer_config_t led_timer3 = {
            .speed_mode = LEDC_HIGH_SPEED_MODE,
            .timer_num = LEDC_TIMER_2,
            .freq_hz = (int)(50 * freq),
            .duty_resolution = LEDC_TIMER_8_BIT,
            .clk_cfg = LEDC_AUTO_CLK,
    };
    ledc_channel_config_t channel1 = {
            .channel    = LEDC_CHANNEL_0,
            .duty       = 0,
            .gpio_num   = GPIO_LED1,
            .speed_mode = LEDC_HIGH_SPEED_MODE,
            .hpoint     = 0,
            .timer_sel  = LEDC_TIMER_0
    };
    ledc_channel_config_t channel2 = {
            .channel    = LEDC_CHANNEL_1,
            .duty       = 0,
            .gpio_num   = GPIO_LED2,
            .speed_mode = LEDC_HIGH_SPEED_MODE,
            .hpoint     = 0,
            .timer_sel  = LEDC_TIMER_1
    };
    ledc_channel_config_t channel3 = {
            .channel    = LEDC_CHANNEL_2,
            .duty       = 0,
            .gpio_num   = GPIO_LED3,
            .speed_mode = LEDC_HIGH_SPEED_MODE,
            .hpoint     = 0,
            .timer_sel  = LEDC_TIMER_2
    };

    switch (num_led) {
        case 1:
            ledc_timer_config(&led_timer1);
            ledc_channel_config(&channel1);
            ledc_fade_func_install(0);
            ledc_set_fade_with_time(channel1.speed_mode, channel1.channel, 128, 100);
            break;
        case 2:
            ledc_timer_config(&led_timer2);
            ledc_channel_config(&channel2);
            ledc_fade_func_install(0);
            ledc_set_fade_with_time(channel2.speed_mode, channel2.channel, 128, 100);
            break;
        case 3:
            ledc_timer_config(&led_timer3);
            ledc_channel_config(&channel3);
            ledc_fade_func_install(0);
            ledc_set_fade_with_time(channel3.speed_mode, channel3.channel, 128, 100);
    }

}

void led_pulse(char *argv) {
    char **arr_argv = NULL;
    int argv1 = 0;
    double argv2 = 0.0;

    if (argv != NULL) {
        arr_argv = mx_strsplit(argv, ' ');
        if (mx_count_words(argv, ' ') == 2) {
            if (is_argv1_correct(arr_argv[0]) && is_argv2_correct(arr_argv[1])) {
                argv1 = atoi(arr_argv[0]);
                argv2 = atof(arr_argv[1]);
                if (argv1 > 0 && argv1 < 4 && argv2 > 0 && argv2 < 2) {
                    led_pulse_on(argv1, argv2);
                } else {
                    error_output(argv);
                }
            } else {
                error_output(argv);
            }
        } else {
            error_output(argv);
        }
    } else {
        error_output("NULL");
    }
}