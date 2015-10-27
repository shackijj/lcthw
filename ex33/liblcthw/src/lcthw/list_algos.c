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
    check(list != NULL, "List_bubble_sort got a NULL pointer");
    check(cmp != NULL, "List_bubble_sort got a NULL as second arg, should be function");

    int n = List_count(list);
    int count;

    while (n > 1) {

        n = 1;
        count = 1;

        LIST_FOREACH(list, first, next, cur) {
             
            if (cur->next && cmp(cur->value, cur->next->value) > 0) {
                List_swap(cur, cur->next);
                n = count;
            }
 
            count++;
        }
    }

    sort_check(list, cmp);
    return 0;

error:
    return 1;        
}


List *List_merge_sort(List *list, List_compare cmp)
{
    check(list != NULL, "List_merge_sort got a NULL pointer.");
    check(cmp != NULL, "List_merge_sort got a NULL as second argument."); 
 
    int length = List_count(list);

    if (length <= 1) return list;

    List *left = List_create();
    List *right = List_create();
    
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

    if (left != left_result) List_destroy(left);
    if (right != right_result) List_destroy(right);
    
    return List_merge(left_result, right_result, cmp); 

error:
    return NULL;
}

List *List_merge(List *left, List *right, List_compare cmp)
{
    if (List_count(left) == 1) {
        if (cmp(right->first->value, left->first->value) > 0) {
            List_unshift(right, List_shift(left));
            List_destroy(left);
            return right;
        }
    }
   
    if (List_count(right) == 1) {
        if (cmp(left->first->value, right->first->value) > 0) {
            List_unshift(left, List_shift(right));
            List_destroy(right);
            return left;
        }
    }
    
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
    
    List_destroy(left);
    List_destroy(right);

    sort_check(result, cmp);
    return result;

error:
    return result;
}
