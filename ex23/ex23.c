#include <stdio.h>
#include <string.h>
#include <time.h>
#include "dbg.h"
#include "ex23.h"

int normal_copy(char *from, char *to, int count)
{
    int i = 0;
    
    for(i = 0; i < count; i++) {
        to[i] = from[i];
    }

    return i;
}

int duffs_device(char *from, char *to, int count)
{
    {
        int n = (count + 7) / 8;

        switch(count % 8) {
            case 0: do { 
                        CREATE_8_CASES(from, to) 
                    } while(--n > 0);
        }
    }

    return count;
}

int zeds_device(char *from, char *to, int count)
{
    //New scope
    {
	// Example: 1000
        int n = (count + 7) / 8;
    	// n = 1007 / 8 = 125
             
        // 125 % 8 = 15
        switch(count % 8) {
            case 0:
            // All that cases work and looped 125 times,
            // because there are'nt any break command
            // 125 * 8 = 1000;
            // Postfix form is used so.
            again: CREATE_8_CASES(from, to)
                    if(--n > 0) goto again;
        }
    }
    
    return count;
}

int valid_copy(char *data, int count, char expects)
{
    int i = 0;
    for(i = 0; i < count; i++) {
        if(data[i] != expects) {
            log_err("[%d] %c != %c", i, data[i], expects);
            return 0;
        }
    }

    return 1;
}

int main(int argc, char *argv[])
{
    char from[1000] = {'a'};
    char to[1000] = {'c'};
    int rc = 0;


    init_timer();

    memset(to, 'y', 1000);

    time_it(normal_copy(from, to, 1000), "normal_copy");

    //Setup from to have some stuff
    memset(from, 'x', 1000);
    //Set it to a failure mode
    memset(to, 'y', 1000);
    check(valid_copy(to, 1000, 'y'), "Not initialized right.");

    rc = normal_copy(from, to, 1000);

    check(rc == 1000, "Normal copy failed: %d.", rc);

    check(valid_copy(to, 1000, 'x'), "Normal copy failed.");

    // reset
    memset(to, 'y', 1000);
    
    //duffs version
    rc = duffs_device(from, to, 1000);

    check(rc == 1000, "Duff's device failed: %d.", rc);
    check(valid_copy(to, 1000, 'x'), "Duff's device failed copy.");

    // reset 

    memset(to, 'y', 1000);

    rc = zeds_device(from, to, 1000);
    check(rc == 1000, "Zed's device failed: %d.", rc);
    check(valid_copy(to, 1000, 'x'), "Zed's device failed copy.");
    
    return 0;
error:
    return 1;
}
