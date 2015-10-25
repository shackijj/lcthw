#include <lcthw/list_algos.h>
#include <lcthw/dbg.h>

void List_swap(List *list, ListNode *a, ListNode *b) {

    char *b_tmp_value = b->value;
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
            if (cur->next && cmp(cur->value, cur->next->value) > 0) {
                List_swap(list, cur, cur->next);
                swapped = 1; 
            } 
        }
    }
    return 0;        
}

List *List_merge_sort(List *list, List_compare cmp)
{
    int counter = 0;
    int length = List_count(list);
    List *left = List_create();
    List *right = List_create();

    if (length <= 1) return list;
    
    int middle = length / 2;
    LIST_FOREACH(list, first, next, cur) {
        if (counter < middle) {
            List_push(left, cur->value);
        } else {
            List_push(right, cur->value);
        }
    }
    
    List *left_result = List_merge_sort(left, cmp);
    List *right_result = List_merge_sort(right, cmp);
    List_join(left_result, right_result); 
    // TODO
    return result; 
}

