#include "brightness.h"

void change_brightness_oled(void *screen) {
    double brightnes = 0;
    int raw = 0;

    while (true) {
        adc2_get_raw(ADC2_CHANNEL_7, ADC_WIDTH_BIT_12, &raw);
        brightnes = (((double)raw) );
        reinit_display((uint8_t) brightnes / 10);
        printf("%d\n",(int) brightnes);
        vTaskDelay(100 / portTICK_PERIOD_MS);   
    }
    printf("afdafa\n");
}

// void change_brightness_oled(void *screen) {
//     double brightnes = 0;

//     while (true) {
//         brightnes = (double) adc1_get_raw(ADC1_CHANNEL_0);
//         brightnes = (brightnes /  4080 * 255);
//         reinit_display((uint8_t) brightnes);
//         printf("%d\n",(int) brightnes);
//         vTaskDelay(10 / portTICK_PERIOD_MS);	
//     }
//     printf("afdafa\n");
// }

void app_main(void) {
    uint8_t **screen = create_arr();

    power_on_oled();
    init_i2c();
    init_display();
    fill_screen_0_or_1(screen, 1);
    screen_onto_display(screen);

    xTaskCreate(change_brightness_oled, "change_brightness_oled", 4096, screen, 11, NULL);
}
