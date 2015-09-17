#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

struct Record {
    int id;
    int set;
    char *record;
};

struct Database {
    int max_rows;
    int max_data;
    FILE *file;
    struct Record *rows;
};

struct Database *db = NULL;

void Database_close();

void die(const char *message)
{
    if(errno) {
        perror(message);
    } else {
        printf("ERROR: %s\n", message);
    }

    Database_close(db);

    exit(1);
}


void Database_load()
{
    struct Record *row;
    // First read max_data
    int rc = fread(&db->max_data, sizeof(db->max_data), 1, db->file);
    if(rc != 1) die("Failed to max_rows");
    // Then read max_rows
    rc = fread(&db->max_rows, sizeof(db->max_rows), 1, db->file);
    if(rc != 1) die("Failed to max_data");

    // Take memory for Record array
    db->rows = malloc(sizeof(struct Record) * db->max_rows);

    //Read structs which had been written one by one
    for (row = db->rows; row < db->rows + db->max_rows; row++) {

        rc = fread(&row->id, sizeof(row->id), 1, db->file);
        if (rc != 1) die("Failed read rec->id");

        rc = fread(&row->set, sizeof(row->set), 1, db->file);
        if (rc != 1) die("Failed read rec->set");

        // Take memory for record
        row->record = malloc(db->max_data);
        if(!row->record) die("Memory Error");
        // Read data from file to that memory

        rc = fread(row->record, db->max_data, 1, db->file);
        if (rc != 1) die("Failed read rec->record");
    }
}

void Database_open(const char *filename, char mode)
{
    db = malloc(sizeof(struct Database));
    if(!db) die("Memory Error");

    if(mode == 'c') {
        db->file = fopen(filename, "w");
    } else {
        db->file = fopen(filename, "r+");
        if(db->file) {
            Database_load();
        }
    }
    if(!db->file) die("Failed to open file");
}

void Database_create(int max_data, int max_rows)
{
    int i = 0;
    struct Record *row;

    db->max_data = max_data;
    db->max_rows = max_rows;

    // Get memory for all records
    // without data just ints and pointer
    db->rows = malloc(sizeof(struct Record) * db->max_rows);
    if(!db->rows) die("Memory error");

    for(i = 0; i < db->max_rows; i++) {
        // Set row to RAM address
        row = &db->rows[i];
        row->id = i;
        row->set = 0;

        row->record = malloc(db->max_data);
        if(!row->record) die("Memory error");

        char *res = strncpy(row->record, "NULL", db->max_data);
        if(!res) die("STRNCPY ERROR");

        res[db->max_data - 1] = '\0';
    }
}

void Database_write()
{
    struct Record *row;

    rewind(db->file);

    int rc = fwrite(&db->max_data, sizeof(db->max_data), 1,  db->file);
    if(rc != 1) die("Failed to write MAX_DATA");

    rc = fwrite(&db->max_rows, sizeof(db->max_rows), 1, db->file);
    if(rc != 1) die("Failed to write MAX_ROWS");

    for(row = db->rows; row < db->max_rows + db->rows; ++row) {

        rc = fwrite(&row->id, sizeof(row->id), 1, db->file);
        if(rc != 1) die("Failed to write a id");

        rc = fwrite(&row->set, sizeof(row->set), 1, db->file);
        if(rc != 1) die("Failed to write a set");

        // What is placed in row->record? Pointer?
        rc = fwrite(row->record, db->max_data, 1, db->file);
        if(rc != 1) die("Failed to write a record");
    }


    rc = fflush(db->file);
    if(rc == -1) die("Cannot flush database");
}

void Database_close()
{
    struct Record *row;

    if(db) {
        if(db->file) fclose(db->file);
        if(db->rows) {
            //Set pointer row on first row and free them one by one
            for(row = db->rows; row < db->max_rows + db->rows; row++) {
                if(row->record) {
                    free(row->record);
                }
            }
            free(db->rows);
        }
        free(db);
    }
}

void Record_print(struct Record *row)
{
    printf("%d %s\n", row->id, row->record);
}


void Database_list()
{
    struct Record *row;

    for(row = db->rows; row < db->rows + db->max_rows; row++) {
        if(row->set) {
            Record_print(row);
        }
    }
}

//What is const?

void Database_set(int id, const char *record)
{
    // Get address of current record and 
    struct Record *row = &db->rows[id];
    if(row->set) die("Already set, delete it first");

    row->set = 1;

    char *res = strncpy(row->record, record, db->max_data);
    if(!res) die("Record copy failed");
    res[db->max_data - 1] = '\0';
}

void Database_delete(int id)
{
    struct Record *row= &db->rows[id];

    char *res = strncpy(row->record, "NULL", db->max_data);
    if(!res) die("STRNCPY ERROR");

}

void Database_get(int id)
{
   if((id > db->max_rows - 1) || (id < 0)) die("Id out of index");

   struct Record *row = &db->rows[id];
   if(row->set) Record_print(row);

}

void Database_find(const char *pattern)
{

    struct Record *row;
    int i = 0;

    for(i = 0; i < db->max_rows - 1; i++) {
        row = &db->rows[i];
        if (!row->set) continue;

        char *res = strstr(row->record, pattern);
        if(res) Record_print(row);
    }
}

int main(int argc, char *argv[])
{
   if(argc < 3) die("USAGE: struct <dbfile> <action> [action param]");

    char *filename = argv[1];
    char action = argv[2][0];
    Database_open(filename, action);

    switch(action) {
        case 'c':
            if(argc < 5) die("Need MAX_DATA and MAX_ROWS");
            Database_create(atoi(argv[3]), atoi(argv[4]));
            Database_write();
            break;
        case 'l':
            Database_list();
            break;
        case 's':
            if(argc < 5) die("Need id, record");
            Database_set(atoi(argv[3]), argv[4]);
            Database_write();
            break;
        case 'd':
            if(argc < 4) die("Need id");
            Database_delete(atoi(argv[3]));
            Database_write();
            break;
        case 'g':
            if(argc < 4) die("Need id");
            Database_get(atoi(argv[3]));
            break;
        case 'f':
            if(argc < 4) die("Need a pattern to find.");
            Database_find(argv[3]);
            break;
        default:
            die("Invalid action, only c=create, g=get, s=set,d =del, l=list");
    }

    Database_close();

    return 0;
}
