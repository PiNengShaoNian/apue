#include "apue.h"

int main(void)
{
  char name[L_tmpnam], line[MAXLINE];
  FILE *fp;

  printf("%s\n", tmpnam(NULL)); /* first temp name */

  tmpnam(name);
  printf("%s\n", name);

  if ((fp = tmpfile()) == NULL)
    err_sys("tmpfile error"); /* create temp file */

  fputs("one line of output\n", fp);
  rewind(fp);
  if ((fgets(line, MAXLINE, fp)) == NULL)
    err_sys("fgets error");
  fputs(line, stdout);

  exit(0);
}