/*
 * 使用fork()编写另一个程序。子进程应打印“hello”，父进程应打印“goodbye”。你应该
 * 尝试确保子进程始终先打印。你能否不在父进程调用wait()而做到这一点呢？
 */

/*
 * 子进程打印后通过管道发送消息，父进程接收消息后打印，保证打印顺序
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
  int pipefds[2]; // 存储管道两端文件描述符的数组
  pid_t pid;
  char signal = 's'; // 用作信号的字符

  // 创建管道
  if (pipe(pipefds) == -1) {
    perror("pipe");
    exit(EXIT_FAILURE);
  }

  // 创建子进程
  pid = fork();
  if (pid == -1) {
    perror("fork");
    exit(EXIT_FAILURE);
  }

  if (pid == 0) {
    // 子进程部分
    close(pipefds[0]); // 关闭读端

    // 子进程打印消息
    printf("Child process says: hello\n");

    // 子进程写入信号到管道的写端
    if (write(pipefds[1], &signal, sizeof(signal)) < 0) {
      perror("write");
    }

    close(pipefds[1]);  // 关闭写端
    exit(EXIT_SUCCESS); // 子进程退出
  } else {
    // 父进程部分
    close(pipefds[1]); // 关闭写端

    // 父进程从管道的读端读取信号
    if (read(pipefds[0], &signal, sizeof(signal)) < 0) {
      perror("read");
    }

    // 父进程打印消息
    printf("Parent process says: goodbye\n");

    close(pipefds[0]); // 关闭读端
  }

  return 0;
}