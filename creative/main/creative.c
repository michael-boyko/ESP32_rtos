#include "creative.h"

static void IRAM_ATTR timer_group_isr(void *para) {
    xTaskNotifyFromISR(xTaskToNotify, 1, eNoAction, NULL);
    timer_group_clr_intr_status_in_isr(TIMER_GROUP_0, TIMER_0);
    timer_group_enable_alarm_in_isr(TIMER_GROUP_0, TIMER_0);
    portYIELD_FROM_ISR();
}

static void init_uart() {
    uart_config_t uart_config = {
            .baud_rate = 9600,
            .data_bits = UART_DATA_8_BITS,
            .parity    = UART_PARITY_DISABLE,
            .stop_bits = UART_STOP_BITS_1,
            .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
    };


    uart_driver_install(UART_NUM, 2048, 2048, 20, &uart0_queue, 0);
    uart_param_config(UART_NUM, &uart_config);

    uart_set_pin(UART_NUM, 17, 16, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    uart_pattern_queue_reset(UART_NUM, 20);
}

static void init_timer() {
    timer_config_t timer1 = {
            TIMER_ALARM_EN,
            TIMER_PAUSE,
            TIMER_INTR_LEVEL,
            TIMER_COUNT_UP,
            TIMER_AUTORELOAD_EN,
            16,
    };

    timer_init(TIMER_GROUP_0, TIMER_0, &timer1);

    timer_set_counter_value(TIMER_GROUP_0, TIMER_0,  0);
    timer_set_alarm_value(TIMER_GROUP_0, TIMER_0, 1 * (TIMER_BASE_CLK / 16));
    timer_enable_intr(TIMER_GROUP_0, TIMER_0);
    timer_isr_register(TIMER_GROUP_0, TIMER_0, timer_group_isr, NULL, ESP_INTR_FLAG_IRAM, NULL);
    timer_start(TIMER_GROUP_0, TIMER_0);
}

void app_main() {
    set_time_queue = xQueueCreate(1, sizeof(uint32_t));
    dht_queue =  xQueueCreate( 60, sizeof( t_dht11) );
    dht_on_oled_queue =  xQueueCreate( 1, sizeof( t_dht11) );
    alarm_queue =  xQueueCreate( 1, sizeof( int ) );
    set_alarm_queue =  xQueueCreate( 1, sizeof( uint32_t ) );
    xMutex = xSemaphoreCreateMutex();
    init_dht11();
    power_on_oled();
    init_timer();
    init_uart();
    init_i2c();
    init_display();

    xTaskCreate(task_get_data_from_dht11, "task_get_data_from_dht11", 12024, NULL, 10, NULL);
    xTaskCreate(uart_event_handler, "read_bytes_from_uart", 12004, NULL, 10, NULL);

    xTaskCreate(digital_clock, "digital_clock", 12004, NULL, 10, &xTaskToNotify);
    xTaskCreate(make_noise, "make_noise", 12004, NULL, 10, NULL);
}

