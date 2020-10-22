#include "readuart.h"

static void del_symbol_inside_str(char *str, int position) {
    int len = strlen(str);

    for (int i = position; i <= len; i++) {
        str[i - 1] = str[i];
    }
}

static void add_symbol_inside_str(char *str, int position, char c) {
    char tmp = '\0';
    int len = strlen(str);

    for (int i = position; i <= len; i++) {
        tmp = str[i];
        str[i] = c;
        c = tmp;
    }
}

static void esc_to_do(uint8_t *buf, t_flag *f) {
    if (buf[2] == 'D' && f->position > 0) {
        uart_write_bytes(UART_NUM, (char *)buttons.left, 1);
        f->position--;
    }
    else if (buf[2] == 'C' && f->position < f->count_str_size) {
        uart_write_bytes(UART_NUM, (char *)buttons.right, 3);
        f->position++;
    }
}

static void data_to_do(char *str, uint8_t *buf, t_flag *f, int read) {
    if (f->position != f->count_str_size) {
        if (read == 1) {
            uart_write_bytes(UART_NUM, (char *)insert_one_space, 3);
            uart_write_bytes(UART_NUM, (char *)buf, read);
            add_symbol_inside_str(str, f->position, (char)buf[0]);
            f->count_str_size += read;
            f->position++;
        } else {
            for (int i = 0; i < read; i++) {
                uart_write_bytes(UART_NUM, (char *)insert_one_space, 3);
                uart_write_bytes(UART_NUM, (const char *) &(buf[i]), 1);
                add_symbol_inside_str(str, f->position, buf[i]);
                f->count_str_size++;
                f->position++;
            }
        }
    } else {
        strcat(str, (char *)buf);
        uart_write_bytes(UART_NUM, (char *)buf, read);
        f->count_str_size += read;
        f->position = f->count_str_size;
    }
}

static void enter_to_do(char *str, t_flag *f) {
    uart_write_bytes(UART_NUM, (char *)buttons.enter, 4);
    uart_flush(UART_NUM);
    if (strcmp("clear", str) == 0) {
        uint8_t clear[8] = {27, '[', '2', 'J', 27, '[', 'H', '>'};
        uart_write_bytes(UART_NUM, (char *)clear, 8);
    }
    memset(str, '\0', strlen(str));
    f->position = 0;
    f->count_str_size = 0;
}

static void backspace_to_do(char *str, t_flag *f, int buf_size) {
    uint8_t backspace[4] = {0x08, 27, '[', 'P'}; // true backspace

    if (f->position == f->count_str_size && f->position != 0) {
        uart_write_bytes(UART_NUM, (char *)buttons.backspace, 3);
        f->count_str_size -= 1;
        f->position = f->count_str_size;
        str[f->count_str_size] = '\0'; //delete last symbol from str
    } else if (f->position != 0) {
        uart_write_bytes(UART_NUM_1, (char *)backspace, 4);
        del_symbol_inside_str(str, f->position);
        f->count_str_size--;
        f->position--;
    }
}

void uart_data_handler(char *str, t_flag *f) {
    uint8_t *buf = NULL;
    int read = 0;
    size_t buf_size = 0;

    uart_get_buffered_data_len(UART_NUM, &buf_size);
    buf = malloc(sizeof(uint8_t) * (buf_size + 1));
    memset(buf, '\0', buf_size + 1);
    read = uart_read_bytes(UART_NUM, buf, buf_size + 1, 1);

    switch (buf[0]) {
        case 27:
            esc_to_do(buf, f);
            break;
        case 13:
            enter_to_do(str, f);
            break;
        case 127:
            backspace_to_do(str, f, read);
            break;
        default:
            data_to_do(str, buf, f, read);
    }
    free(buf);
    buf = NULL;
}

void uart_event_handler() {
    uart_event_t event;
    char str[1024];
    t_flag f = {0, 0};

    memset(str, '\0', 1024);
    while (true) {
        if (xQueueReceive(uart0_queue, (void * )&event, (portTickType)portMAX_DELAY)) {
            if (event.type == UART_DATA) {
                uart_data_handler(str, &f);
                printf("str = %d  pos = %d\n", f.count_str_size, f.position);
            }
            if (event.type == UART_BREAK) {
                printf("UART_BREAk------------------------------\n");
            }
            if (event.type == UART_PATTERN_DET) {
                printf("UART_PATTERN_DET------------------------------\n");
            }
            if (event.type == UART_EVENT_MAX) {
                printf("UART_EVENT_MAX------------------------------\n");
            }
        }
    }
    vTaskDelete(NULL);
}