#include "creative.h"

void task_get_data_from_dht11() {
    uint32_t adjustment = 0;
    TickType_t x = 0;
    uint8_t size = 0;
    int *arr = NULL;
    t_dht11 data_t_h = {0, 0, 0};
    t_dht11 tmp_data_t_h = {0, 0, 0};
    t_dht11 tmp = {0, 0, 0};

    while (true) {
        x = xTaskGetTickCount() - adjustment;
        size = uxQueueMessagesWaiting(dht_queue);
        vTaskDelay(5000 / portTICK_PERIOD_MS);
        xSemaphoreTake(xMutex, (portTickType)portMAX_DELAY);
        arr = get_data_dht11();
        data_t_h.temperature = arr[2];
        data_t_h.humidity = arr[0];
        if (size == 0) {
            data_t_h.time = x;
        } else {
            if (tmp_data_t_h.temperature != data_t_h.temperature ||
                tmp_data_t_h.humidity != data_t_h.humidity) {
                data_t_h.time = x;
            } else {
                data_t_h.time = 0;
            }
        }
        tmp_data_t_h.temperature = data_t_h.temperature;
        tmp_data_t_h.humidity = data_t_h.humidity;
        if (size == 60) {
            xQueueReceive(dht_queue,  &tmp,( TickType_t ) 0);
            xQueueSend(dht_queue,  &data_t_h,( TickType_t ) 0 );
        } else {
            xQueueSend(dht_queue,  &data_t_h,( TickType_t ) 0 );
        }
        xSemaphoreGive(xMutex);
        adjustment += 2;
        free(arr);
    }
    vTaskDelete(NULL);
}