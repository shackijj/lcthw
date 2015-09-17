#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "dbg.h"

#define MAX_DATA 100

typedef enum EyeColor {
    BLUE_EYES, GREEN_EYES, BROWN_EYES,
    BLACK_EYES, OTHER_EYES
} EyeColor;

const char *EYE_COLOR_NAMES[] = {
    "Blue", "Green", "Brown", "Black", "Other"
};

typedef struct Person {
    int age;
    char first_name[MAX_DATA];
    char last_name[MAX_DATA];
    EyeColor eyes;
    float income;
} Person;

char * trim(char str[]) 
{
    int i = strlen(str) - 1; 

    while (str[i] == '\n' || str[i] == ' ' || str[i] == '\0') {
        str[i] = '\0';
        i--;
    }

    return str;
}
     
int input(int s, char *to)
{
    int i;
    for(i = 0; i <= s; i++) {
        int rc = scanf("%c", to);
        check(rc > 0, "Failed to read a char");
        if(*to == '\n') break; 
        to++;
    }
    *to = '\0'; 
    return 0;
error:
    return -1;
}

int main(int argc, char *argv[])
{
    Person you = {.age = 0};
    int i = 0;
    char *in = NULL;
    //char buff[MAX_DATA];
    char buff[MAX_DATA];

    printf("What's your First Name? ");
    in = fgets(buff, MAX_DATA-1, stdin);
    check(in != NULL, "Failed to read first name.");
    strcpy(you.first_name, trim(buff));

    printf("What's your Last Name? ");
    input(30, you.last_name);

   //in = fgets(you.last_name, MAX_DATA-1, stdin);
    //check(in != NULL, "Failed to read last name.");

    printf("How old are you? ");
    in = fgets(buff, MAX_DATA-1, stdin);
    check(in != NULL, "Failed to read years.");
    you.age = atoi(buff);
    check(you.age > 0, "You have to enter a number.");

    //int rc = fscanf(stdin, "%d", &you.age);
    //check(rc > 0, "You have to enter a number. ");

    printf("What color are your eyes:\n");
    for (i = 0; i <= OTHER_EYES; i++) {
        printf("%d) %s\n", i+1, EYE_COLOR_NAMES[i]);
    }
    printf("> ");

    int eyes = -1;
    in = fgets(buff, MAX_DATA-1, stdin);
    check(in != NULL, "Failed to read a number."); 
    you.eyes = atoi(buff) - 1;
    check(you.eyes <= OTHER_EYES && you.eyes >= 0,
       "Do it right, that's not an option");

    printf("How much do you make an hour? ");
    //in = fgets(buff, MAX_DATA-1, stdin);
    //check(in != NULL, "Failed to read income.");
    //log_info("atof(buff): %f", atof(buff));
    //you.income = atof(buff);
    //check(you.income > 0, "Enter a floating number");
    
    //int rc = fscanf(stdin, "%f", &you.income);
    //check(rc > 0, "Enter a floating number.");

    int rc = scanf("%f", &you.income);
    check(rc > 0, "Enter a floating number.");

    printf("-------------RESULTS-------------\n");
    printf("Fisrt Name: %s\n", you.first_name);
    printf("Last Name: %s\n", you.last_name);
    printf("Age: %d\n", you.age);
    printf("Eyes: %s\n", EYE_COLOR_NAMES[you.eyes]);
    printf("Income: %f\n", you.income);

    return 0;
error:
    return -1;
}
