#include <lcthw/bstrlib.h>
#include "minunit.h"
#include <string.h>

const char *test_str1 = "test1";
const char *test_str2 = "test2";
const char *cstr = "cstring";

bstring a = NULL;
bstring b = NULL;


char *test_bfromcstr()
{
    //Create a bstring from a C style constant.
    a = bfromcstr(cstr);
    mu_assert(a != NULL, "Bstring is NULL");
    mu_assert(a->slen == (int) strlen(cstr), "bstring has wrong length");
    mu_assert(strcmp((const char *)a->data, cstr) == 0, "bstring has wrong value");

    return NULL;
}


char *test_blk2bstr()
{
    //Same but give the length of the buffer.
    size_t clen = (size_t) strlen(cstr);
    void *blk = malloc(clen);
    memcpy(blk, cstr, clen);

    b = blk2bstr(blk, clen);   

    mu_assert(b != NULL, "Bstring is NULL");
    mu_assert(b->slen == (int) clen, "Bstring has wrong length.");
    mu_assert(strcmp((const char *)b->data, cstr) == 0, "bstring has wrong value");

    free(blk);
    
    return NULL;
}
char *test_bstrcpy()
{
    //Copy a bstring.
    bstring c = bstrcpy(a);
    mu_assert(c != NULL, "bstrcpy returned NULL.");
    mu_assert(c->slen == a->slen, "bstrcpy returned wrong len.");
    mu_assert(strcmp((const char *)c->data, (const char *) a->data) == 0, "wrong data.");

    bdestroy(c);    

    return NULL;
}
char *test_bassign()
{
    //Set one bstring to another.
   
    mu_assert(bassign(a, b) == BSTR_OK, "bassinn return error.");
    mu_assert(a->slen == b->slen, "wrong slen");
    mu_assert(strcmp((const char *)a->data, (const char *)b->data) == 0, "wrong data");

    return NULL;
}
char *test_bassigncstr()
{
    //Set a bstring to a C string's contents.
    mu_assert(bassigncstr(a, test_str1) == BSTR_OK, "returned error");
    mu_assert(strcmp((const char *) a->data, test_str1) == 0, "wrong data");
    mu_assert(a->slen == (int) strlen(test_str1), "wrong len");
   
    return NULL;
}
char *test_bassignblk()
{
    //Set a bstring to a C string but give the length.
    size_t clen = strlen(test_str1);
    void *blk = malloc(clen);
    memcpy(blk, test_str1, clen);

    mu_assert(bassignblk(a, blk, clen) == BSTR_OK, "retured error.");
    mu_assert(a->slen == (int) clen, "wrong length.");
    mu_assert(strcmp((const char *) a->data, test_str1) == 0, "wrong_data.");
    free(blk);

    return NULL;
}
char *test_bdestroy()
{
    //Destroy a bstring.
    mu_assert(bdestroy(a) == BSTR_OK, "bdestroy failed.");
    mu_assert(bdestroy(b) == BSTR_OK, "bdestroy failed.");
    return NULL;
}
char *test_bconcat()
{
    //Concatenate one bstring onto another.
    bassigncstr(a, test_str1);
    bassigncstr(b, test_str2);

    mu_assert(bconcat(a, b) == BSTR_OK, "returned error");
    mu_assert(strcmp((const char *)a->data, "test1test2") == 0, "wrong data");
 
    return NULL;
}
char *test_bstricmp()
{
    //Compare two bstrings returning the same result as strcmp.
    bassigncstr(a, test_str1);
    bassigncstr(b, test_str2);

    mu_assert(bstricmp(a, b) < 0, "wrong cmp");
    mu_assert(bstricmp(b, a) > 0, "wrong cmp");

    bassigncstr(b, test_str1);
    
    mu_assert(bstricmp(a, b) == 0, "wrong cmp");
    
    return NULL;
}
char *test_biseq()
{
    mu_assert(biseq(a, b) == 1, "wrong cmp");
    return NULL;
}
char *test_binstr()
{
    //Tells if one bstring is in another.
    bassigncstr(a, "1");
    mu_assert(binstr(b, 0, a) == 4, "wrong position");

    
    bassigncstr(a, "5");
    mu_assert(binstr(b, 0, a) == BSTR_ERR, "should be error");
    
    return NULL;
}
char *test_bfindreplace()
{
    //Find one bstring in another then replace it with a third.
    bassigncstr(a, test_str1);
    bassigncstr(b, test_str1);
    bstring repl = bfromcstr(test_str2);

    mu_assert(bfindreplace(b, a, repl, 0) == BSTR_OK, "returned error");
    mu_assert(biseq(b, repl) == 1, "wrong replace");
    bdestroy(repl);
  
    return NULL;
}
char *test_bsplit()
{
    //How to split a bstring into a bstrList

    struct bstrList *list = bsplit(a, 's');
    mu_assert(list != NULL, "bsplit returned NULL");
    mu_assert(list->qty == 2, "wrong list size");
    
    bstring te = bfromcstr("te");
    bstring t1 = bfromcstr("t1");
 
    mu_assert(biseq(list->entry[0], te) == 1, "wrong first");
    mu_assert(biseq(list->entry[1], t1) == 1, "wrong second");

    bdestroy(te);
    bdestroy(t1);

    bstrListDestroy(list);

    return NULL;
}
char *test_bformat()
{
    //Doing a format string, super handy.
    bstring c = bformat("%d, %s, %c", 1, "test", 'c');
    bassigncstr(a, "1, test, c");

    mu_assert(biseq(a, c) == 1, "wrong content");

    bdestroy(c);

    return NULL;
}
char *test_blength()
{
    //Getting the length of a bstring.
    bassigncstr(a, test_str1);
    
    mu_assert(blength(a) == (int) strlen(test_str1), "wrong length");

    return NULL;
}
char *test_bdata()
{
    //Getting the data from a bstring.
    mu_assert((char *)a->data ==  bdata(a), "wrong data");

    return NULL;
}
char *test_bchar()
{
    //Getting a char from a bstring. 
    mu_assert(bchar(a, 0) == 't', "wrong char");
    return NULL;
}

char *all_tests()
{
    mu_suite_start();
    mu_run_test(test_bfromcstr);
    mu_run_test(test_blk2bstr);
    mu_run_test(test_bstrcpy);
    mu_run_test(test_bassign);
    mu_run_test(test_bassigncstr);
    mu_run_test(test_bassignblk);
    mu_run_test(test_bconcat);
    mu_run_test(test_bstricmp);
    mu_run_test(test_biseq);
    mu_run_test(test_binstr);
    mu_run_test(test_bfindreplace);
    mu_run_test(test_bsplit);
    mu_run_test(test_bformat);
    mu_run_test(test_blength);
    mu_run_test(test_bdata);
    mu_run_test(test_bchar);
    mu_run_test(test_bdestroy);

    return NULL;
}

RUN_TESTS(all_tests);
