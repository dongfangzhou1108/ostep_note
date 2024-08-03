/*
 * 编写一个创建子进程的程序，然后在子进程中关闭标准输出（STDOUT_FILENO）。如果
 * 子进程在关闭描述符后调用printf()打印输出，会发生什么？
 */

/*
 * 会没有打印信息
 */

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
int main(int argc, char *argv[]) {
  int rc = fork();
  if (rc < 0) { // fork failed; exit
    fprintf(stderr, "fork failed\n");
    exit(1);
  } else if (rc == 0) { // child: redirect standard output to a file
    close(STDOUT_FILENO);

    printf("hhhh\n");
  } else { // parent goes down this path (main)
    int wc = wait(NULL);
  }
  return 0;
}