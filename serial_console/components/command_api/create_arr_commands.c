#include "command_api.h"

static t_pars_tree *mx_create_node_tree(t_command_config *cc) {
    t_pars_tree *list = (t_pars_tree *) malloc(sizeof(t_pars_tree));

    list->command = cc->command;
    list->name_func = cc->name_func;
    list->next = NULL;
    list->subsidiary = NULL;
    return list;
}

t_pars_tree **create_arr_commands() {
    t_pars_tree **commands = (t_pars_tree **) malloc(sizeof(t_pars_tree **) * 256);
    t_command_config cc = {NULL, NULL};

    for (int i = 0; i < 256; i++) {
        commands[i] = mx_create_node_tree(&cc);
    }
    return commands;
}
