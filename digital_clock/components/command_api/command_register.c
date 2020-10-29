#include "command_api.h"

static t_pars_tree *mx_create_node_tree(t_command_config *cc) {
    t_pars_tree *list = (t_pars_tree *) malloc(sizeof(t_pars_tree));

    list->command = cc->command;
    list->name_func = cc->name_func;
    list->next = NULL;
    list->subsidiary = NULL;
    return list;
}

static void mx_push_back_tree_next(t_pars_tree **list, void *data) {
    t_pars_tree *new_list = mx_create_node_tree(data);
    t_pars_tree *tmp = *list;

    while(tmp->next != NULL) {
        tmp = tmp->next;
    }
    tmp->next = new_list;
}

static void mx_push_back_tree_subsidiary(t_pars_tree **list, void *data) {
    t_pars_tree *new_list = mx_create_node_tree(data);
    t_pars_tree *tmp = *list;

    while(tmp->subsidiary != NULL) {
        tmp = tmp->subsidiary;
    }
    tmp->subsidiary = new_list;
}

void command_regist(t_command_config *config, t_pars_tree **commands) { // need add defend when empty config
    t_pars_tree *command = NULL;
    t_pars_tree *tmp = NULL;
    char **arr_command = mx_strsplit(config->command, ' ');
    int amount_words = mx_count_words(config->command, ' ');
    int i = 0;
    _Bool stop = 0;
    t_command_config cc = {NULL, NULL};

    command = commands[(int)arr_command[0][0]];

    while (amount_words > i) {
        if (command->command == NULL) {
            command->command = arr_command[i];
            mx_push_back_tree_subsidiary(&command, &cc);
            mx_push_back_tree_next(&command, &cc);
            tmp = command;
            command = command->subsidiary;
            i++;
        }
        else {
            while (mx_strcmp(command->command, arr_command[i]) != 0 && command->next != NULL && stop == 0) {
                if (mx_strcmp(command->command, arr_command[i]) == 0) {
                    stop = 1;
                }
                else {
                    command = command->next;
                }
            }
            if (mx_strcmp(command->command, arr_command[i]) == 0) {
                free(arr_command[i]);
                arr_command[i] = NULL;
                tmp = command;
                command = command->subsidiary;
                i++;
            }
            else {
                while (command->next != NULL) {
                    command = command->next;
                }
                command->command = arr_command[i];
                mx_push_back_tree_next(&command, &cc);
                mx_push_back_tree_subsidiary(&command, &cc);
                tmp = command;
                command = command->subsidiary;
                i++;
            }
        }
        stop = 0;
    }
    tmp->name_func = config->name_func;
    free(arr_command);
}