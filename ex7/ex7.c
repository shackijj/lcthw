#include <stdio.h>

int main(int argc, char *argv[])
{
    //16bit
    int bugs = 100;
    double bug_rate = 1.2;

    printf("You have %d bugs at the imaginary rate of %f.\n",
        bugs, bug_rate);

    //32 byte
    long universe_of_defects = -2147483647; // 2L * 1L * 1024L * 1024L * 1024L;
    printf("The entire universe has %ld bugs.\n",
        universe_of_defects);

    double expected_bugs = bugs * bug_rate;
    printf("You are expected to have %f bugs.\n",
       expected_bugs);

    double part_of_universe = expected_bugs / universe_of_defects;
    printf("That is only a %e porion of universe.\n",
        part_of_universe);


    char nul_byte = 'a';
    int care_percentage = bugs * nul_byte;
    printf("which means you should care %d%%.\n",
        care_percentage);

    printf("That's null byte %d  printed as int.\n",
       nul_byte);

    printf("That's null byte %c printed as character.\n",
       nul_byte); 

    return 0;
}
