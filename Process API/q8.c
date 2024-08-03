/*
 * 编写一个程序，创建两个子进程，并使用pipe()系统调用，将一个子进程的标准输出连接
 * 到另一个子进程的标准输入
 */

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  int pipefds[2]; // 存储管道两端文件描述符的数组
  pipe(pipefds);

  int pid1 = fork();
  // pid1
  if (pid1 == 0) {
    int pid2 = fork();
    // pid2
    if (pid2 == 0) {
      // 关闭写端
      close(pipefds[1]);
      // stdin重定向为管道读端
      dup2(pipefds[0], fileno(stdin));

      char str[7];
      scanf("%6s", str);

      printf("In fork2, the string from fork1: %s\n", str);
    }
    // pid1
    else {
      // 关闭读端
      close(pipefds[0]);
      // stdout重定向为管道写端
      dup2(pipefds[1], fileno(stdout));
      printf("hello\n");
    }
  }
  return 0;
}