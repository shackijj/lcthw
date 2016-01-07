
void read_file(Handler *handler, bstring filename)
{
    int rc = -1;
    struct bStream *stream = NULL;
    FILE *fh = NULL;
    long fsize = 0;
    bstring result = NULL;

    bstring path = bstrcpy(handler->dir);
    bconcat(path, filename);

    fh = fopen(bdata(path), "r");
    check(fh != NULL, "Can't open file '%s'", bdata(path));

    fseek(fh, 0, SEEK_END);
    fsize = ftell(fh);
    rewind(fh);

    result = bfromcstr("");

    stream = bsopen((bNread) fread, fh);
    check(stream != NULL, "Can't open a stream");

    rc = bsread(result, stream, fsize);
    
    printf("rc: %d, file: %s\n%s", rc, bdata(path), bdata(result));

    bdestroy(path);
    bsclose(stream);
    fclose(fh);
    bdestroy(result);
    return;

error:
    if(path) bdestroy(path);
    if(stream) bsclose(stream);
    if(fh) fclose(fh);
    if(result) bdestroy(result);

    return;
}



