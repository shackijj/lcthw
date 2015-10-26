#include <lcthw/list.h>

typedef int (*List_compare)(const void *a, const void *b);

void List_swap(ListNode *a, ListNode *b);

int List_bubble_sort(List *list, List_compare cmp);

List *List_merge_sort(List *list, List_compare cmp);

List *List_merge(List *right, List *left, List_compare cmp);
