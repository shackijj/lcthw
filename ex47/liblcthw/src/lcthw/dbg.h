#ifndef __dbg_h__
#define __dbg_h__

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/time.h>

#ifdef NDEBUG
#define debug(M, ...)
#else
#define debug(M, ...) fprintf(stderr, "[DEBUG] %s:%d: " M "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#endif

#define clean_errno() (errno == 0 ? "None" : strerror(errno))

#define log_err(M, ...) fprintf(stderr, "[ERROR] (%s:%d function: %s errno: %s) " M "\n", __FILE__, __LINE__, __func__, clean_errno(), ##__VA_ARGS__)

#define log_warn(M, ...) fprintf(stderr, "[WARN] (%s:%d: function: %s errno: %s) " M "\n", __FILE__, __LINE__, __func__, clean_errno(), ##__VA_ARGS__)

#define log_info(M, ...) fprintf(stderr, "[INFO] (%s:%d) " M "\n", __FILE__, __LINE__, ##__VA_ARGS__)

#define check(A, M, ...) if(!(A)) { log_err(M, ##__VA_ARGS__); errno=0; goto error; }

#define sentinel(M, ...)  { log_err(M, ##__VA_ARGS__); errno=0; goto error; }

#define check_mem(A) check((A), "Out of memory.")

#define check_debug(A, M, ...) if(!(A)) { debug(M, ##__VA_ARGS__); errno=0; goto error; }

#define init_timer() struct timeval tval_before, tval_after, tval_result;

#define time_it( F, M )\
    gettimeofday(&tval_before, NULL);\
    F();\
    gettimeofday(&tval_after, NULL);\
    timersub(&tval_after, &tval_before, &tval_result);\
    log_info(M " Time elapsed: %ld.%06ld\n", \
    (long int)tval_result.tv_sec, (long int) tval_result.tv_usec);

#define time_it_with_args( F, M )\
    gettimeofday(&tval_before, NULL);\
    F;\
    gettimeofday(&tval_after, NULL);\
    timersub(&tval_after, &tval_before, &tval_result);\
    log_info(M " Time elapsed: %ld.%06ld\n", \
    (long int)tval_result.tv_sec, (long int) tval_result.tv_usec);


#endif
