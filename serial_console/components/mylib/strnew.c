#include "mylib.h"

char *mx_strnew(const int size) {
    char *s = NULL;

    if (size > 0) {
        s =  (char *) malloc(sizeof(char) * (size + 1));
        if (s != NULL) {
            for(int i = 0; i < size + 1; i++) {
                s[i] = '\0';
            }
        }
    }
    return s;
}
