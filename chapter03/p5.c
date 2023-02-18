#include <unistd.h>

int main()
{
    write(STDOUT_FILENO, "Hello ", 7);
    write(STDERR_FILENO, "World!\n", 9);
    return 0;
}

/**
 * 第一种
 * dup2(fileno, STDOUT_FILENO);  //  STDOUT_FILENO 此时等于对应文件的描述符
 * dup2(STDOUT_FILENO, STDERR_FILENO);  // STDERR_FILENO 等于 STDOUT_FILENO ，也就是等于对应文件的描述符，因此此时 stdout 和 stderr 都会被输出到文件
 * 第二种
 * dup2(STDERR_FILENO, STDOUT_FILENO);  // STDERR_FILENO 等于 STDOUT_FILENO ， stderr 会被输出到标准输出
 * dup2(STDOUT_FILENO, fileno);  // STDOUT_FILENO 等于 fileno ， stdout 会被输出到文件
 */