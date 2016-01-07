#include <lcthw/dbg.h>
#include <lcthw/tstree.h>
#include <lcthw/bstrlib.h>
#include <stdlib.h>
#include <stdio.h>
#include <dlfcn.h>

typedef void (*handler_fuct) ();

typedef struct Handler {
    bstring url;
    bstring name;
    bstring lib_file;
    bstring func_name;
    handler_fuct func;
    void *lib;
} Handler;

TSTree *add_route_data(TSTree *routes, bstring line)
{
    const char *dl_error;
    struct bstrList *data = bsplit(line, ' ');

    check(data->qty == 4, "line '%s' does not have 4 columns",
        bdata(line));

    Handler *handler = malloc(sizeof(Handler));

    handler->url = bstrcpy(data->entry[0]);
    handler->name = bstrcpy(data->entry[1]);
    handler->lib_file = bstrcpy(data->entry[2]);
    handler->func_name = bstrcpy(data->entry[3]); 

    handler->lib = dlopen(bdata(handler->lib_file), RTLD_NOW);    
    check(handler->lib != NULL, "Failed to open lib: %s, reason: %s", bdata(handler->lib_file), dlerror());

    handler->func = (handler_fuct) dlsym(handler->lib, bdata(handler->func_name)); // locate hello fuct    
    dl_error = dlerror();
    check(dl_error == NULL, "Failed to locate hello function, reason: %s", dl_error);    

    routes = TSTree_insert(routes, 
            bdata(data->entry[0]), blength(data->entry[0]),
            handler);

    bstrListDestroy(data);

    return routes;

error:
    bstrListDestroy(data);
    return NULL;
}

TSTree *load_routes(const char *file)
{
    TSTree *routes = NULL;
    bstring line = NULL;
    FILE *routes_map = NULL;
    int c = 0;

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

Handler *match_url(TSTree *routes, bstring url)
{

    debug("url %s length %d", bdata(url), blength(url));
    Handler *route = TSTree_search(routes, bdata(url), blength(url));

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
    check(blength(result) != 0, "Got empty line");

    return result;

error:
    if(result) bdestroy(result);
    return NULL;
}

void handler_destroy_cb(void *value, void *ignored)
{
    // WTF?
    (void)ignored;
    Handler *handler = value;
    bdestroy(handler->lib_file);
    bdestroy(handler->url);
    bdestroy(handler->name);    
    bdestroy(handler->func_name);
    dlclose(handler->lib);
    free(handler);
}

void destroy_routes(TSTree *routes)
{
    TSTree_traverse(routes, handler_destroy_cb, NULL);
    TSTree_destroy(routes);
}

int main(int argc, char *argv[])
{
    bstring f_content = NULL;
    bstring url = NULL;
    Handler *route = NULL;
    check(argc == 2, "USAGE: urlor <urlfile>");

    TSTree *routes = load_routes(argv[1]);
    check(routes != NULL, "Your route file has an error.");

    while(1) {
        url = read_line("URL> ");
        check_debug(url != NULL, "goodbuy");

        route = match_url(routes, url);

        if(route) {
            printf("MATCH: '%s' == '%s'\n", bdata(url), bdata(route->name));
            //read_file(route, url);
            route->func();
        } else {
            printf("FAIL: %s\n", bdata(url));
        }

        bdestroy(url);

    }

    destroy_routes(routes);
    return 0;

error:
    if(url) bdestroy(url);
    destroy_routes(routes);
    return 0;
}
