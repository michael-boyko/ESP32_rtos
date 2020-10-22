#include "mylib.h"

void mx_del_strarr(char ***arr) {
    char **p = *arr;

    while (*p != NULL) {
        mx_strdel(p);
        p++;
    }
    free(*arr);
    *arr = NULL;
}