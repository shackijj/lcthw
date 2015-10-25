#include <lcthw/list_algos.h>
#include <lcthw/dbg.h>

void List_swap(List *list, ListNode *a, ListNode *b) {

    /*
    if (list->first == a) { 
        log_info("I DID CHANGE! list->first before: %p", list->first); 
                                
        list->first = b;
        log_info("list->first after: %p", list->first);
    }
    if (list->last == b) list->last = a;
    */

    char *b_tmp_value = b->value;
    // Swap values
    b->value = a->value;
    a->value = b_tmp_value;
    
    return;
} 

int List_bubble_sort(List *list, List_compare cmp)
{
    int swapped = 1;
    char *tmp;
    while (swapped) {

        swapped = 0;

        LIST_FOREACH(list, first, next, cur) {
            log_info("Cur: %s", cur->value);
            if (cmp(cur->value, cur->next->value) > 0) {
                log_info("Before %s %s", cur->value, cur->next->value);
                List_swap(list, cur, cur->next);
                log_info("After %s %s", cur->value, cur->prev->value);
                swapped = 1; 
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
