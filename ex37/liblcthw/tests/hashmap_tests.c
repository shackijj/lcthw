#include "minunit.h"
#include <lcthw/hashmap.h>
#include <assert.h>
#include <lcthw/bstrlib.h>

#define ELEMENTS 100

Hashmap *map = NULL;
static int traverse_called = 0;
struct tagbstring test1 = bsStatic("test data 1");
struct tagbstring test2 = bsStatic("test data 2");
struct tagbstring test3 = bsStatic("test data 3");
struct tagbstring expect1 = bsStatic("THE VALUE 1");
struct tagbstring expect2 = bsStatic("THE VALUE 2");
struct tagbstring expect3 = bsStatic("THE VALUE 3");

bstring keyArray[ELEMENTS];
bstring valueArray[ELEMENTS];

void init_arrays()
{
    int i = 0;

    for(i = 0; i < ELEMENTS; i++) {
        keyArray[i] = (bstring) bformat("test key %d", i);
        valueArray[i] =  (bstring) bformat("test value %d", i);
    }
}

void destroy_arrays()
{
    int i = 0;
    for (i = 0; i < ELEMENTS; i++) {
        bdestroy(keyArray[i]);
        bdestroy(valueArray[i]);
    }
}

static int traverse_good_cb(HashmapNode *node)
{
    debug("KEY: %s", bdata((bstring)node->key));
    traverse_called++;
    return 0;
}

static int traverse_debug(HashmapNode *node)
{
    debug("KEY: %s, HASH %zu", bdata((bstring)node->key), node->hash);
    return 0;
}

static int traverse_fail_cb(HashmapNode *node)
{
    debug("KEY: %s", bdata((bstring)node->key));
    traverse_called++;
    if (traverse_called == 2) {
        return 1;
    } else {
        return 0;
    }
}

char *test_create()
{
    map = Hashmap_create(NULL, NULL);
    mu_assert(map != NULL, "Failed to create map.");

    return NULL;
}

char *test_destroy()
{
    Hashmap_destroy(map);
    return NULL;
}

char *test_get_set()
{
    int rc = Hashmap_set(map, &test1, &expect1);
    mu_assert(rc == 0, "Failed to set &test1");
    bstring result = Hashmap_get(map, &test1);
    mu_assert(result == &expect1, "Wrong value for test3.");
   
    rc = Hashmap_set_new(map, &test1, &expect2);
    mu_assert(rc == -1, "Failed test1 rewrited");
    result = Hashmap_get(map, &test1);
    mu_assert(result == &expect1, "Wrong value for test1.");
    
    rc = Hashmap_set(map, &test2, &expect2);
    Hashmap_traverse(map, traverse_debug);
    log_info("At test2---------");
    mu_assert(rc == 0, "Fasiled to set &test2");
    log_info("%s result", bdata(result));
    result = Hashmap_get(map, &test2);
    mu_assert(result == &expect2, "Wrong value for test2.");

    rc = Hashmap_set(map, &test3, &expect3);
    Hashmap_traverse(map, traverse_debug);
    mu_assert(rc == 0, "Fasiled to set &test3");
    result = Hashmap_get(map, &test3);
    mu_assert(result == &expect3, "Wrong value for test3.");

    return NULL;
}

char *test_traverse()
{
    int rc = Hashmap_traverse(map, traverse_good_cb);
    mu_assert(rc == 0, "Failed to traverse.");
    mu_assert(traverse_called == 3, "Wrong count traverse.");

    traverse_called = 0;
    rc = Hashmap_traverse(map, traverse_fail_cb);
    mu_assert(rc == 1, "Failed to traverse");
    mu_assert(traverse_called == 2, "Wrong count traverse for fail.");

    return NULL;
}

char *test_delete()
{
    bstring deleted = (bstring)Hashmap_delete(map, &test1);
    mu_assert(deleted != NULL, "Got NULL on delete test1.");
    mu_assert(deleted == &expect1, "Should get test1");
    bstring result = Hashmap_get(map, &test1);
    mu_assert(result == NULL, "Should delete");

    deleted = (bstring)Hashmap_delete(map, &test2);
    mu_assert(deleted != NULL, "Got NULL on delete test2.");
    mu_assert(deleted == &expect2, "Should get test2");
    result = Hashmap_get(map, &test2);
    mu_assert(result == NULL, "Should delete");

    deleted = (bstring)Hashmap_delete(map, &test3);
    mu_assert(deleted != NULL, "Got NULL on delete test3.");
    mu_assert(deleted == &expect3, "Should get test3");
    result = Hashmap_get(map, &test3);
    mu_assert(result == NULL, "Should delete");

    return NULL;
}

char *test_performance()
{
    int i = 0;

    for (i = 0; i < ELEMENTS; i++) {
        Hashmap_set(map, keyArray[i], valueArray[i]);     
    }

    return NULL;
}

char *all_tests()
{
    mu_suite_start();

    mu_run_test(test_create);
    mu_run_test(test_get_set);
    mu_run_test(test_traverse);
    mu_run_test(test_delete);
    mu_run_test(test_destroy);

    init_timer();
    init_arrays();
    mu_run_test(test_create);
    time_it_with_args(mu_run_test(test_performance), "Hashmap set");
    mu_run_test(test_destroy); 
    destroy_arrays();

    return NULL;
}

RUN_TESTS(all_tests);
