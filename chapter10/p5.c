#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

struct timer_t
{
  unsigned long started_at; // 设置定时器时的 UNIX 时间戳
  unsigned long ended_at;   // 设置的定时器应该触发的 UNIX 时间戳
  void (*proc)();           // 超时执行的函数
  struct timer_t *next;
};

struct timer_t *head = NULL;

/**
 * 向队列中添加一个定时器
 * 返回最近一个要触发的闹钟时间的余留秒数
 */
unsigned long push_to_timer_list(struct timer_t *node)
{
  unsigned long cur_time;
  time((time_t *)&cur_time);

  struct timer_t *cur = head;

  /* 若当前没有待触发的定时器 */
  if (cur == NULL)
  {
    head = node;
    alarm(node->ended_at - cur_time);
    return (head->ended_at - cur_time);
  }
  else
  {
    /* 如果添加的定时器先于之前的第一个定时器 */
    if (head->ended_at > node->ended_at)
    {
      node->next = head;
      head = node;
      alarm(node->ended_at - cur_time);
      return (node->ended_at - cur_time);
    }
    else
    {
      /* 移动游标 */
      while (cur->next && cur->next->ended_at < node->ended_at)
        cur = cur->next;

      node->next = cur->next;
      cur->next = node;
      return (head->ended_at - cur_time);
    }
  }
}

/**
 * 触发指定的事件并启动下一个定时器
 */
void alarm_proc(int signo)
{
  head = head;
  head->proc();

  if (head->next)
  {
    alarm(head->next->ended_at - head->ended_at);
    struct timer_t *next = head->next;
    free(head);
    head = next;
  }
  else
  {
    free(head);
    head = NULL;
  }
}

void settimeout(unsigned int seconds, void (*proc)())
{
  /* 如果时间为 0 ，则直接执行 */
  if (seconds == 0)
  {
    proc();
    return;
  }

  struct timer_t *t = (struct timer_t *)calloc(1, sizeof(struct timer_t));
  time((time_t *)&(t->started_at));
  t->ended_at = t->started_at + seconds;
  t->proc = proc;

  if (signal(SIGALRM, alarm_proc) == SIG_ERR)
  {
    printf("signal (SIGALRM) error\n");
    exit(0);
  }

  push_to_timer_list(t);
}

void print1() { printf("print1\n"); }

void print2() { printf("print2\n"); }

int main()
{
  settimeout(2, print1);
  settimeout(1, print1);
  settimeout(5, print1);
  settimeout(3, print2);
  printf("main\n");

  for (;;)
    pause();

  return 0;
}