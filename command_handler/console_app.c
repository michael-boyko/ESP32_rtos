#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

typedef char *(*func) (char *data);

typedef struct  s_pars_tree {
    char  *command;
    func name_func;
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

int mx_strcmp(const char *s1, const char *s2) {
    if (s1 == NULL || s2 == NULL) {
        return -1;
    }

    while(*s1 != '\0') {
        if (*s1 != *s2) {
            return *s1 - *s2;
        }
        s1++;
        s2++;
    }
    return *s1 - *s2;
}

char *mx_strnew(const int size) {
    char *s = NULL;

    if (size > 0) {
        s = (char *) malloc(size + 1);
        for(int i = 0; i < size + 1; i++) {
            s[i] = '\0';
        }
    }
    return s;
}

int mx_count_words(const char *str, char c) {
    int count = 0;

    if (str == NULL) {
        return -1;
    }
    while(*str != '\0') {
        if (*str != c) {
            count++;
            while(*str != c && *str != '\0') {
                str++;
            }
            if (*str == '\0') {
                return count;
            }
        }
        str++;
    }
    return count;
}

int strlen_mod(const char *s, char c) {
    int count = 0;

    while(*s != c && *s != '\0') {
        count++;
        s++;
    }
    return count;
}
char **mx_strsplit(const char *s, char c) {
    int size = 0;
    int j = 0;
    char **arr = NULL;

    if (s == NULL) {
        return NULL;
    }
    size = mx_count_words(s, c);
    arr = malloc((size + 1) * sizeof(char *));
    for(int i = 0; i < size; i++) {
        j = 0;
        while (*s == c && *s != '\0') {
            s++;
        }
        arr[i] = mx_strnew(strlen_mod(s, c));
        while (*s != c && *s != '\0') {
            arr[i][j] = *s;
            s++;
            j++;
        }
    }
    arr[size] = NULL;
    return arr;
}

void mx_free_arr(char **arr) {
    if (arr != NULL) {
        for (int i = 0; arr[i] != NULL; i++) {
            free(arr[i]);
        }
        free(arr);
    }
}


/////////////////////////////////////////////////////////////////////////////////
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
    }
    else {
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
        command = commands[arr_command[0][0]];
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
    mx_free_arr(arr_command);

    return error;
}
/////////////////////////////////////////////////////////////////////////////////


char *print(char *str) {
    char *s = NULL;

    if (str == NULL) {
        s = mx_strnew(5);
        s = strcat(s, "Hello");
    }
    else {
        printf("%s\n", str);
    }
    return s;
}

char *led_on(char *str) {
    char *s = NULL;
    char **arr_command = mx_strsplit(str, ' ');
    int amount_words = mx_count_words(str, ' ');

    if (amount_words == 1) {
        printf("led on %d\n", atoi(arr_command[0]));
    } else {
        s = mx_strnew(strlen("parameters not found"));
        s = strcat(s, "parameters not found");
    }
    return s;
}


/////////////////////////////////////////////////////////////////////////////////
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

    command = commands[arr_command[0][0]];

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
//////////////////////////////////////////////////////////////////////////////////


t_pars_tree **create_arr_commands() {
    t_pars_tree **commands = (t_pars_tree **) malloc(sizeof(t_pars_tree **) * 256);
    t_command_config cc = {NULL, NULL};

    for (int i = 0; i < 256; i++) {
        commands[i] = mx_create_node_tree(&cc);
    }
    return commands;
}



int main() {
    char *error = NULL;
    t_pars_tree **commands = create_arr_commands();               //create arr of commands

	t_command_config cc = {              //set command config 
        .command = "led on",
        .name_func = led_on,
    };
    t_command_config cc1 = {              //set command config 
        .command = "led off",
        .name_func = print,
    };
    t_command_config cc2 = {              //set command config 
        .command = "led pulse",
        .name_func = print,
    };
    t_command_config cc3 = {              //set command config 
        .command = "led 0 1 2 3 4 5 6 7 8 9",
        .name_func = print,
    };
    t_command_config cc4 = {              //set command config 
        .command = "print",
        .name_func = print,
    };

    command_regist(&cc, commands);
    command_regist(&cc1, commands);
    command_regist(&cc2, commands);
    command_regist(&cc3, commands);
    command_regist(&cc4, commands);

	error = command_handler("led on 1 0,1", commands);
    command_handler("led pulse abra cadabra sym salabym!", commands);
    command_handler("print it abra cadabra sym salabym!", commands);
    command_handler("led off abra cadabra sym salabym!", commands);
    command_handler("led on Hi, my darling!", commands);
    command_handler("led off Good bye, my darling!", commands);
    command_handler("led pulse abra cadabra sym salabym!", commands);
    command_handler("print Hello World!", commands);
    // error = command_handler("led on 3", commands);
    if (error == NULL) {
        printf("everything is ok\n");
    }
    else {
        printf("%s\n", error);
    }

	// system("leaks -q  test");
	return 0;
}




















