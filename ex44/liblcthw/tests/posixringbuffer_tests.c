#include "minunit.h"
#include <lcthw/posixringbuffer.h>
#include <string.h>

#define BUFFER_LEN 15

char *test1 = "test1";
char *test2 = "test2";
char *test3 = "test3";
PosixRingBuffer *posix_buff = NULL;

char *test_posix_create()
{
    posix_buff = PosixRingBuffer_create(BUFFER_LEN);
    mu_assert(posix_buff != NULL, "Can't create buffer");
  
    return NULL;
}

char *test_posix_read_write()
{
    char *target = malloc(sizeof(test1) + 1);

    int rc = PosixRingBuffer_write(posix_buff, test1, 5);
    mu_assert(rc == 5, "Can't write to posix_buffer.");

    rc = PosixRingBuffer_write(posix_buff, test2, 5);
    mu_assert(rc == 5, "Can't wtite to posix_buffer.");

    rc = PosixRingBuffer_read(posix_buff, target, 5);
    mu_assert(rc == 5, "Can't read from from posix_buffer.");
    mu_assert(memcmp(test1, target, 5) == 0, "Wrong data in posix_buffer.");

    rc = PosixRingBuffer_read(posix_buff, target, 5);
    mu_assert(rc == 5, "Can't read from posix_buffer.");
    mu_assert(memcmp(test2, target, 5) == 0, "Wrong data in posix_buffer.");

    rc = PosixRingBuffer_write(posix_buff, test3, 5);
    mu_assert(rc == 5, "Can't write to posix_buffer.");
    
    rc = PosixRingBuffer_read(posix_buff, target, 5);
    mu_assert(rc == 5, "Can't read from posix_buffer.");
    mu_assert(memcmp(test3, target, 5) == 0, "Wrong data in posix_buffer.");

    free(target);
    return NULL;
}

char *test_posix_puts_gets()
{
    bstring a = bfromcstr(test1);
    
    int rc = PosixRingBuffer_puts(posix_buff, a);
    mu_assert(rc == 5, "Cant't write to posix_buff.");

    bstring b = PosixRingBuffer_gets(posix_buff, 5);
    mu_assert(bstricmp(a, b) == 0, "Wrong result.");

    rc = PosixRingBuffer_puts(posix_buff, a);
    mu_assert(rc == 5, "Can't write to posix_buff.");
    
    bstring c = PosixRingBuffer_get_all(posix_buff);
    mu_assert(bstricmp(a, c) == 0, "Wrong result.");

    bdestroy(a);
    bdestroy(b);
    bdestroy(c);

    return NULL;
}

char *test_posix_destroy()
{
    PosixRingBuffer_destroy(posix_buff);

    return NULL;
}

char *test_fuzzing()
{
    // data 99\0 = 8 * 100 = 800 chars
    PosixRingBuffer *buffer = PosixRingBuffer_create(800);
    int i = 0;
    int rc = 0;
    bstring numbers[100] = {NULL};    

    for(i = 0; i < 100; i++) {

        numbers[i] = bformat("data %d", i);

        rc = PosixRingBuffer_write(buffer, bdata(numbers[i]), blength(numbers[i]));
        mu_assert(rc == blength(numbers[i]), "Can't write to buffer");
    }

    bstring test;

    for(i = 0; i < 100; i++) {

        test = PosixRingBuffer_gets(buffer, blength(numbers[i]));
        mu_assert(biseq(test, numbers[i]) == 1, "Wrong data in buffer.");

        bdestroy(test);
        bdestroy(numbers[i]);
    }

    PosixRingBuffer_destroy(buffer);
 
    return NULL;               
}

char *all_tests()
{
    mu_suite_start();

    mu_run_test(test_posix_create);
    mu_run_test(test_posix_read_write);
    mu_run_test(test_posix_puts_gets);
    mu_run_test(test_posix_destroy);
    mu_run_test(test_fuzzing);

    return NULL;
}

RUN_TESTS(all_tests);    
