#define _GNU_SOURCE
#include <lcthw/stats.h>
#include <lcthw/dbg.h>
#include <lcthw/darray.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void multicolumn(char *filename)
{
    size_t buff_size = 128;
    ssize_t read;
    float number; 
    int rc = 0;
    FILE *fp = NULL;
    char *token;
    Stats *st = NULL;
    int line_counter = 0;
    int column_counter = 0;

    DArray *ar_stats = DArray_create(sizeof(Stats), 10);
    check(ar_stats != NULL, "Can't create darray.");
 
    fp = fopen(filename, "r");
    check(fp != NULL, "Can't open file %s", filename);
    
    char *line_ptr = malloc(buff_size);
    check_mem(line_ptr);

    read = getline(&line_ptr, &buff_size, fp);
    check(read != -1, "Can't read headers on first line.");
    line_counter++;

    token = strtok(line_ptr, " ");

    while (token != NULL) {
       if (strcmp(token, " ") != 0) {
           st = Stats_create();
           check(st != NULL, "Can't create stats.");
           DArray_push(ar_stats, st);
       }
       token = strtok(NULL, " ");
    }

    log_info("END: %d", DArray_end(ar_stats));    

    while((read = getline(&line_ptr, &buff_size, fp)) != -1 ) {

        column_counter = 0;
        line_counter++;

        debug("Line is: %s", line_ptr);

        token = strtok(line_ptr, " ");
        while(token != NULL) {
            if (strcmp(token, " ") != 0 ){

                st = DArray_get(ar_stats, column_counter);
                check(st != NULL, "Date error line %d, column %d", 
                   line_counter, column_counter);

                rc = sscanf(line_ptr, "%f", &number);

                if (rc >= 0) {
                    debug("Succesfully read: %f", number);
                    Stats_sample(st, number);
                }

                token = strtok(NULL, " ");
                column_counter++;
            }
        }
        check(column_counter == DArray_end(ar_stats), 
            "Date error line %d, column %d", line_counter, column_counter); 
    }        

    for (rc = 0; rc < DArray_end(ar_stats); rc++) {
        st = ar_stats->contents[rc];
        Stats_dump(st);
    }

//falltrough
error:
    if(ar_stats) DArray_clear_destroy(ar_stats);
    if(line_ptr) free(line_ptr);
    if(fp) fclose(fp);
}

void onecolumn(char *filename)
{
    size_t buff_size = 128;
    ssize_t read;
    float number; 
    int rc = 0;
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
