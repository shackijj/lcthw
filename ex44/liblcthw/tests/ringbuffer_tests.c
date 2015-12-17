#include "minunit.h"
#include <lcthw/ringbuffer.h>
#include <string.h>
#include <time.h>

#define BUFFER_LEN 15

char *test1 = "test1";
char *test2 = "test2";
char *test3 = "test3";
RingBuffer *buff = NULL;

char *test_create()
{
    buff = RingBuffer_create(BUFFER_LEN);
    mu_assert(buff != NULL, "Can't create buffer");

    return NULL;
}

char *test_read_write()
{
    char *target = malloc(sizeof(test1) + 1);

    int rc = RingBuffer_write(buff, test1, 5);
    mu_assert(rc == 5, "Can't write to buffer.");

    rc = RingBuffer_write(buff, test2, 5);
    mu_assert(rc == 5, "Can't wtite to buffer.");

    rc = RingBuffer_read(buff, target, 5);
    mu_assert(rc == 5, "Can't read from from buffer.");
    mu_assert(memcmp(test1, target, 5) == 0, "Wrong data in buffer.");

    rc = RingBuffer_read(buff, target, 5);
    mu_assert(rc == 5, "Can't read from buffer.");
    mu_assert(memcmp(test2, target, 5) == 0, "Wrong data in buffer.");

    rc = RingBuffer_write(buff, test3, 5);
    mu_assert(rc == 5, "Can't write to buffer.");
    
    rc = RingBuffer_read(buff, target, 5);
    mu_assert(rc == 5, "Can't read from buffer.");
    mu_assert(memcmp(test3, target, 5) == 0, "Wrong data in buffer.");

    free(target);
    return NULL;
}

char *test_puts_gets()
{
    bstring a = bfromcstr(test1);
    
    int rc = RingBuffer_puts(buff, a);
    mu_assert(rc == 5, "Cant't write to buff.");

    bstring b = RingBuffer_gets(buff, 5);
    mu_assert(bstricmp(a, b) == 0, "Wrong result.");

    rc = RingBuffer_puts(buff, a);
    mu_assert(rc == 5, "Can't write to buff.");
    
    bstring c = RingBuffer_get_all(buff);
    mu_assert(bstricmp(a, c) == 0, "Wrong result.");

    bdestroy(a);
    bdestroy(b);
    bdestroy(c);

    return NULL;
}

char *test_destroy()
{ 
    RingBuffer_destroy(buff);

    return NULL;  
}

char *test_fuzzing()
{
    // data 99\0 = 8 * 100 = 800 chars
    RingBuffer *buffer = RingBuffer_create(1024);
    int i = 0;
    int rc = 0;
    bstring chunk_w, chunk_r;

    srand((unsigned int)time(NULL));

    FILE *urand = fopen("/dev/urandom", "r");  
    check(urand != NULL, "Failed to open /dev/random.");

    struct bStream *stream = bsopen((bNread)fread, urand);  

    chunk_w = bfromcstr("");

    for(i = 0; i < 100; i++) {

        int num = rand();
        rc = bsread(chunk_w, stream, num % 20);

        rc = RingBuffer_write(buffer, bdata(chunk_w), blength(chunk_w));
        mu_assert(rc == blength(chunk_w), "Can't write to buffer");
  
        chunk_r = RingBuffer_gets(buffer, 10);
        debug("Chunk is: %s", bdata(chunk_r));           

        bdestroy(chunk_r);
    }

    bdestroy(chunk_w);
    RingBuffer_destroy(buffer);
 
    return NULL;
error:
    return NULL;               
}

char *all_tests()
{
    mu_suite_start();

    mu_run_test(test_create);
    mu_run_test(test_read_write);
    mu_run_test(test_puts_gets);
    mu_run_test(test_destroy);

    mu_run_test(test_fuzzing);

    return NULL;
}

RUN_TESTS(all_tests);       
