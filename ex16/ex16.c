#include <stdio.h>
//functions: assert
#include <assert.h>
// functions: malloc, free
#include <stdlib.h>
// functions: strdup
#include <string.h>


//Declare struct and type variables which are part of it.
struct Person {
    char *name;
    int age;
    int height;
    int weight;
};

// Function which create person object in memory
struct Person *Person_create(char *name, int age, int height, int weight)
{
    //Allocate memory for our struct and return pointer or NULL
    struct Person *who = malloc(sizeof(struct Person));
    // Check result of logical expression and in case of NULL
    // abort the program by abort(3) and print error in stdin
    assert(who != NULL);
    //Strdup return a pointer to new srtring which is duplicte of it's arg.
    who->name = strdup(name);
    who->age = age;
    who->height = height;
    who->weight = weight;

    return who;
}


void Person_destroy(struct Person *who)
{
    assert(who != NULL);
    // Return memory we got with malloc and strdup to OS
    free(who->name);
    free(who);
}

void Person_print(struct Person *who)
{
    printf("Name: %s\n", who->name);
    printf("\tAge: %d\n", who->age);
    printf("\tHeight: %d\n", who->height);
    printf("\tWeight: %d\n", who->weight);
}

int main(int argc, char *argv[])
{
    struct Person *joe = Person_create(
        "Joe Alex", 32, 64, 140);

    struct Person *frank = Person_create(
        "Frank Blank", 20, 72, 180);

    printf("Joe is at memory location %p:\n", joe);
    Person_print(joe);

    printf("Frank is at memory location %p:\n", frank);
    Person_print(frank);

    joe->age += 20;
    joe->height -= 2;
    joe->weight += 40;
    Person_print(joe);

    frank->age += 20;
    frank->weight += 20;
    Person_print(frank);

    Person_destroy(joe);
    Person_destroy(frank);

    return 0;
}
