/*
 * 现在编写一个程序，在父进程中使用wait()，等待子进程完成。wait()返回什么？如果你在
 * 子进程中使用wait()会发生什么？
 */

/*
 * wait 在父进程返回子进程PID, 子线程调用wait将会返回-1
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
int main(int argc, char *argv[]) {
  printf("hello world (pid:%d)\n", (int)getpid());
  int rc = fork();
  if (rc < 0) { // fork failed; exit
    fprintf(stderr, "fork failed\n");
    exit(1);
  } else if (rc == 0) { // child (new process)
    int wc = wait(NULL);
    printf("wc = %d\n", wc);
    printf("hello, I am child (pid:%d)\n", (int)getpid());
  } else { // parent goes down this path (main)
    printf("rc = %d", rc);
    int wc = wait(NULL);
    printf("hello, I am parent of %d (wc:%d) (pid:%d)\n", rc, wc,
           (int)getpid());
  }
  return 0;
}