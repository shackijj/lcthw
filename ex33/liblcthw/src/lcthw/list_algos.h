#include <lcthw/list.h>

typedef int (*List_compare)(int a, int b);

void List_swap(List *list, ListNode *a, ListNode *b);

int List_bubble_sort(List *list, List_compare cmp);

List *List_merge_sort(List *list, List_compare cmp);
