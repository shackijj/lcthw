#include <lcthw/list.h>

#ifdef NSORTCHK
#define sort_check(M, ...)
#else
#define sort_check(M, CMP, ...) LIST_FOREACH(M, first, next, cur) { if (cur->next && CMP(cur->value, cur->next->value) > 0) \
{ printf("Unsorted list! %s %s\n", (char *) cur->value, (char *) cur->next->value); goto error; } }
#endif

#define min(a, b) \
    ({__typeof__ (a) _a = (a); \
      __typeof__(b) _b = (b); \
      _a > _b ? _b : _a;})

typedef int (*List_compare)(const void *a, const void *b);

void List_swap(ListNode *a, ListNode *b);

void List_insert_sorted(List *list, void *value, List_compare cmp);

int List_bubble_sort(List *list, List_compare cmp);

List *List_merge_sort(List *list, List_compare cmp);

List *List_merge(List *right, List *left, List_compare cmp);

void List_bottom_up_merge_sort(List *list, List_compare cmp);

void List_bottom_up_merge(List *A, int iLeft, int iRight, int iEnd, List *B, List_compare cmp);

void List_copy(List *B, List *A);
