#include <stdio.h>
#include <ctype.h>
#include "dbg.h"

int print_a_message(const char *msg)
{
    printf("A STRING: %s\n", msg);
    
    return 0;
}

int uppercase(const char *msg, int len)
{
    int i = 0;
    log_info("Get %d length", len);

    for(i = 0; i < len ; i++) {
        printf("%d: %c\n", i, toupper(msg[i]));
    }
    log_info("Last step is %d", i);
    printf("\n");
  
    return 0;
}

int lowercase(const char *msg, int len)
{
    int i = 0;

    for(i = 0; i < len; i++) {
        printf("%c", tolower(msg[i]));
    }

    printf("\n");

    return 0;
}

int fail_on_purpose(const char *msg)
{
   return 1;
}
