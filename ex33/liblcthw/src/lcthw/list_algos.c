#include <lcthw/list_algos.h>
#include <lcthw/dbg.h>

void List_copy(List *from, List *to)
{
    int counter = 0;
    ListNode *to_node;
    
    LIST_FOREACH(from, first, next, cur) {
        to_node = List_get_node(to, counter);
        if (to_node) {
            to_node->value = cur->value;   
        } else {
            break;
        }
        counter++;
    } 
     
    return;
}

void List_bottom_up_merge_sort(List *A, List_compare cmp)
{
    List *B = List_create();
    int length = List_count(A);
    int width, i;
    
    for(width = 1; width < length; width = 2 * width) {
        for(i = 0; i < length; i = i + (2 * width)) {
            List_bottom_up_merge(A, i, min(i + width, length), min(i + (2 * width), length), B, cmp);
        }
        List_copy(B, A);
    }

    List_destroy(B);
    return;
}

void List_bottom_up_merge(List *A, int iLeft, int iRight, int iEnd, 
   List *B, List_compare cmp)
{
    int i0 = iLeft;
    int i1 = iRight;
    int j;

    ListNode *Ai0, *Ai1, *Bj;

    for (j = iLeft; j < iEnd; j++) {

        Ai0 = List_get_node(A, i0);
        Ai1 = List_get_node(A, i1);
        Bj = List_get_node(B, j);
        if (!Bj) {
            List_push(B, NULL);
            Bj = B->last;
        }

        if (i0 < iRight && (i1 >= iEnd || cmp(Ai0->value, Ai1->value) < 0)) {
            Bj->value = Ai0->value;
            i0++;
        } else {
            Bj->value = Ai1->value;
            i1++;
        }
    }    

    return;
}

void List_swap(ListNode *a, ListNode *b) {

    char *b_tmp_value = b->value;
    b->value = a->value;
    a->value = b_tmp_value; 
    return;
} 

void List_insert_sorted(List *list, void *value, List_compare cmp)
{

    check(list != NULL, "List_insert_sorted got a NULL poineter instead of list.");
    check(cmp != NULL, "List_insert_sorted got a NULL as second arg, should be function.");    
    check(value != NULL, "List_insert_sorted got a NULL as value.");
    
    if (List_count(list) == 0) List_push(list, value);
    
    LIST_FOREACH(list, first, next, cur) {

        if (cmp(cur->value, value) >= 0) {
            
            if (cur == list->first) {
                List_unshift(list, value);
                return;
            }
            
            ListNode *node = calloc(1, sizeof(ListNode));
            check_mem(node);
            
            node->value = value;

            cur->prev->next = node;
            node->prev = cur->prev;
            node->next = cur;
            cur->prev = node;

            list->count++;

            return; 
        }
    } 
    // If value is biger thab other.
    List_push(list, value);

error:
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
