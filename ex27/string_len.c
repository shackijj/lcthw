#include <stdio.h>
#include <getopt.h>
#include <string.h>
#include <stdlib.h>

#define PACKAGE "strl"
#define VERSION "1.0.0"

void print_help(int exval);

int main(int argc, char *argv[]) {
 FILE *fp = stdin;
 char line[1024];
 int opt = 0;
 int strip_newline = 0;

 while((opt = getopt(argc, argv, "hvs")) != -1) {
  switch(opt) {
   case 'h':
    print_help(0);
    break;
   case 'v':
    fprintf(stdout, "%s %s\n", PACKAGE, VERSION); 
    exit(0);
    break;
   case 's':
    strip_newline = 1;
    break;
   case '?':
    fprintf(stderr, "%s: Error - No such option: `%c'\n\n", PACKAGE, optopt);
    print_help(1);
    break;
  } /* switch */
 } /* while */

 if((argc - optind) == 0) {
  while((fgets(line, 1024, fp)) != NULL) {


   if(strip_newline == 1)
    if(line[strlen(line) - 1] == '\n') 
     line[strlen(line) - 1] = '\0';

   printf("%4d %s\n", strlen(line), line);
  } /* while */
 } else {
  for(; optind < argc; optind++) {
   if(freopen(argv[optind], "r", fp) == NULL) {
    perror(argv[optind]);
    continue;
   }

   while((fgets(line, 1024, fp)) != NULL) {
    if(strip_newline == 1)
     if(line[strlen(line) - 1] == '\n') 
      line[strlen(line) - 1] = '\0';

    printf("%4d %s\n", strlen(line), line);
   } /* while */
  } /* for */
 } /* else */

 return 0;
}

void print_help(int exval) {
 printf("%s,%s compute string length\n", PACKAGE, VERSION);
 printf("Usage: %s [-h] [-v] [-s] FILE1 FILE2 ...\n\n", PACKAGE);

 printf("Options:\n");
 printf("  -h    print this help and exit\n");
 printf("  -v    print version and exit\n");
 printf("  -s    strip trailing newlines\n\n");

 printf(" If no filenames given, strings\n");
 printf(" are read from stdin().\n\n");

 printf(" This program does basicly the same as\n");
 printf(" `${#varname}' in the GNU/Linux bash shell.\n\n");

 exit(exval);
}
