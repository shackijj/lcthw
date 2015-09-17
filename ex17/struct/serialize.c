#include <stdio.h>
#include <stdlib.h>

struct Record {
    int id;
    char *name;
};

struct Database {
    int max_rows;
    int max_data;
    FILE *file;
    struct Record **rows;
};

struct Database *Database_open(char *filename, const char)

int main(int argc, char *argv[]) {
    
}
