#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <lcthw/dbg.h>
#include <lcthw/tstree.h>

static inline TSTree *TSTree_insert_base(TSTree *root, TSTree *node,
    const char *key, size_t len, void *value)
{

    check(key != NULL, "Key can't be NULL");
    check(len > 0, "Len can't be 0");

    if(node == NULL) {
        node = (TSTree *) calloc(1, sizeof(TSTree));
        
        if(root == NULL) {
            root = node;
        }

        node->splitchar = *key;
    }

    if(*key < node->splitchar) {
        node->low = TSTree_insert_base(root, node->low, key, len, value);
    } else if (*key == node->splitchar) {
        if(len > 1) {
            node->equal = TSTree_insert_base(root, node->equal, key+1, len-1, value);
        } else {
            assert(node->value == NULL && "Duplicate insert into tst.");
            node->value = value;
        }
    } else {
        node->high = TSTree_insert_base(root, node->high, key, len, value);
    }

    return node;

error:
    return NULL;
}

TSTree *TSTree_insert(TSTree *node, const char *key, size_t len, void *value)
{
    return TSTree_insert_base(node, node, key, len, value);
}

void *TSTree_search(TSTree *root, const char *key, size_t len)
{
    check(root != NULL, "root can't be NULL.");

    TSTree *node = root;
    size_t i = 0;

    while(i < len && node) {
        if(key[i] < node->splitchar) {
            node = node->low;
        } else if (key[i] == node->splitchar) {
            i++;
            if(i < len) node = node->equal;
        } else {
            node = node->high;
        }
    }

    if(node) {
        return node->value;
    } else {
        return NULL;
    }

error:
    return NULL;
}

void *TSTree_search_prefix(TSTree *root, const char *key, size_t len)
{
    check(root != NULL, "root can't be NULL.");
    check(len <= strlen(key), "Length more than actual key length.");

    if(len == 0) return NULL;

    TSTree *node = root;
    TSTree *last = NULL;
    size_t i = 0;

    while(i < len && node) {
        if(key[i] < node->splitchar) {
            node = node->low;
        } else if(key[i] == node->splitchar) {
            i++;
            if (i < len) {
                if(node->value) last = node;
                node = node->equal;
            }
        } else {
            node = node->high;
        }
    }

    node = node ? node : last;

    while(node && !node->value) {
        node = node->equal;
    }

    return node ? node->value : NULL;

error:
    return NULL;
}

DArray *TSTree_collect(TSTree *root, const char *key, size_t len)
{
    check(key != NULL, "Key can't be NULL"); 

    if (len == 0) return 0;
    DArray *result = DArray_create(sizeof(void *), 10);
    
    TSTree *node = root;
    
    size_t i = 0;

    while(i < len && node) {
        if(key[i] < node->splitchar) {
            node = node->low;
        } else if(key[i] == node->splitchar) {
            i++;
            if (i <= len) {

                if(node->value) {
                    // We can use bstrlib instead strncpy but we know exactly size of 
                    // found_key and have check for avoiding Segfault.
                    char *found_key = malloc(i + 1);
                    strncpy(found_key, key, i);
                    found_key[i] = '\0';                    

                    DArray_push(result, found_key);
                }

                node = node->equal;

            } else {
                node = node->high;
            }
        }
    }
    
    return result;   

error:
    return NULL;            
}

void TSTree_traverse(TSTree *node, TSTree_traverse_cb cb, void *data)
{
    if(!node) return;

    if(node->low) TSTree_traverse(node->low, cb, data);

    if(node->equal) {
        TSTree_traverse(node->equal, cb, data);
    }

    if(node->high) TSTree_traverse(node->high, cb, data);

    if(node->value) cb(node->value, data);
}

void TSTree_destroy(TSTree *node)
{
    if(node == NULL) return;

    if(node->low) TSTree_destroy(node->low);

    if(node->equal) {
        TSTree_destroy(node->equal);
    }

    if(node->high) TSTree_destroy(node->high);

    free(node);
}            
