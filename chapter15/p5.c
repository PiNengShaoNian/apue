#include "apue.h"

static void sig_pipe(int);

int main()
{
  int n, fd1[2], fd2[2];
  pid_t pid;
  char line[MAXLINE];

  if (signal(SIGPIPE, sig_pipe) == SIG_ERR)
    err_sys("signal error");

  if (pipe(fd1) < 0 || pipe(fd2) < 0)
    err_sys("pipe error");

  if ((pid = fork()) < 0)
  {
    err_sys("fork error");
  }
  else if (pid > 0)
  {
    close(fd1[0]);
    close(fd2[1]);

    FILE *fpin, *fpout;

    if ((fpin = fdopen(fd2[0], "r")) == NULL)
      err_sys("fdopen error");
    if ((fpout = fdopen(fd1[1], "w")) == NULL)
      err_sys("fdopen error");

    if (setvbuf(fpin, NULL, _IOLBF, 0) < 0)
      err_sys("setvbuf error");
    if (setvbuf(fpout, NULL, _IOLBF, 0) < 0)
      err_sys("setvbuf error");

    while (fgets(line, MAXLINE, stdin) != NULL)
    {
      n = strlen(line);
      if (fputs(line, fpout) == EOF)
        err_sys("fputs error to pipe");

      if (fgets(line, MAXLINE, fpin) == NULL)
      {
        err_msg("child closed pipe");
        break;
      }

      line[n] = 0;
      if (fputs(line, stdout) == EOF)
        err_sys("fputs error");

      exit(0);
    }
  }
  else
  {
    close(fd1[1]);
    close(fd2[0]);
    if (fd1[0] != STDIN_FILENO)
    {
      if (dup2(fd1[0], STDIN_FILENO) != STDIN_FILENO)
        err_sys("dup2 error to stdin");
      close(fd1[0]);
    }

    if (fd2[1] != STDOUT_FILENO)
    {
      if (dup2(fd2[1], STDOUT_FILENO) != STDOUT_FILENO)
        err_sys("sup2 error to stdout");
      close(fd2[1]);
    }

    if (execl("./add2", "add2", (char *)0) < 0)
      err_sys("execl error");
  }
  exit(0);
}

static void sig_pipe(int signo)
{
  printf("SIGPIPE caught\n");
  exit(1);
}