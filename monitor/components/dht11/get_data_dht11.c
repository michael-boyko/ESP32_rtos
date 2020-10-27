#include "dht11.h"

static int get_level(int time, _Bool status) {
    int sec = 0;

    while (gpio_get_level(GPIO_DHT11) == status) {
        if (sec > time)
            return -1;
        sec++;
        ets_delay_us(1);
    }
    return sec;
}

static void set_up_dht11(int *check) {
    gpio_set_direction(GPIO_DHT11, GPIO_MODE_OUTPUT);
    gpio_set_level(GPIO_DHT11, 0);
    ets_delay_us(20000);
    gpio_set_level(GPIO_DHT11, 1);
    ets_delay_us(40);
    gpio_set_direction(GPIO_DHT11, GPIO_MODE_INPUT);

    *check = get_level(80, 0);
    if (*check <= 0)
        write(2, "ERROR: set_up_dht11 0\n", 22);
    *check = get_level(80, 1);
    if (*check <= 0)
        write(2, "ERROR: set_up_dht11 1\n", 22);
}

static void get_5_byte_dht11(int *arr_byte, int *check) {
    for (int bit = 1, byte = 0; bit < 41; bit++) {
        *check = get_level(50, 0);
        if (*check <= 0)
            write(2, "ERROR: get_5_byte_dht11 0\n", 26);
        *check = get_level(72, 1);
        if (*check <= 0)
            write(2, "ERROR: get_5_byte_dht11 1\n", 26);
        if (*check > 40) {
            arr_byte[byte] <<= 1;
            arr_byte[byte] += 1;
        }
        else
            arr_byte[byte] <<= 1;
        if (bit % 8 == 0)
            byte++;
    }
    if (arr_byte[4] != (arr_byte[0] + arr_byte[1] + arr_byte[2] + arr_byte[3])) {
        write(2, "ERROR: wrong checksum\n", 22);
    }
}

int *get_data_dht11() {
    int check = 0;
    int *arr_byte = malloc(sizeof(int) * 5);

    bzero(arr_byte, sizeof(int) * 5);
    set_up_dht11(&check);
    get_5_byte_dht11(arr_byte, &check);

    return arr_byte;
}