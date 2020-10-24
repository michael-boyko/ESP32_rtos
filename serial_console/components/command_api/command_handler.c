#include "command_api.h"

static t_commands_queue *mx_create_node_queue(t_func *data) {
    t_commands_queue *list = malloc(sizeof(t_commands_queue));

    list->data = data;
    list->next = NULL;
    return list;
}

static void mx_push_back_queue(t_commands_queue **list, t_func *data) {
    t_commands_queue *new_node = mx_create_node_queue(data);
    t_commands_queue *s = *list;

    if (s == NULL) {
        *list = new_node;
    } else {
        while(s->next != NULL) {
            s = s->next;
        }
        s->next = new_node;
    }
}

static int str_lenght(char **arr, int size, int position) {
    int len = 0;

    for (int i = position; i < size; i++) {
        len += strlen(arr[i]) + 1;
    }
    return len -1;
}

static char *str_concatenation(char **arr, int size, int position) {
    int len = str_lenght(arr, size, position);
    char *str = mx_strnew(len);

    for (int i = position + 1; i < size; i++) {
        str = strcat(str, arr[i]);
        if (i + 1 < size) {
            str = strcat(str, " ");
        }
    }
    if (str[0] == 0) {
        free(str);
        str = NULL;
    }
    return str;
}

char *command_handler(char *str, t_pars_tree **commands) {
    int i = 0;
    int amount_words = 0;
    char *error = NULL;
    char **arr_command = mx_strsplit(str, ' ');
    t_pars_tree *command = NULL;
    t_commands_queue *commands_queue = NULL;
    t_func *func = NULL;

    if (arr_command != NULL && commands != NULL) {
        command = commands[(int)arr_command[0][0]];
        amount_words = mx_count_words(str, ' ');
        while (command->command != NULL && arr_command[i] != NULL) {
            if (mx_strcmp(command->command, arr_command[i]) == 0) {
                if (command->name_func != NULL) {
                    func = (t_func *) malloc(sizeof(t_func *));
                    func->parameter = str_concatenation(arr_command, amount_words, i);
                    func->name_func = command->name_func;
                    mx_push_back_queue(&commands_queue, (void *)func);
                    command = command->subsidiary;
                    i++;
                } else {
                    command = command->subsidiary;
                    i++;
                }
            }
            else {
                while (mx_strcmp(command->command, arr_command[i]) != 0 && command->command != NULL) {
                    command = command->next;
                }
            }
        }
    }
    if (commands_queue != NULL) {                  //need to add arr error for return when more then 1 comand
        while (commands_queue != NULL) {
            error = commands_queue->data->name_func(commands_queue->data->parameter);
            free(commands_queue->data->parameter); // need to check leacks
            commands_queue = commands_queue->next;
        }
        free(commands_queue);
    }
    else {
        error = mx_strnew(strlen("command not found"));
        error = strcat(error, "command not found");
    }
    mx_del_strarr(&arr_command);

    return error;
}
