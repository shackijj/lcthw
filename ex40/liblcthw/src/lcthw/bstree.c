#include <lcthw/dbg.h>
#include <lcthw/bstree.h>
#include <stdlib.h>
#include <lcthw/bstrlib.h>

static int default_compare(void *a, void *b)
{
    return bstrcmp((bstring) a, (bstring) b);
}

static uint32_t default_hash(void *data)
{
    size_t len = blength((bstring)data);
    char *key = bdata((bstring)data);
    uint32_t hash = 0;
    uint32_t i = 0;

    for(hash = i = 0; i < len; ++i)
    {
        hash += key[i];
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }

    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);

    return hash;
}

BSTree *BSTree_create(BSTree_compare compare, BSTree_hash hash)
{
    // Why not malloc?
    BSTree *map = calloc(1, sizeof(BSTree));
    check_mem(map);

    map->compare = compare == NULL ? default_compare : compare;
    map->hash = hash == NULL ? default_hash : hash;

    return map;

error:
    if (map) {
        BSTree_destroy(map);
    }
    return NULL;
}

static int BSTree_destroy_cb(BSTreeNode *node)
{
    free(node);
    return 0;
}

void BSTree_destroy(BSTree *map)
{
    if(map) {
        BSTree_traverse(map, BSTree_destroy_cb);
        free(map);
    }
}

static inline BSTreeNode *BSTreeNode_create(BSTreeNode *parent, void *key, void *data, uint32_t hash)
{
    BSTreeNode *node = calloc(1, sizeof(BSTreeNode));
    check_mem(node);

    node->key = key;
    node->data = data;
    node->parent = parent;
    node->hash = hash;

    return node;

error:
    return NULL;
}

static inline void BSTree_setnode(BSTree *map, BSTreeNode *node, void *key, void *data, uint32_t hash)
{
    //int cmp = map->compare(node->key, key);

    if(node->hash <= hash) {
        if(node->left) {
            BSTree_setnode(map, node->left, key, data, hash);
        } else {
            node->left = BSTreeNode_create(node, key, data, hash);
        }
    } else {
        if(node->right) {
            BSTree_setnode(map, node->right, key, data, hash);
        } else {
            node->right = BSTreeNode_create(node, key, data, hash);
        }
    }
}

int BSTree_set(BSTree *map, void *key, void *data)
{
    check(map != NULL, "Map can't be NULL.");
    check(key != NULL, "Key can't be NULL.");

    uint32_t hash = map->hash(key);

    if(map->root == NULL) {
        map->root = BSTreeNode_create(NULL, key, data, hash);
        check_mem(map->root);
    } else {
        BSTree_setnode(map, map->root, key, data, hash);
    }

    return 0;

error:
    return -1;
}

static inline BSTreeNode *BSTree_getnode(BSTree *map, BSTreeNode *node, void *key, uint32_t hash)
{
    //int cmp = map->compare(node->key, key);

    if(node->hash == hash && map->compare(node->key, key) == 0) {
        return node;
    } else if (node->hash < hash) {
        if(node->left) {
            return BSTree_getnode(map, node->left, key, hash);
        } else {
            return NULL;
        }
    } else {
        if(node->right) {
            return BSTree_getnode(map, node->right, key, hash);
        } else {
            return NULL;
        }
    }
    
    return NULL;
}

void *BSTree_get(BSTree *map, void *key)
{
    check(map != NULL, "Map can't be NULL.");
    check(key != NULL, "Key can't be NULL.");

    uint32_t hash = map->hash(key);

    if(map->root == NULL) {
        return NULL;
    } else {
        BSTreeNode *node = BSTree_getnode(map, map->root, key, hash);
        return node == NULL ? NULL : node->data;
    }
error:
    return NULL;
}

static inline int BSTree_traverse_nodes(BSTreeNode *node, BSTree_traverse_cb traverse_cb)
{
    int rc = 0;

    if(node->left) {
        rc = BSTree_traverse_nodes(node->left, traverse_cb);
        if(rc != 0) return rc;
    }

    if(node->right) {
        rc = BSTree_traverse_nodes(node->right, traverse_cb);
        if(rc != 0) return rc;
    }

    return traverse_cb(node);
}

int BSTree_traverse(BSTree *map, BSTree_traverse_cb traverse_cb)
{
    if(map->root) {
        return BSTree_traverse_nodes(map->root, traverse_cb);
    }

    return 0;
}

static inline BSTreeNode *BSTree_find_min(BSTreeNode *node)
{
    while(node->left) {
        node = node->left;
    }

    return node;
}

static inline void BSTree_replace_node_in_parent(BSTree *map, BSTreeNode *node, BSTreeNode *new_value)
{
    if(node->parent) {
        if(node == node->parent->left) {
            node->parent->left = new_value;
        } else {
            node->parent->right = new_value;
        }
    } else {
        map->root = new_value;
    }

    if(new_value) {
        new_value->parent = node->parent;
    }
}

static inline void BSTree_swap(BSTreeNode *a, BSTreeNode *b)
{
    void *temp = NULL;
    uint32_t temp_hash = NULL;
    temp = b->key; b->key = a->key; a->key = temp;
    temp = b->data; b->data = a->data; a->data = temp;
    temp_hash = b->hash; b->hash = a->hash; a->hash = temp_hash;
}

static inline BSTreeNode *BSTree_node_delete(BSTree *map, BSTreeNode *node, void *key, uint32_t hash)
{
    //int cmp = map->compare(node->key, key);

    if(node->hash < hash) {
        if(node->left) {
            return BSTree_node_delete(map, node->left, key, hash);
        } else {
            //not found
            return NULL;
        }
    } else if (node->hash > hash) {
        if(node->right) {
            return BSTree_node_delete(map, node->right, key, hash);
        } else {
            // not found
            return NULL;
        }
    } else {
        if(node->left && node->right) {
            BSTreeNode *sucessor = BSTree_find_min(node->right);
            BSTree_swap(sucessor, node);
            
            BSTree_replace_node_in_parent(map, sucessor, sucessor->right);
            return sucessor;
        } else if(node->left) {
            BSTree_replace_node_in_parent(map, node, node->left);
        } else if(node->right) {
            BSTree_replace_node_in_parent(map, node, node->right);
        } else {
            BSTree_replace_node_in_parent(map, node, NULL);
        }

        return node;
    }
}

void *BSTree_delete(BSTree *map, void *key)
{
    void *data = NULL;
    uint32_t hash = map->hash(key);

    if(map->root) {
        BSTreeNode *node = BSTree_node_delete(map, map->root, key, hash);

        if(node) {
            data = node->data;
            free(node);
        }
    }

    return data;
}
