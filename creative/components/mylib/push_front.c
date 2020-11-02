#include "mylib.h"

void mx_push_front(t_list **list, void *data) {
    t_list *new_node = mx_create_node(data);
    t_list *l = *list;
    t_list *tmp = NULL;

    if (l == NULL) {
        *list = new_node;
    } else {
        tmp = l;
        *list = new_node;
        new_node->next = tmp;
    }
}
