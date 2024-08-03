/*
 * 对前一个程序稍作修改，这次使用waitpid()而不是wait()。什么时候waitpid()会有用？
 */

/*
 * wait会接收到最先执行完毕的子进程信号并且向下执行
 * 而waitpid可以选择具体需要等待的子进程
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
int main(int argc, char *argv[]) {
  printf("hello world (pid:%d)\n", (int)getpid());
  int pid1 = fork();

  if (pid1 < 0) { // fork failed; exit
    fprintf(stderr, "fork failed\n");
    exit(1);
  } else if (pid1 == 0) { // child (new process)
    sleep(5);
    printf("hello, I am child (pid:%d)\n", (int)getpid());
  } else { // parent goes down this path (main)
    int pid2 = fork();

    if (pid2 == 0) {
      sleep(10);
      printf("hello, I am child (pid:%d)\n", (int)getpid());
    } else {
      // fork1 sleep 5，fork2 sleep 10，wait min(5, 10)
      // fork1 sleep 10，fork2 sleep 5，wait min(5, 10)
      // int wc = wait(NULL);

			// wait until pid2 finish
      int wc = waitpid(pid2, NULL, 0);
      printf("pid1 = %d, pid2 = %d\n", pid1, pid2);
    }
  }
  return 0;
}