#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include "dbg.h"

#define MAX_DATA 100


// Called by address, xaaaaaa
int read_string(char **out_string, int max_buffer)
{
    *out_string = calloc(1, max_buffer + 1);
    check_mem(*out_string);
     
    int i = 0;
    // s points on allocated piece of memory
    char *s = *out_string;
    
    for (i = 0; i < max_buffer + 1; i++) {
        *s = fgetc(stdin);
        if (*s == '\n') break;
        *s++;
    }

    *s = '\0';

    return 0;
error:
    if(*out_string) free(*out_string);
    *out_string = NULL;
    return -1;
}

//Calleed by address
int read_int(int *out_int)
{
    char *input = NULL;
    int rc = read_string(&input, MAX_DATA);
    check(rc == 0, "Failed to read number");
    
    *out_int = atoi(input);

    free(input);
    return 0;

error:
    if(input) free(input);
    return -1;
}

int read_scan(const char *fmt, ...)
{
    int i = 0;
    int rc = 0;
    //Pointer
    int *out_int = NULL;
    //Pointer
    char *out_char = NULL;
    // Pointer on poinetr
    char **out_string = NULL;
    int max_buffer = 0;

    va_list argp;
    va_start(argp, fmt);

    for(i = 0; fmt[i] != '\0'; i++) {
        if(fmt[i] == '%') {
            i++;
            switch(fmt[i]) {
                case '\0':
                    sentinel("Invalid format, ended with %%.");
                    break;
                case 'd':
                    out_int = va_arg(argp, int *);
                    rc = read_int(out_int);
                    check(rc == 0, "Failed to read int.");
                    break;
                case 'c':
                    out_char = va_arg(argp, char *);
                    *out_char = fgetc(stdin);
                    break;
                case 's':
                    max_buffer = va_arg(argp, int);
                    out_string = va_arg(argp, char **);
                    rc = read_string(out_string, max_buffer);
                    check(rc == 0, "Failed to read string.");
                    break;

                default:
                    sentinel("Invalid format.");
            }
        } else {
            fgetc(stdin);
        }
        
        check(!feof(stdin) && !ferror(stdin), "Input error.");
    }

    va_end(argp);
    return 0;

error:
    va_end(argp);
    return -1;
}

int print_int(int n)
{
    printf("%d", n);
}

int print_str(char *str)
{
    printf("%s", str);
}

int print_char(char c)
{
    printf("%c", c);
}

int print(const char *fmt, ...)
{
    int i = 0;
    int out_int = NULL;
    char out_char = NULL;
    char *out_str = NULL;    

    va_list argp;
    va_start(argp, fmt);

    for(i = 0; fmt[i] != '\0'; i++) {
        if(fmt[i] == '%') {
            i++;
            switch(fmt[i]) {
                case '\0':
                     sentinel("Invalid format, you nedd ended with %%.");
                     break;
                case 'd':
                     out_int = va_arg(argp, int);
                     print_int(out_int);
                     break;
                case 'c':
                     out_char = va_arg(argp, int);
                     print_char(out_char);
                     break;
                case 's':
                     out_str = va_arg(argp, char *);
                     print_str(out_str);
                     break;
                default:
                     sentinel("Invalid format.");
             }
        } else {
            printf("%c", fmt[i]);
        }
    }
    
    return 0;
error:
    return -1;
}

int main(int argc, char *argv[])
{
    char *first_name = NULL;
    char initial = ' ';
    char *last_name = NULL;
    int age = 0;

    printf("What's your first name? ");
    log_info("First name is at %p", &first_name);
    int rc = read_scan("%s", MAX_DATA, &first_name);
    check(rc == 0, "Failed first name");
    
    printf("What's you initian? ");
    rc = read_scan("%c\n", &initial);
    check(rc == 0, "Failed initial.");

    printf("What's you last name? ");
    rc = read_scan("%s", MAX_DATA, &last_name);
    check(rc == 0, "Failed last name.");

    printf("How old are you? ");
    rc = read_scan("%d", &age);
    check(rc == 0, "Failed age.");

    print("---- RESULTS ----\n");
    print("First Name: %s\n", first_name);
    print("Initial: '%c'\n", initial);
    print("Last Name: %s\n", last_name);
    print("Age: %d\n", age);;

    free(first_name);
    free(last_name);
    return 0;

error:
    return -1;
}    
