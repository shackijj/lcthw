#include <lcthw/list_algos.h>
#include <lcthw/dbg.h>

void List_swap(ListNode *a, ListNode *b) {

    char *b_tmp_value = b->value;
    b->value = a->value;
    a->value = b_tmp_value; 
    return;
} 

int List_bubble_sort(List *list, List_compare cmp)
{
    int length = List_count(list);
    int swapped = 1;
    int count;

    while (swapped) {

        swapped = 0;
        count = 1;

        LIST_FOREACH(list, first, next, cur) {

            if (count == length) break;

            if (cur->next && cmp(cur->value, cur->next->value) > 0) {
                List_swap(cur, cur->next);
                swapped = 1; 
            }
 
            count++;
        }

        length--;
    }
    return 0;        
}

List *List_merge_sort(List *list, List_compare cmp)
{
    int length = List_count(list);
    List *result;   

    if (length <= 1) {
        result = List_create();
        List_join(result, list);
        return result;
    }

    List *left = List_create();
    List *right = List_create();

    if (length <= 1) return list;
    
    int middle = length / 2;

    LIST_FOREACH(list, first, next, cur) {
        if (middle > 0) {
            List_push(left, cur->value);
            middle--;
        } else {
            List_push(right, cur->value);
        }
    }
    
    List *left_result = List_merge_sort(left, cmp);
    List *right_result = List_merge_sort(right, cmp);
    result = List_merge(left_result, right_result, cmp);

    List_destroy(left);
    List_destroy(right);
    List_destroy(left_result);
    List_destroy(right_result);        

    return result; 
}

List *List_merge(List *left, List *right, List_compare cmp)
{
    List *result = List_create();
    
    while(List_count(left) > 0 && List_count(right) > 0) {

        if (cmp(right->first->value, left->first->value) > 0) {
            List_push(result, List_shift(left));
        } else {
            List_push(result, List_shift(right));
        }
    }   

    if (List_count(left) > 0) List_join(result, left);
    if (List_count(right) > 0) List_join(result, right);

    return result;
}
