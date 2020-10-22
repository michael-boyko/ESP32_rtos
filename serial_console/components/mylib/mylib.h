#ifndef MYLIB_H
#define MYLIB_H

#include <stdlib.h>

char *mx_strnew(const int size);
int mx_count_words(const char *str, char c);
char **mx_strsplit(const char *str, char c);
void mx_strdel(char **str);
void mx_del_strarr(char ***arr);

#endif