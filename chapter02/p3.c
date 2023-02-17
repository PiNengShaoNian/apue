#include "apue.h"
#include <bits/resource.h>
#include <errno.h>
#include <limits.h>

#ifdef OPEN_MAX
static long openmax = OPEN_MAX;
#else
static long openmax = 0;
#endif

#define OPEN_MAX_GUESS 256

/**
 * 改写图2-17中的程序，使其在 sysconf 为 OPEN_MAX 限制返回 LONG_MAX 时，避免进行不必要的处理
 * 在 sysconf 没有能够准确获得 OPEN_MAX 限制的时候，可以使用 getrlimit 来获得。
 */
long open_max()
{
  struct rlimit rl;
  if (openmax == 0)
    errno = 0;

  if ((openmax = sysconf(_SC_OPEN_MAX)) < 0 || openmax == LONG_MAX)
  {
    if ((openmax = getrlimit(RLIMIT_NOFILE, &rl)) < 0)
      err_sys("can not get file limit\n");
    else if (openmax == RLIM_INFINITY)
      openmax = OPEN_MAX_GUESS;
    else
      openmax = rl.rlim_max;
  }

  return (openmax);
}