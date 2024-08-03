/*
 * 编写一个调用fork()的程序。在调用fork()之前，让主进程访问一个变量（例如x）并将其值
 * 设置为某个值（例如100）。子进程中的变量有什么值？当子进程和父进程都改变x的值时，
 * 变量会发生什么
 */

/*
 * 子进程x和fork前相同，fork前后进程不占用相同内存，所以修改互不影响
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  printf("hello world (pid:%d)\n", (int)getpid());

  int x = 100;

  int rc = fork();
  if (rc < 0) { // fork failed; exit
    fprintf(stderr, "fork failed\n");
    exit(1);
  } else if (rc == 0) { // child (new process)
    printf("hello, I am child (pid:%d)\n", (int)getpid());
    printf("x = %d\n", x);
  } else { // parent goes down this path (main)
    wait(NULL);
    x = 10;
    printf("hello, I am parent of %d (pid:%d)\n", rc, (int)getpid());
    printf("x = %d\n", x);
  }
  return 0;
}
