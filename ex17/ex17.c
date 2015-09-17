#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

//#define MAX_DATA 512
//#define MAX_ROWS 100

struct Address {
    int id;
    int set;
    char *name;
    char *email;
};

struct Database {
    int max_data;
    int max_rows;
    //Pointer to struct
    struct Address *rows;
};

struct Connection {
    FILE *file;
    struct Database *db;
};

void Database_close(struct Connection *conn);

void die(const char *message, struct Connection *conn)
{
    if(errno) {
        perror(message);
    } else {
        printf("ERROR: %s\n", message);
    }

    Database_close(conn);

    exit(1);
}

void Address_print(struct Address *addr)
{
    printf("%d %s %s\n",
        addr->id, addr->name, addr->email);
}

void Database_load(struct Connection *conn)
{
    // 1 - what to read? 2-size of book 3- how many elements 4- file
    int rc = fread(conn->db, sizeof(struct Database), 1, conn->file);
    if(rc != 1) die ("Failed to load database.", conn);

    printf("Max Rows is %d\n", conn->db->max_rows);
    printf("Max Data is %d\n", conn->db->max_data);
}


struct Connection *Database_open(const char *filename, char mode)
{
    struct Connection *conn = malloc(sizeof(struct Connection));
    if(!conn) die("Memory error", conn);

    conn->db = malloc(sizeof(struct Database));
    if(!conn->db) die("Memory error", conn);

    if(mode == 'c') {
        conn->file = fopen(filename, "w");
    } else {
        //Open for reading and writing
        conn->file = fopen(filename, "r+");

        if(conn->file) {
            Database_load(conn);
        }
    }

    if(!conn->file) die("Failed to open the file", conn);

    return conn;
}

void Database_close(struct Connection *conn)
{
    if(conn) {
        if(conn->file) fclose(conn->file);
        if(conn->db->rows) free(conn->db->rows);
        if(conn->db) free(conn->db);
        free(conn);
    }
}

void Database_write(struct Connection *conn)
{
    rewind(conn->file);

    struct Address *addr;
    int rc = fwrite(conn->db, sizeof(struct Database), 1, conn->file);
    if(rc != 1) die("Failed to write database.", conn);

    rc = fwrite(&conn->db->max_data, sizeof(conn->db->max_data), 1, conn->file);
    if(rc != 1) die("Failed to write max_data size", conn);

    rc = fwrite(&conn->db->max_rows, sizeof(conn->db->max_rows), 1, conn->file);
    if(rc != 1) die("Failed to write max_row size.", conn);

    //Using pointer like an index
    for(addr = conn->db->rows; addr < conn->db->rows + conn->db->max_rows; addr++) {
       if(fwrite(addr, sizeof(addr), 1, conn->file) != 1)
          /*
          fwrite(&addr->set, sizeof(addr->set), 1, conn->file) != 1 ||
          fwrite(addr->name, conn->db->max_data, 1, conn->file) != 1 ||
          fwrite(addr->email, conn->db->max_data, 1, conn->file) != 1)
          */
          die("Failed to write database", conn);
    }

    rc = fflush(conn->file);
    if (rc == -1) die("Cannot flush database.", conn);
}

void Database_create(struct Connection *conn, int max_data, int max_rows)
{
    int i = 0;

    conn->db->max_rows = max_rows;
    conn->db->max_data = max_data;
    // Get RAM for creating rows
    conn->db->rows = malloc(conn->db->max_rows * sizeof(struct Address));

    for(i = 0; i < max_rows; i++) {
        //make prototype to inititialize it
        struct Address addr = {.id = i, .set = 0, .name = "test", .email = "test"};
        conn->db->rows[i] = addr;
    }
}

void Database_set(struct Connection *conn, int id, char *name,
        char *email)
{
    struct Address *addr = &conn->db->rows[id];
    if(addr->set) die("Already set, delete it first", conn);

    addr->set = 1;
    name[-1] = '\0';
    email[-1] = '\0';

    // What is this?
    char *res = strncpy(addr->name, name, conn->db->max_data);
    if (!res) die("Name copy failed", conn);

    res = strncpy(addr->email, email, conn->db->max_data);
    if (!res) die("Email copy failed", conn);
}

void Database_get(struct Connection *conn, int id)
{
    struct Address *addr = &conn->db->rows[id];

    if(addr->set) {
        Address_print(addr);
    } else {
        die("ID is not set", conn);
    }
}

void Database_delete(struct Connection *conn, int id)
{
    struct Address addr = {.id = id, .set = 0};
    conn->db->rows[id] = addr;
}

void Database_list(struct Connection *conn)
{
    int i = 0;
    struct Database *db = conn->db;

    for(i = 0; i < db->max_rows; i++) {
        struct Address *cur = &db->rows[i];

        if(cur->set) {
           Address_print(cur);
        }
    }
}

int main(int argc, char *argv[])
{
    if(argc < 3) {
        printf("USAGE: ex17 <dbfile> <action> [action params]");
        exit(1);
    }

    char *filename = argv[1];
    char action = argv[2][0];
    struct Connection *conn = Database_open(filename, action);
    int id = 0;

    if (action != 'c') {
        if(argc > 3 && action) id = atoi(argv[3]);
        if(id >= conn->db->max_rows) die("There's not that many records", conn);
    }

    switch(action) {
        case 'c':
            if(argc != 5) die("Need a max_data, max_rows", conn);

            int max_data = atoi(argv[3]);
            int max_rows = atoi(argv[4]);

            printf("MAX DATA: %d, MAX ROWS %d\n", max_data, max_rows);

            Database_create(conn, max_data, max_rows);
            Database_write(conn);
            break;

        case 'g':
            if(argc != 4) die("Need an id to get", conn);
            Database_get(conn, id);
            break;

        case 's':
            if(argc != 6) die("Need id, name, email to set", conn);

            Database_set(conn, id, argv[4], argv[5]);
            Database_write(conn);
            break;

        case 'd':
            if(argc != 4) die("Need id to delete", conn);
            Database_delete(conn, id);
            Database_write(conn);
            break;

        case 'l':
            Database_list(conn);
            break;

        default:
            die("Invalid action, only: c=create, g=get, s=set, d=del, l=list", conn);
    }

    Database_close(conn);

    return 0;
}
