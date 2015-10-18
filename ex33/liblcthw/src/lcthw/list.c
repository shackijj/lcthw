#include <lcthw/list.h>
#include <lcthw/dbg.h>

List *List_create()
{
    List *list = calloc(1, sizeof(List));
    check(list != NULL, "Memory error.");
    return list;
error:
    return NULL;
}

void List_clear_destroy(List *list)
{
    check(list != NULL, "List_clear_destroy got NULL pointer");

    LIST_FOREACH(list, first, next, cur) {
        if(cur->value) free(cur->value);
        if(cur->prev) free(cur->prev);
    }
    
    free(list->last);
    free(list);

error:
    return;
}

void List_push(List *list, void *value)
{
    ListNode *node = calloc(1, sizeof(ListNode));
    check_mem(node);

    node->value = value;

    if(list->last == NULL) {
        list->first = node;
        list->last = node;
    } else {
        list->last->next = node;
        node->prev = list->last;
        list->last = node;
    }

    list->count++;
error:
    return;
}

void *List_pop(List *list)
{
    ListNode *node = list->last;
    return node != NULL ? List_remove(list, node) : NULL;
}

void List_unshift(List *list, void *value)
{
    ListNode *node = calloc(1, sizeof(ListNode));
    check_mem(node);

    node->value = value;

    if(list->first == NULL) {
        list->first = node;
        list->last = node;
    } else {
        node->next = list->first;
        list->first->prev = node;
        list->first = node;
    }

    list->count++;

error:
    return;
}

void *List_shift(List *list)
{
    ListNode *node = list->first;
    return node != NULL ? List_remove(list, node) : NULL;
}

void *List_remove(List *list, ListNode *node)
{
    void *result = NULL;

    check(list->first && list->last, "List is empty");
    check(node, "node can't be NULL");

    if(node == list->first && node == list->last) {
        list->first = NULL;
        list->last = NULL;
    } else if(node == list->first) {
        list->first = node->next;
        check(list->first != NULL, "Invalid list, somehow got a first that is NULL");
        list->first->prev = NULL;
    } else if(node == list->last) {
        list->last = node->prev;
        check(list->last != NULL, "Invalid list, somehow got a last that is NULL");
        list->last->next = NULL;
    } else {
        ListNode *after = node->next;
        ListNode *before = node->prev;
        after->prev = before;
        before->next = after;
    }

    list->count--;
    check(list->count >= 0, "List->count can't be less than zero.");
    check((list->count > 0) ^ (list->first == NULL), "First can't be NULL whether count > 0");
    result = node->value;
    free(node);

error:
    return result;
} 

void List_join(List *left, List *right)
{
    check(left != NULL, "Destination list is NULL");
    check(right != NULL, "Source list is NULL");

    LIST_FOREACH(right, first, next, cur) {
         List_push(left, cur->value);
    }

error:
    return;
}

List *List_split(List *list, int size)
{
    List *result = List_create();
    List *sublist = List_create();
    int cnt = 0;

    LIST_FOREACH(list, first, next, cur) {
        if (cnt < size ) {
            List_push(sublist, cur->value);
            cnt++;
        } else {
            log_info("PUSH INTO RESULT");
            List_push(result, sublist);
            sublist = List_create();
            List_push(sublist, cur->value);
            cnt = 0;
        }
    }
    
    List_push(result, sublist);

    return result;   
}
