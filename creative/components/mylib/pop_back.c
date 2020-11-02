#include "mylib.h"

void mx_pop_back(t_list **head) {
    t_list *h = *head;

    if (h != NULL) {
        if (h->next == NULL) {
            free(h);
            *head = NULL;
        } else {
            while (h->next->next != NULL) {
                h = h->next;
            }
            free(h->next);
            h->next = NULL;
        }
    } else {
        head = NULL;
    }
}
