idf_component_register(SRCS "command_handler.c" "command_register.c" "create_arr_commands.c" "command_api.h"
                       INCLUDE_DIRS "."
                       REQUIRES mylib
                      )