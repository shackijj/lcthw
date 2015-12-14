#include "minunit.h"
#include <lcthw/string_algos.h>
#include <lcthw/bstrlib.h>
#include <time.h>
#include <lcthw/stats.h>

struct tagbstring IN_STR = bsStatic("I have ALPHA beta ALPHA and oranges ALPHA");
struct tagbstring ALPHA = bsStatic("ALPHA");
const int TEST_TIME = 1;
struct bstrList *list = NULL;
Stats *find_stats = NULL;
Stats *binstr_stats = NULL;
Stats *scan_stats = NULL;

#define NUM_REPEATS 10


char *test_find_and_scan()
{
    StringScanner *scan = StringScanner_create(&IN_STR);
    mu_assert(scan != NULL, "Failed to make the scanner.");

    int find_i = String_find(&IN_STR, &ALPHA);
    mu_assert(find_i > 0, "Failed to find 'ALPHA' in test string.");

    int scan_i = StringScanner_scan(scan, &ALPHA);
    mu_assert(scan_i > 0, "Failed to find 'ALPHA' with scan.");
    mu_assert(scan_i == find_i, "find and scan don't match");

    scan_i = StringScanner_scan(scan, &ALPHA);
    mu_assert(scan_i > find_i, "should find another ALPHA after the first");

    scan_i = StringScanner_scan(scan, &ALPHA);
    mu_assert(scan_i > find_i, "should find another ALPHA after the first");

    mu_assert(StringScanner_scan(scan, &ALPHA) == -1, "shouldn't find it");

    StringScanner_destroy(scan);

    return NULL;
}

void init_bstrList() 
{
    list = bsplit(&IN_STR, ' ');
    log_info("List mlen %d, list qty %d", list->mlen, list->qty);
    
}

void destroy_bstrList()
{
    if(list) {
       bstrListDestroy(list);
    }
}
    

char *test_binstr_performance()
{
    int i = 0;
    int found_at = 0;
    unsigned long find_count = 0;
    time_t elapsed = 0;

    found_at = binstr(&IN_STR, 0, &ALPHA);

    time_t start = time(NULL);

    do {
        for(i = 0; i < 1000; i++) {
            found_at = binstr(&IN_STR, 0, list->entry[i % list->mlen]);
            mu_assert(found_at != BSTR_ERR, "Failed to find!");
            find_count++;
        }

        elapsed = time(NULL) - start;
    } while(elapsed <= TEST_TIME);

    debug("BINSTR COUNT: %lu, END TIME: %d, OPS: %f",
            find_count, (int)elapsed, (double)find_count / elapsed);

    if(binstr_stats) {
        Stats_sample(binstr_stats, find_count);
    }

    return NULL;
}

char *test_find_performance()
{
    int i = 0;
    int found_at = 0;
    unsigned long find_count = 0;
    time_t elapsed = 0;

    found_at = String_find(&IN_STR, &ALPHA);


    time_t start = time(NULL);

    do {
        for(i = 0; i < 1000; i++) {
            found_at = String_find(&IN_STR, list->entry[i % list->mlen]);
            find_count++;
        }

        elapsed = time(NULL) - start;
    } while(elapsed <= TEST_TIME);

    debug("FIND COUNT: %lu, END TIME: %d, OPS: %f",
            find_count, (int)elapsed, (double)find_count / elapsed);

    if(find_stats) {
        Stats_sample(find_stats, find_count);
    }

    return NULL;
}

char *test_scan_performance()
{
    int i = 0;
    int found_at = 0;
    unsigned long find_count = 0;
    time_t elapsed = 0;
    StringScanner *scan = StringScanner_create(&IN_STR);
    // Warm up
    found_at = StringScanner_scan(scan, list->entry[i % list->mlen]);


    time_t start = time(NULL);

    do {
        for(i = 0; i < 1000; i++) {
            found_at = 0;
            
            do {
                found_at = StringScanner_scan(scan, list->entry[i % list->mlen]);
                find_count++;
            } while(found_at != -1);
        }

        elapsed = time(NULL) - start;
    } while(elapsed <= TEST_TIME);

    debug("SCAN COUNT: %lu, END TIME: %d, OPS: %f",
            find_count, (int)elapsed, (double)find_count / elapsed);

    StringScanner_destroy(scan);

    if(scan_stats) {
        Stats_sample(scan_stats, find_count);
    }

    return NULL;
}

char *performance_test()
{
    binstr_stats = Stats_create();
    scan_stats = Stats_create();
    find_stats = Stats_create();

    int i = 0;

    for (i = 0; i < NUM_REPEATS; i++) {
        test_scan_performance();
        test_find_performance();
        test_binstr_performance();
    }

    log_info("-------- binstr --------");
    Stats_dump(binstr_stats);
    log_info("-------- scan ----------");
    Stats_dump(scan_stats);
    log_info("-------- find ----------");
    Stats_dump(find_stats);

    free(binstr_stats);
    free(scan_stats);
    free(find_stats);
    
    return NULL;
}
char *all_tests()
{
    mu_suite_start();

    mu_run_test(test_find_and_scan);
    init_bstrList();
    //this is an idiom for commenting out sections of code
#ifdef STAT_PERF
    mu_run_test(performance_test);
#endif

#if 0
    mu_run_test(test_scan_performance);
    mu_run_test(test_find_performance);
    mu_run_test(test_binstr_performance);
#endif
    destroy_bstrList();

    return NULL;
}

RUN_TESTS(all_tests);
