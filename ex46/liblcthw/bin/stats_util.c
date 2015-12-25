#define _GNU_SOURCE
#include <lcthw/stats.h>
#include <lcthw/dbg.h>
#include <stdio.h>
#include <stdlib.h>

void onecolumn(char *filename)
{
    size_t buff_size = 128;
    ssize_t read;
    float number; 
    int rc = 0;;
    FILE *fp = NULL;

    Stats *st = Stats_create();
    check(st != NULL, "Memory error");
 
    fp = fopen(filename, "r");
    check(fp != NULL, "Can't open file %s", filename);
    
    char *line_ptr = malloc(buff_size);
    check_mem(line_ptr);

    while((read = getline(&line_ptr, &buff_size, fp)) != -1 ) {
        debug("Line is: %s", line_ptr);
        rc = sscanf(line_ptr, "%f", &number);
        if (rc) {
            debug("Succesfully read: %f", number);
            Stats_sample(st, number);
        }     
    }        
    
    Stats_dump(st);

//falltrough
error:
    if(line_ptr) free(line_ptr);
    if(st) free(st);
    if(fp) fclose(fp);
}

void multicolumn(char *filename)
{
    debug("[MULTI] I will open %s", filename);
}

void show_usage()
{
    printf("Usage stats_util [-h] file\n");
}

int main(int argc, char *argv[])
{
    switch (argc) {
        case 2:
            onecolumn(argv[1]);
            break;
        case 3:
            if (strcmp(argv[1], "-h") == 0) {
                multicolumn(argv[2]);
            } else {
                show_usage();
            }
            break;
        default:
            show_usage();
    }

    return 0;
}
