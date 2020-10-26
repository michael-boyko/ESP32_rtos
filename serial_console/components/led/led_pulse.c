#include "led.h"

void led_pulse(char *argv) {
    ledc_timer_config_t led_timer = {
            .speed_mode = LEDC_HIGH_SPEED_MODE,
            .timer_num = LEDC_TIMER_0,
            .freq_hz = 50,
            .duty_resolution = LEDC_TIMER_8_BIT,
            .clk_cfg = LEDC_AUTO_CLK,
        };
    ledc_channel_config_t channel = {
            .channel    = LEDC_CHANNEL_0,
            .duty       = 0,
            .gpio_num   = GPIO_LED1,
            .speed_mode = LEDC_HIGH_SPEED_MODE,
            .hpoint     = 0,
            .timer_sel  = LEDC_TIMER_0
        };

    ledc_timer_config(&led_timer);
    ledc_channel_config(&channel);
    ledc_fade_func_install(0);
    ledc_set_fade_with_time(channel.speed_mode, channel.channel, 128, 100);
}