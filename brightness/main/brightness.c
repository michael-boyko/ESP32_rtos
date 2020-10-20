#include <limits.h>
#include "brightness.h"

_Noreturn void change_brightness_oled() {
     double brightnes = (double) adc1_get_raw(ADC1_CHANNEL_0);;
     double tmp = brightnes;

     adc1_config_channel_atten(ADC1_CHANNEL_0, ADC_ATTEN_DB_0 );
     while (true) {
         brightnes = (double) adc1_get_raw(ADC1_CHANNEL_0);
         brightnes = (brightnes /  4080 * 255);
         if (brightnes > (tmp + 25) || brightnes < (tmp - 25)) {
             reinit_display((uint8_t) brightnes);
             tmp = brightnes;
         }
         printf("%d\n",(int) brightnes);
         vTaskDelay(100 / portTICK_PERIOD_MS);
     }
     vTaskDelete(NULL);
 }

void app_main(void) {
    uint8_t **screen = create_arr();

    power_on_oled();
    init_i2c();
    init_display();
    fill_screen_0_or_1(screen, 1);
    screen_onto_display(screen);

    xTaskCreate(change_brightness_oled, "change_brightness_oled", 4096, NULL, 10, NULL);
}
