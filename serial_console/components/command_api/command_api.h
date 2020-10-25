#ifndef COMMANDAPI_H
#define COMMANDAPI_H

#include "../mylib/mylib.h"

typedef void (*func) (char *data);

typedef struct  s_pars_tree {
    func name_func;
    char  *command;
    struct s_pars_tree *next;
    struct s_pars_tree *subsidiary;
} t_pars_tree;

typedef struct s_command_config {
    char  *command;
    func name_func;
} t_command_config;

typedef struct s_func { // maybe need to concat two structs
    char *parameter;
    func name_func;
} t_func;

typedef struct s_commands_queue {
    t_func *data;
    struct s_commands_queue *next;
} t_commands_queue;

void command_regist(t_command_config *config, t_pars_tree **commands);
char *command_handler(char *str, t_pars_tree **commands);
t_pars_tree **create_arr_commands();

#endif