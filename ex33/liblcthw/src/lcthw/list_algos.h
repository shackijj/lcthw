#include <lcthw/list.h>

#ifdef NSORTCHK
#define sort_check(M, ...)
#else
#define sort_check(M, CMP, ...) LIST_FOREACH(M, first, next, cur) { if (cur->next && CMP(cur->value, cur->next->value) > 0) \
{ printf("Unsorted list! %s %s\n", (char *) cur->value, (char *) cur->next->value); goto error; } }
#endif
typedef int (*List_compare)(const void *a, const void *b);

void List_swap(ListNode *a, ListNode *b);

int List_bubble_sort(List *list, List_compare cmp);

List *List_merge_sort(List *list, List_compare cmp);

List *List_merge(List *right, List *left, List_compare cmp);
