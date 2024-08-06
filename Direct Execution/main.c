#include <fcntl.h>
#define __USE_GNU
#include <sched.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <x86intrin.h>

/*
 * 绑定到同一个核心
 * 一个 write +read系统调用 0byte 执行时间大概 6.25ns (2000 cycle)
 * 一个 write +read系统调用 1byte 执行时间大概 15ns (7000 cylce)
 * 以上在真实文件测试
 * 在pipe上，0byte 1300-1500cycle
 * 1byte  和 0 byte区别不大
 * 【临时结论】：相比于管道（内存），实际文件的读写因为要经过实际IO，所以更慢
 *              在管道上，0/1 byte 测试差距不大
 * 【结论】：本来认为，time_ - 2 * time = 2 * switch, 但结果与此不符合
 *          time_ 2500 time 1400
 * 绑定到不同核心
 * 上下两段时间基本相同
 */

// https://www.jianshu.com/p/0c519fc6248b
uint64_t current_cycles() {
  uint32_t low, high;
  asm volatile("rdtsc" : "=a"(low), "=d"(high));
  return ((uint64_t)low) | ((uint64_t)high << 32);
}

// bind process on concrete CPU core
void bind_core(int i) {
  cpu_set_t cpuset;
  CPU_ZERO(&cpuset);
  CPU_SET(i, &cpuset);
  if (sched_setaffinity(0, sizeof(cpuset), &cpuset) < 0) {
    perror("sched_setaffinity failed");
    exit(EXIT_FAILURE);
  }
}

int main() {
  bind_core(0);

  const char *filename = "example.txt";
  int fd;
  fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU);

  // generate pipe
  int pipe1[2], pipe2[2];
  pipe(pipe1);
  pipe(pipe2);
  char signal = 's';

  // CPU 3.2GHZ
  uint64_t tick, tick1, time;
  tick = current_cycles();
  for (int i = 0; i < 10000; i++) {
    write(pipe1[0], &signal, 1);
    read(pipe1[1], &signal, 1);
  }
  tick1 = current_cycles();
  time = (unsigned)((double)(tick1 - tick) / (double)10000);
  printf("one [write + read 1 byte] system call in %lu CPU cycle\n", time);

  int rc = fork();
  if (rc < 0) { // fork failed; exit
    fprintf(stderr, "fork failed\n");
    exit(1);
  } else if (rc == 0) { // child (new process)

    bind_core(0);
    for (int i = 0; i < 10000; i++) {
      read(pipe1[1], &signal, sizeof(signal)); // OS swith father process
      write(pipe2[0], &signal, sizeof(signal));
    }

  } else { // parent goes down this path (main)
    tick = current_cycles();
    for (int i = 0; i < 10000; i++) {
      write(pipe1[0], &signal, sizeof(signal));
      read(pipe2[1], &signal, sizeof(signal)); // OS switch child process
    }
    tick1 = current_cycles();
    uint64_t time_ = (unsigned)((double)(tick1 - tick) / (double)10000);
    printf("write + read + switch system call in %lu CPU cycle\n", time_);
  }

  return 0;
}
