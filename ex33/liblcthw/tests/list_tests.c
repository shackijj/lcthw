#include "minunit.h"
#include <lcthw/list.h>
#include <assert.h>

static List *list = NULL;
static List *second_list = NULL;
char *test1 = "test1 data";
char *test2 = "test2 data";
char *test3 = "test3 data";


char *test_create()
{
    list = List_create();
    mu_assert(list != NULL, "Failed to create list.");

    return NULL;
}

char *test_create_second()
{
    second_list = List_create();
    mu_assert(list != NULL, "Failed to create second_list.");

    return NULL;
}

char *test_destroy()
{
    List_destroy(list);

    return NULL;
}

char *test_destroy_second()
{
    List_destroy(second_list);
     
    return NULL;
}

char *test_push_pop()
{
    List_push(list, test1);
    mu_assert(List_last(list) == test1, "Wrong last value.");

    List_push(list, test2);
    mu_assert(List_last(list) == test2, "Wrong last value.");

    List_push(list, test3);
    mu_assert(List_last(list) == test3, "Wrong last value.");
    mu_assert(List_count(list) == 3, "Wrong count on push.");

    char *val = List_pop(list);
    mu_assert(val == test3, "Wrong value on pop.");
    
    val = List_pop(list);
    mu_assert(val == test2, "Wrong value on pop.");
    
    val = List_pop(list);
    mu_assert(val == test1, "Wrong value on pop.");

    return NULL;
}

char *test_unshift()
{
    List_unshift(list, test1);
    mu_assert(List_first(list) == test1, "Wrong first value.");
  
    List_unshift(list, test2);
    mu_assert(List_first(list) == test2, "Wrong first value.");

    List_unshift(list, test3);
    mu_assert(List_first(list) == test3, "Wrong first value.");

    mu_assert(List_count(list) == 3, "Wrong count on unshift.");

    return NULL;
}

char *test_remove()
{
    char *val = List_remove(list, list->first->next);
    mu_assert(val == test2, "Wrong removed element.");
    mu_assert(List_count(list) == 2, "Wrong count after remove.");
    mu_assert(List_first(list) == test3, "Wrong first after remove.");
    mu_assert(List_last(list) == test1, "Wrong last after remove.");

    return NULL;
}

char *test_join()
{
    List_push(second_list, test2);
    mu_assert(List_count(second_list) == 1, "Wrong count in second_list before join.");
    
    List_join(second_list, list);
    mu_assert(List_count(second_list) == 3, "Wrong count in second_list after joining.");
    mu_assert(List_first(second_list) == test2, "Wrong first value in second_list after joining.");
    mu_assert(List_last(second_list) == test1, "Wrong last value in second_list after joining.");  
    
    return NULL;
}



char *test_shift()
{
    mu_assert(List_count(list) != 0, "Wrong count before shift.");
    char *val = List_shift(list);
    mu_assert(val == test3, "Wrong value on shift.");
    
    val = List_shift(list);
    mu_assert(val == test1, "Wrong value on shift.");
    mu_assert(List_count(list) == 0, "Wrong count after shift.");

    mu_assert(List_count(second_list) == 3, "Wrong count before shift."); 
    val = List_shift(second_list);
    mu_assert(val == test2, "Wrong value on shift.");    

    val = List_shift(second_list);
    mu_assert(val == test3, "Wrong value on shift.");

    val = List_shift(second_list);
    mu_assert(val == test1, "Wrong value on shift.");

    return NULL;
}

char *test_split()
{

    list = List_create();
    List_push(list, test1);
    List_push(list, test2);
    List_push(list, test3);

    second_list = List_split(list, 2);
    mu_assert(List_count(second_list) == 2, "Wrong count on split function.");

    List_shift(list);
    List_shift(list);
    List_shift(list);
    List_destroy(list);

    list = second_list->first->value;
    char *val = List_shift(list);

    mu_assert(val == test1, "Wrong value after shift.");
    val = List_shift(list);
    mu_assert(val == test2, "Wrong value after shift.");    
 
    list = second_list->last->value;
    val = List_shift(list);
    mu_assert(val == test3, "Wrong value after shift.");

    List_destroy(second_list);

    return NULL;

}

char *all_tests() {

    mu_suite_start();

    mu_run_test(test_create);
    mu_run_test(test_create_second);
    mu_run_test(test_push_pop);
    mu_run_test(test_unshift);
    mu_run_test(test_remove);
    mu_run_test(test_join);
    mu_run_test(test_shift);
    mu_run_test(test_destroy);
    mu_run_test(test_destroy_second);
    mu_run_test(test_split);

    

    return NULL;
}

RUN_TESTS(all_tests);
