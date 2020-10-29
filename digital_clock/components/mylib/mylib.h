#ifndef MYLIB_H
#define MYLIB_H

#include <stdlib.h>
#include "stdio.h"
#include "string.h"
#include "ctype.h"
#include "unistd.h"

typedef struct s_list {
    void  *data;
    struct s_list *next;
} t_list;

char *mx_strnew(const int size);
int mx_count_words(const char *str, char c);
char **mx_strsplit(const char *str, char c);
void mx_strdel(char **str);
void mx_del_strarr(char ***arr);
int mx_strcmp(const char *s1, const char *s2);
t_list *mx_create_node(void *data);
void mx_push_back(t_list **list, void *data);
void mx_push_front(t_list **list, void *data);
void mx_pop_back(t_list **head);

#endif
