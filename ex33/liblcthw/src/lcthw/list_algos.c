#include <lcthw/list_algos.h>
#include <lcthw/dbg.h>

int List_bubble_sort(List *list, List_compare cmp)
{

/*
procedure bubbleSort( A : list of sortable items )
   n = length(A)
   repeat 
     swapped = false
     for i = 1 to n-1 inclusive do
       
       if A[i-1] > A[i] then
         swap( A[i-1], A[i] )
         swapped = true
       end if
     end for
   until not swapped
end procedure

*/
    int swapped = 1;
    char *tmp;
    while (swapped) {

        swapped = 0;

        LIST_FOREACH(list, first, next, cur) {
                        
            if (cur->next && cmp(cur->value, cur->next->value) > 0) {
                log_info("%s is more than %s. Swap", cur->value, cur->next->value);
                tmp_next = cur->next->value;
                cur->next->value = cur->value;
                cur->value = tmp;
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
