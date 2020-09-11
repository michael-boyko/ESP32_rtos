#include "brightness.h"

void reinit_display(uint8_t brightnes) {
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (DISPLAY_ADDR << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write_byte(cmd, 0x00, true);
    
    i2c_master_write_byte(cmd, 0x81, true);
    i2c_master_write_byte(cmd, brightnes, true);

    i2c_master_stop(cmd);
    i2c_master_cmd_begin(DISPLAY_PORT, cmd, 10 / portTICK_PERIOD_MS);
    i2c_cmd_link_delete(cmd);
}
