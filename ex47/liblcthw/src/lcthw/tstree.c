#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <lcthw/dbg.h>
#include <lcthw/tstree.h>

static inline char *reverse_string(const char *string, size_t len)
{   
    char *rev_str = malloc(len + 1);
    int i = 0;
    int j = 0;

    for(i = (int) len - 1; i > -1 ; i--) {
        rev_str[j] = string[i];
        j++;   
    }

    rev_str[len] = '\0';

    return rev_str;       
}

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

TSTree *TSTree_insert_suffix(TSTree *node, const char *key, size_t len, void *value)
{
    char *revert_key = reverse_string(key, len);
    debug("Revert key is %s", revert_key);

    TSTree *result_node = TSTree_insert(node, revert_key, len, value);

    free(revert_key);
    return result_node;
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

void *TSTree_search_suffix(TSTree *root, const char *key, size_t len)
{
    char *revert_key = reverse_string(key, len);

    void *result_node = TSTree_search_prefix(root, revert_key, len);

    free(revert_key);
    
    return result_node;  
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
                    // bstrlib is kind of overkill here.
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
