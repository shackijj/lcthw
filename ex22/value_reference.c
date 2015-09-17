#include <stdio.h>
#include "dbg.h"

int i = 1;
int *pi = &i;

void Call_by_value(int i)
{
    i++;
    log_info("i inside Call_by_value: %d", i); 
}

void Call_by_reference(int *pi)
{
    *pi = 2;
    log_info("i insde Call_by_reference: %d", *pi);
}

int main()
{

    log_info("i in global scope euaqls: %d", i);
    Call_by_value(i);
    log_info("i after beign call by value: %d", i);

    log_info("i in global scope euaqls: %d", i);
    Call_by_reference(pi);
    log_info("i eqals after beign call by reference: %d", i);

    return 0;
}
