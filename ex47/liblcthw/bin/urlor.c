#include <lcthw/tstree.h>
#include <lcthw/bstrlib.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct Handler {
    bstring url;
    bstring name;
    bstring dir;
} Handler;

bstring read_file(Handler h, const char *filename)
{
    int rc = 0;
    struct bStream *stream = NULL;
    FILE *fh = NULL;
    long fsize = 0;
    bstring result = NULL;

    check(filename != NULL, "Filename can't be NULL.");

    fh = fopen(filename, "r");
    check(fh != NULL, "Can't open file '%s'", filename);

    fseek(fh, 0, SEEK_END);
    fsize = ftell(fh);
    rewind(fh);

    result = bfromcstr("");

    stream = bsopen((bNread) fread, fh);
    check(bsread(result, stream, fsize) == BSTR_OK, "Can't read from stream");

    bsclose(stream);
    fclose(fh);

error:
    if(stream) bsclose(stream);
    if(fh) fclose(fh);
    if(result) bdestroy(result);
    return NULL;
}


TSTree *add_route_data(TSTree *routes, bstring line)
{
    struct bstrList *data = bsplit(line, ' ');
    check(data->qty == 2, "line '%s' does not have 2 columns",
        bdata(line));

    routes = TSTree_insert(routes, 
            bdata(data->entry[0]), blength(data->entry[0]),
            bstrcpy(data->entry[1]));

    bstrListDestroy(data);

    return routes;

error:
    return NULL;
}

TSTree *load_routes(const char *file)
{
    TSTree *routes = NULL;
    bstring line = NULL;
    FILE *routes_map = NULL;

    routes_map = fopen(file, "r");
    check(routes_map != NULL, "Failed to open routes: '%s'", file);

    while((line = bgets((bNgetc)fgetc, routes_map, '\n')) != NULL ) {
        check(btrimws(line) == BSTR_OK, "Failed to trim line.");
        routes = add_route_data(routes, line);
        check(routes != NULL, "Failures to add route.");
        bdestroy(line);
    }

    fclose(routes_map);
    return routes;

error:
    if(routes_map) fclose(routes_map);
    if(line) bdestroy(line);

    return NULL;
}

bstring match_url(TSTree *routes, bstring url)
{
    bstring route = TSTree_search(routes, bdata(url), blength(url));

    if(route == NULL) {
        printf("No exact match found, trying prefix.\n");
        route = TSTree_search_prefix(routes, bdata(url), blength(url));
    }

    return route;
}

bstring read_line(const char *prompt)
{
    printf("%s", prompt);

    bstring result = bgets((bNgetc)fgetc, stdin, '\n');
    check_debug(result != NULL, "stdin closed.");

    check(btrimws(result) == BSTR_OK, "Failed to trim.");

    return result;

error:
    return NULL;
}

void bdestroy_cb(void *value, void *ignored)
{
    // WTF?
    (void)ignored;
    bdestroy((bstring)value);
}

void destroy_routes(TSTree *routes)
{
    TSTree_traverse(routes, bdestroy_cb, NULL);
    TSTree_destroy(routes);
}

int main(int argc, char *argv[])
{
    bstring url = NULL;
    bstring route = NULL;
    check(argc == 2, "USAGE: urlor <urlfile>");

    TSTree *routes = load_routes(argv[1]);
    check(routes != NULL, "Your route file has an error.");

    while(1) {
        url = read_line("URL> ");
        check_debug(url != NULL, "goodbuy");

        route = match_url(routes, url);

        if(route) {
            printf("MATCH: %s == %s\n", bdata(url), bdata(route));
        } else {
            printf("FAIL: %s\n", bdata(url));
        }

        bdestroy(url);

    }

    destroy_routes(routes);
    return 0;

error:
    destroy_routes(routes);
    return 0;
}