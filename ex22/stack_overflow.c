#include <stdio.h>

int recursion()
{
    long double overload;
    overload = 3000.0;
    recursion();
    return 0;
}

int main()
{
    recursion();
    return 0;
}
