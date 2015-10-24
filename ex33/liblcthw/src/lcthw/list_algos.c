#include <lcthw/list_algos.h>
#include <lcthw/dbg.h>

void List_swap(List *list, ListNode *a, ListNode *b) {

    if (list->first == a) list->first = b;
    if (list->last == b) list->last = a;

    char *b_tmp_value = b->value;
    ListNode *b_tmp_next = b->prev;

    b->value = a->value;
    a->value = b_tmp_value;
    a->next = b_tmp_next;
    b->next = a;
    


} 

int List_bubble_sort(List *list, List_compare cmp)
{
    int swapped = 1;
    char *tmp;
    while (swapped) {

        swapped = 0;

        LIST_FOREACH(list, first, next, cur) {
                        
            if (cur->next && cmp(cur->value, cur->next->value) > 0) {
                List_swap(list, cur, cur->next);
                swapped = 1; 
            } else {
                log_info("%s not swapped", cur->value);
            }
        }

        log_info("Swapped: %d", swapped);
    }
    return 0;        
}

List *List_merge_sort(List *list, List_compare cmp)
{
    return list;
}
