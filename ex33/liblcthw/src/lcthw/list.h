#ifndef lcthw_List_h
#define lcthw_List_h

#include <stdlib.h>

// ListNode is element of list, which has three params: next, prev (they allow navigation) and value.
struct ListNode;

typedef struct ListNode {
    struct ListNode *next;
    struct ListNode *prev;
    void *value;
} ListNode;
// List is a container for elements (ListNodes)
typedef struct List {
    int count;
    ListNode *first;
    ListNode *last;
} List;
// Create empty List
List *List_create();
// Destroy existing list and free all elements
void List_destroy(List *list);
// Returns number of elements
#define List_count(A) ((A)->count)
// Returns value of first element
#define List_first(A) ((A)->first != NULL ? (A)->first->value : NULL)
// Returns value of last element
#define List_last(A) ((A)->last != NULL ? (A)->last->value : NULL)
// Add element to the end of list.
void List_push(List *list, void *value);
// Remove the last element of list. Return it's value,
void *List_pop(List *list);
// Add element to the begginigs of list
void List_unshift(List *list, void *value);
// Remove the first element and return it's value.
void *List_shift(List *list);
// Remove element of list given by poiner. Return value
void *List_remove(List *list, ListNode *node);
// Loop over each element of the list.
#define LIST_FOREACH(L, S, M, V) ListNode *_node = NULL;\
    ListNode *V = NULL;\
    for(V = _node = L->S; _node != NULL; V = _node = _node->M)
// Join two list into one
void List_join(List *left, List *right);
// Split an array into arrays with size elements. The last element may contail less elements
List *List_split(List *list, int size);
#endif
