#include "dht11.h"

void init_dht11() {
    gpio_set_direction(GPIO_EN_DHT11, GPIO_MODE_OUTPUT);
    gpio_set_level(GPIO_EN_DHT11, 1);
}
