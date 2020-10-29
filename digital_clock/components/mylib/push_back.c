#include "mylib.h"

void mx_push_back(t_list **list, void *data) {
    t_list *new_node = mx_create_node(data);
    t_list *l = *list;

    if (l == NULL) {
        *list = new_node;
    } else {
        while(l->next != NULL) {
            l = l->next;
        }
        l->next = new_node;
    }
}
