#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <stdlib.h>
#include <unistd.h>

void checkLS()
{
  DIR *dp1, *dp2;
  struct dirent *dirp1, *dirp2;

  char *dir1 = ".";
  char *dir2 = "..";

  if ((dp1 = opendir(dir1)) == NULL)
    printf("can't open %s\n", dir1);

  if ((dp2 = opendir(dir2)) == NULL)
    printf("can't open %s\n", dir2);

  while ((dirp1 = readdir(dp1)) != NULL && (dirp2 = readdir(dp2)) != NULL)
  {
    if (strcmp(dirp1->d_name, dirp2->d_name) != 0)
    {
      printf("%s: %s 与 %s: %s 目录内文件不同\n", dir1, dirp1->d_name, dir2, dirp2->d_name);
    }
  }

  printf("%s 与 %s 目录内文件相同\n", dir1, dir2);
  closedir(dp1);
  closedir(dp2);
}

void checkPwd()
{
  char path1[1024], path2[1024];
  realpath(".", path1);
  realpath("..", path2);
}

int main(void)
{
  checkLS();
  checkPwd();
  exit(0);
}