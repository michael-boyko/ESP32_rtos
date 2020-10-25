#include "monitor.h"
#include "stdio.h"

void ts() {
    TickType_t x = 0;

    while (true) {
        x = xTaskGetTickCount();
        printf("%d\n", x / 100);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }

}

void app_main() {
    xTaskCreate(ts, "ts", 4208, NULL, 10, NULL);
}