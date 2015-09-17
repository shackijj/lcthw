#include <string.h>
#include <stdio.h>
#include "dbg.h"

int memset_test(char *to)
{
    memset(to, 1000, 'x');
}

int memmove_test(char *from, char *to) 
{
    memmove(to, from, sizeof(to));          
}

int memcpy_test(char *from, char *to) 
{
    memcpy(to, from, sizeof(to));
}

int main(int argc, char *argv) 

{

    char from[1000] = {'a'};
    char to[1000];

    init_timer();
    time_it(memcpy_test(from, to), "memcpy_test");
    time_it(memmove_test(from, to), "memmove_test");
    time_it(memset_test(to), "memset_test");
   
    return 0;

}

