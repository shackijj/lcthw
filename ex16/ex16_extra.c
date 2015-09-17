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

void Person_print(struct Person who)
{
    printf("Name: %s\n", who.name);
    printf("\tAge: %d\n", who.age);
    printf("\tHeight: %d\n", who.height);
    printf("\tWeight: %d\n", who.weight);
}

struct Person Person_create(char *name, int age, int height, int weight)
{
    struct Person who;

    who.name = name;
    who.age = age;
    who.height = height;
    who.weight = weight;

    return who;

}

int main(int argc, char *argv[])
{
    struct Person dmitry = Person_create(
        "Dmitry Lobaskov", 27, 180, 70);

    Person_print(dmitry);

    return 0;
}
