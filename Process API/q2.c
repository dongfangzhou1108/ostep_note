/*
 * 编写一个打开文件的程序（使用open()系统调用），然后调用fork()创建一个新进程。子进
 * 程和父进程都可以访问open()返回的文件描述符吗？当它们并发（即同时）写入文件时，会发
 * 生什么
 */

/*
 * 都可以访问，并发写入时刻会产生乱序
 */

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

void write_file(int fd, int pid) {
  char str[20]; // 足够存储转换后的字符串，包括终止符'\0'和可能的正负号
  const char *cstr;
  sprintf(str, "%d\r\n", pid);
  cstr = str;
  for (int i = 0; i < 100; i++) {
    write(fd, cstr, strlen(cstr));
  }
}

int main(int argc, char *argv[]) {

  const char *filename = "example.txt"; // 要打开的文件名=
  int fd;                               // 文件描述符
  ssize_t written;                      // write 函数返回的字节数

  // 使用 open 系统调用打开文件，O_CREAT 和 O_WRONLY
  // 标志表示创建文件（如果不存在）并写入模式
  fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU);
  if (fd == -1) {
    // 如果 open 调用失败，打印错误消息
    perror("Error opening file");
    return 1;
  }

  int rc = fork();
  if (rc < 0) { // fork failed; exit
    fprintf(stderr, "fork failed\n");
    exit(1);
  } else if (rc == 0) { // child: redirect standard output to a file
    write_file(fd, (int)getpid());
    close(fd);
  } else {
    write_file(fd, (int)getpid()); // parent goes down this path (main)
    close(fd);
  }
  return 0;
}