#include <stdio.h>

int main(int argc, char *argv[])
{
    int ages[] = {23, 43, 12, 89, 2};
    char *names[] = {
        "Alan", "Frank",
        "Mary", "John", "Lisa"
    };

    int i = 0;
    int count = sizeof(ages) / sizeof(int);
    int *cur_age = ages;
    char **cur_name = names;


    void array_style(char **strings, int *digits, int i, int count)
    {
        printf("---ARRAY STYLE---\n");
        while(i < count) {
            printf("%s is %d years old\n",
                strings[i], digits[i]);
            i++;
        }
        printf("-------\n");

    }

    void pointer_style(char **strings, int *digits, int i, int count)
    {
        printf("---POINTRS STYLE----\n");

        while(i < count) {
            printf("%s is %d years old\n",
                *(strings + i), *(digits + i));
            i++;
        }
        printf("--------------------\n");
    }

    void pointer_like_array(char **strings, int *digits, int i, int count)
    {
        printf("----POINTERS LIKE ARRAY----\n");
        while(i < count) {
           printf("%s is %d years old\n",
               strings[i], digits[i]);
           i++;
        }
    }

    void pointers_crazy(char **strings, int *digits)
    {
        printf("----POINTERS CRAZY----\n");
        strings = names;
        digits = ages;
        int delta = digits - ages;

        while(delta < count) {
           printf("%s is %d years old\n",
               *strings, *digits);
           strings++;
           digits++;
           delta = digits - ages;
        }
    }

    array_style(cur_name, cur_age, i, count);
    pointer_style(cur_name, cur_age, i, count);
    pointer_like_array(cur_name, cur_age, i, count);
    pointers_crazy(cur_name, cur_age);

    return 0;
}
