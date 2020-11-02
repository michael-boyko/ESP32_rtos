#include "oled.h"

void screen_onto_display(uint8_t **screen) {
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    uint8_t arr[8][128];

    for (int i = 0; i < 8; ++i) //need to change
        for (int j = 0; j < 128; ++j)
            arr[i][j] = screen[i][j];

    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (DISPLAY_ADDR << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write_byte(cmd, 0x80, true); // single command
    i2c_master_write_byte(cmd, 0xB0, true);
    i2c_master_write_byte(cmd, 0x40, true); // data stream
    i2c_master_write(cmd, *arr, 128 * 8, true);
    i2c_master_stop(cmd);
    i2c_master_cmd_begin(DISPLAY_PORT, cmd, 10 / portTICK_PERIOD_MS);
    i2c_cmd_link_delete(cmd);
}
