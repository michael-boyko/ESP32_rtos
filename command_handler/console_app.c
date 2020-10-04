#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

typedef void (*func) (void *data);


typedef struct  s_pars_tree {
    char  *command;
    int parameter;
    func name_func;
    struct s_pars_tree *next;
    struct s_pars_tree *subsidiary;
} t_pars_tree;

typedef struct s_command_config {
    char  *command;
    int parameter;
    func name_func;
} t_command_config;

struct s_func {
    char *parameter;
    func name_func;
};

typedef struct s_commands_queue {
    struct s_func *data;
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

t_commands_queue *mx_create_node_queue(void *data) {
    t_commands_queue *list = malloc(sizeof(t_commands_queue));

    list->data = data;
    list->next = NULL;
    return list;
}

void mx_push_back_queue(t_commands_queue **list, void *data) {
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
    return str;
}

void command_handler(char *str, t_pars_tree **commands) {
    int i = 0;
    int amount_words = 0;
    char **arr_command = mx_strsplit(str, ' ');
    t_pars_tree *command = NULL;
    t_commands_queue *commands_queue = NULL;
    struct s_func func = {NULL, NULL};

    if (arr_command != NULL && commands != NULL) {
        command = commands[arr_command[0][0]];
        amount_words = mx_count_words(str, ' ');
        while (command->command != NULL && arr_command[i] != NULL) {
            if (mx_strcmp(command->command, arr_command[i]) == 0) {
                if (command->name_func != NULL && command->parameter > 0 && (amount_words - i - 1) > 0) {
                    func.parameter = str_concatenation(arr_command, amount_words, i);
                    func.name_func = command->name_func;
                    mx_push_back_queue(&commands_queue, (void *)&func);
                    command = command->subsidiary;
                    i++;
                } else if (command->name_func != NULL && command->parameter == 0 && (amount_words - i - 1) == 0) {
                    mx_push_back_queue(&commands_queue, NULL);
                    i = amount_words;
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
    if (commands_queue != NULL) {
        while (commands_queue != NULL) {
            commands_queue->data->name_func(commands_queue->data->parameter);
            free(commands_queue->data->parameter);
            free(commands_queue);
            commands_queue = commands_queue->next;
        }
    }
    else {
        printf("command not found\n"); //change to true func
    }
    mx_free_arr(arr_command);
    //need add return
}

void print(void *str) {
    printf("%s\n", (char *)str);
}

t_pars_tree *mx_create_node_tree(t_command_config *cc) {
    t_pars_tree *list = (t_pars_tree *) malloc(sizeof(t_pars_tree));
    
    list->command = cc->command;
    list->parameter = cc->parameter;
    list->name_func = cc->name_func;
    list->next = NULL;
    list->subsidiary = NULL;
    return list;
}

void mx_push_back_tree_next(t_pars_tree **list, void *data) {
    t_pars_tree *new_list = mx_create_node_tree(data);
    t_pars_tree *tmp = *list;

    while(tmp->next != NULL) {
        tmp = tmp->next;
    }
    tmp->next = new_list;
}

void mx_push_back_tree_subsidiary(t_pars_tree **list, void *data) {
    t_pars_tree *new_list = mx_create_node_tree(data);
    t_pars_tree *tmp = *list;

    while(tmp->subsidiary != NULL) {
        tmp = tmp->subsidiary;
    }
    tmp->subsidiary = new_list;
}

void command_regist(t_command_config *config, t_pars_tree **commands) { //segmentation fault with similar command
    t_pars_tree *command = NULL;
    t_pars_tree *tmp = NULL;
    char **arr_command = mx_strsplit(config->command, ' ');
    int amount_words = mx_count_words(config->command, ' ');
    int i = 1;
    _Bool stop = 0;
    t_command_config cc = {NULL, 0, NULL};

    command = commands[arr_command[0][0]];

    while (amount_words >= i) {
        if (command->command == NULL) {
            command->command = arr_command[i - 1];
            mx_push_back_tree_subsidiary(&command, &cc);
            mx_push_back_tree_next(&command, &cc);
            tmp = command;
            command = command->subsidiary;
            i++;
        }
        else {
            while (mx_strcmp(command->command, arr_command[i - 1]) != 0 && command->next != NULL && stop == 0) {
                if (mx_strcmp(command->command, arr_command[i - 1]) == 0) {
                    stop = 1;
                }
                else {
                    command = command->next;
                }
            }
            if (mx_strcmp(command->command, arr_command[i - 1]) == 0) {
                free(arr_command[i - 1]);
                arr_command[i - 1] = NULL;
                command = command->subsidiary;
                i++;
            }
            else {
                while (command->next != NULL) {
                    command = command->next;
                }
                command->command = arr_command[i - 1];
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
    tmp->parameter = config->parameter;
    free(arr_command);
}

t_pars_tree *create_arr_commands() {
    for (int i = 0; i < 256; i++) {
        commands[i] = mx_create_node_tree(&ccc);
    }
}

int main() {
    t_command_config ccc = {NULL, 0, NULL};
    t_pars_tree *commands[256];               //create arr of commands
	

	t_command_config cc = {              //set command config 
        .command = "led on",
        .parameter = 1,
        .name_func = print,
    };
    t_command_config cc1 = {              //set command config 
        .command = "led off",
        .parameter = 1,
        .name_func = print,
    };
    t_command_config cc2 = {              //set command config 
        .command = "led pulse",
        .parameter = 1,
        .name_func = print,
    };
    t_command_config cc3 = {              //set command config 
        .command = "print i nt",
        .parameter = 1,
        .name_func = print,
    };
    // command_regist(&cc, NULL);
    command_regist(&cc1, commands);
    command_regist(&cc2, commands);
    command_regist(&cc3, commands);

    // printf("%s (next->) %s (subsidiary->) %s\n", commands['l']->command,
    //  commands['l']->next->command, commands['l']->subsidiary->command);
    // printf("%s (next->) %s (subsidiary->) %s\n", commands['l']->subsidiary->command,
    //  commands['l']->subsidiary->next->command, commands['l']->subsidiary->subsidiary->command);

	// command_handler("led on 1 0,1", commands);
    // command_handler("led pulse abra cadabra sym salabym!", commands);
    // command_handler("print it abra cadabra sym salabym!", commands);
    // command_handler("led off abra cadabra sym salabym!", commands);
    command_handler("prnt int abra cadabra sym salabym!", commands);
    command_handler("led on abra cadabra sym salabym!", commands);
    command_handler("print i n t abra cadabra sym salabym!", commands);
    command_handler(NULL, commands);
    // command_handler("led pulse 1 0,1", commands);
    // printf("%d\n", mx_strcmp(NULL, "led"));
	// system("leaks -q  test");
	return 0;
}




















