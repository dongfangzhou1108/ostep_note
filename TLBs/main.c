#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <threads.h>

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

// https://www.jianshu.com/p/0c519fc6248b
uint64_t current_cycles() {
  uint32_t low, high;
  asm volatile("rdtsc" : "=a"(low), "=d"(high));
  return ((uint64_t)low) | ((uint64_t)high << 32);
}

void bind_core(int i) {
  cpu_set_t cpuset;
  CPU_ZERO(&cpuset);
  CPU_SET(i, &cpuset);
  if (sched_setaffinity(0, sizeof(cpuset), &cpuset) < 0) {
    perror("sched_setaffinity failed");
    exit(EXIT_FAILURE);
  }
}

// argv[1]
int main(int argc, char *argv[]) {

  bind_core(0);

  struct timeval start, end;
  long long elapsed_time;

  struct timespec duration;
  duration.tv_sec = 0;
  duration.tv_nsec = 1e6;
  gettimeofday(&start, NULL);
  thrd_sleep(&duration, NULL);
  gettimeofday(&end, NULL);

  elapsed_time =
      (end.tv_sec - start.tv_sec) * 1e6 + (end.tv_usec - start.tv_usec);

  // printf("Elapsed time: %lld us\n", elapsed_time);

  long long NUMPAGES = atoll(argv[1]);
  long long PAGESIZE = 4096; // getconf PAGE_SIZE
  long long jump = PAGESIZE / sizeof(int);
  long long array_size = NUMPAGES * jump;
  int *array = (int *)malloc(array_size * sizeof(int));
  long long repeat = atoll(argv[2]);

  // for (int j = 0; j < array_size; j += jump)
  //   array[j] = 0;

  uint64_t tick, tick1;
  double time;
  tick = current_cycles();
  for (int i = 0; i < repeat; i += 1) {
    for (int j = 0; j < array_size; j += jump) {
      array[j] += 1;
    }
  }
  tick1 = current_cycles();
  time = ((double)(tick1 - tick) / (double)3.2e6);
  printf("%lf us\n", time);

  return 0;
}