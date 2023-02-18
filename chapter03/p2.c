#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

struct Node
{
    int fd;
    struct Node *next;
};

void free_fd(struct Node *head)
{
    struct Node *cur = head;
    while (cur)
    {
        struct Node *next = cur->next;
        close(cur->fd);
        printf("close fd %d\n", cur->fd);
        free(cur);
        cur = next;
    }
}

int my_dup2(int fd, int fd2)
{
    if (fd == fd2)
        return fd;

    close(fd2);

    struct Node head = {0};
    struct Node *cur = &head;
    int fd_temp;
    while ((fd_temp = dup(fd)) != fd2)
    {
        printf("create fd %d\n", fd_temp);
        if (fd_temp == -1)
        {
            free_fd(head.next);
            return -1;
        }

        struct Node *temp = (struct Node *)malloc(sizeof(struct Node));
        temp->fd = fd_temp;
        temp->next = NULL;
        cur->next = temp;
        cur = cur->next;
    }

    free_fd(head.next);
    return fd2;
}

int main()
{
    int fd = my_dup2(STDOUT_FILENO, 15);
    write(15, "Hello World!\n", 13);
    return 0;
}