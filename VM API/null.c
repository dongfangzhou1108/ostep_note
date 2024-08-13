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
 * 1. 正常结束 
 * 2. exited normally
 * 3. 正常结束
		(base) uftpc@uftpc-SER:~/Documents/dfz/OSTEP/VM API$ valgrind --leak-check=yes ./a.out 
		==507243== Memcheck, a memory error detector
		==507243== Copyright (C) 2002-2022, and GNU GPL'd, by Julian Seward et al.
		==507243== Using Valgrind-3.22.0 and LibVEX; rerun with -h for copyright info
		==507243== Command: ./a.out
		==507243== 
		==507243== 
		==507243== HEAP SUMMARY:
		==507243==     in use at exit: 0 bytes in 0 blocks
		==507243==   total heap usage: 0 allocs, 0 frees, 0 bytes allocated
		==507243== 
		==507243== All heap blocks were freed -- no leaks are possible
		==507243== 
		==507243== For lists of detected and suppressed errors, rerun with: -s
		==507243== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
 * 4.
		==508256== Memcheck, a memory error detector
		==508256== Copyright (C) 2002-2022, and GNU GPL'd, by Julian Seward et al.
		==508256== Using Valgrind-3.22.0 and LibVEX; rerun with -h for copyright info
		==508256== Command: ./a.out
		==508256== 
		==508256== 
		==508256== HEAP SUMMARY:
		==508256==     in use at exit: 4 bytes in 1 blocks
		==508256==   total heap usage: 1 allocs, 0 frees, 4 bytes allocated
		==508256== 
		==508256== 4 bytes in 1 blocks are definitely lost in loss record 1 of 1
		==508256==    at 0x4846828: malloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
		==508256==    by 0x109165: main (in /home/uftpc/Documents/dfz/OSTEP/VM API/a.out)
		==508256== 
		==508256== LEAK SUMMARY:
		==508256==    definitely lost: 4 bytes in 1 blocks
		==508256==    indirectly lost: 0 bytes in 0 blocks
		==508256==      possibly lost: 0 bytes in 0 blocks
		==508256==    still reachable: 0 bytes in 0 blocks
		==508256==         suppressed: 0 bytes in 0 blocks
		==508256== 
		==508256== For lists of detected and suppressed errors, rerun with: -s
		==508256== ERROR SUMMARY: 1 errors from 1 contexts (suppressed: 0 from 0)
 * 5. 不正确
		(base) uftpc@uftpc-SER:~/Documents/dfz/OSTEP/VM API$ valgrind --leak-check=yes ./a.out 
		==508752== Memcheck, a memory error detector
		==508752== Copyright (C) 2002-2022, and GNU GPL'd, by Julian Seward et al.
		==508752== Using Valgrind-3.22.0 and LibVEX; rerun with -h for copyright info
		==508752== Command: ./a.out
		==508752== 
		==508752== 
		==508752== HEAP SUMMARY:
		==508752==     in use at exit: 400 bytes in 1 blocks
		==508752==   total heap usage: 1 allocs, 0 frees, 400 bytes allocated
		==508752== 
		==508752== 400 bytes in 1 blocks are definitely lost in loss record 1 of 1
		==508752==    at 0x4846828: malloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
		==508752==    by 0x109165: main (null.c:57)
		==508752== 
		==508752== LEAK SUMMARY:
		==508752==    definitely lost: 400 bytes in 1 blocks
		==508752==    indirectly lost: 0 bytes in 0 blocks
		==508752==      possibly lost: 0 bytes in 0 blocks
		==508752==    still reachable: 0 bytes in 0 blocks
		==508752==         suppressed: 0 bytes in 0 blocks
		==508752== 
		==508752== For lists of detected and suppressed errors, rerun with: -s
		==508752== ERROR SUMMARY: 1 errors from 1 contexts (suppressed: 0 from 0)
 * 6.
		==509210== Memcheck, a memory error detector
		==509210== Copyright (C) 2002-2022, and GNU GPL'd, by Julian Seward et al.
		==509210== Using Valgrind-3.22.0 and LibVEX; rerun with -h for copyright info
		==509210== Command: ./a.out
		==509210== 
		==509210== Invalid read of size 4
		==509210==    at 0x1091D0: main (null.c:89)
		==509210==  Address 0x4a831cc is 396 bytes inside a block of size 400 free'd
		==509210==    at 0x484988F: free (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
		==509210==    by 0x1091C5: main (null.c:88)
		==509210==  Block was alloc'd at
		==509210==    at 0x4846828: malloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
		==509210==    by 0x1091A5: main (null.c:84)
		==509210== 
		100
		==509210== 
		==509210== HEAP SUMMARY:
		==509210==     in use at exit: 0 bytes in 0 blocks
		==509210==   total heap usage: 2 allocs, 2 frees, 1,424 bytes allocated
		==509210== 
		==509210== All heap blocks were freed -- no leaks are possible
		==509210== 
		==509210== For lists of detected and suppressed errors, rerun with: -s
		==509210== ERROR SUMMARY: 1 errors from 1 contexts (suppressed: 0 from 0)
 * 7. malloc 分配内存段，不允许 free 其中一部分
 * 		编译通过(有warning)
				null.c: In function ‘main’:
				null.c:115:9: warning: ‘free’ called on pointer ‘p’ with nonzero offset 396 [-Wfree-nonheap-object]
					115 |         free(p + 99);
							|         ^~~~~~~~~~~~
				null.c:108:18: note: returned from ‘malloc’
					108 |         int *p = malloc(100 * sizeof(int));
 *    运行报错
				free(): invalid pointer
				Aborted (core dumped)
 * 		gdb
				free(): invalid pointer
				Program received signal SIGABRT, Aborted.
 *		valgrind
				==509876== Memcheck, a memory error detector
				==509876== Copyright (C) 2002-2022, and GNU GPL'd, by Julian Seward et al.
				==509876== Using Valgrind-3.22.0 and LibVEX; rerun with -h for copyright info
				==509876== Command: ./a.out
				==509876== 
				==509876== Invalid free() / delete / delete[] / realloc()
				==509876==    at 0x484988F: free (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
				==509876==    by 0x1091CB: main (null.c:125)
				==509876==  Address 0x4a831cc is 396 bytes inside a block of size 400 alloc'd
				==509876==    at 0x4846828: malloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
				==509876==    by 0x1091A5: main (null.c:118)
				==509876== 
				100
				==509876== 
				==509876== HEAP SUMMARY:
				==509876==     in use at exit: 400 bytes in 1 blocks
				==509876==   total heap usage: 2 allocs, 2 frees, 1,424 bytes allocated
				==509876== 
				==509876== 400 bytes in 1 blocks are definitely lost in loss record 1 of 1
				==509876==    at 0x4846828: malloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
				==509876==    by 0x1091A5: main (null.c:118)
				==509876== 
				==509876== LEAK SUMMARY:
				==509876==    definitely lost: 400 bytes in 1 blocks
				==509876==    indirectly lost: 0 bytes in 0 blocks
				==509876==      possibly lost: 0 bytes in 0 blocks
				==509876==    still reachable: 0 bytes in 0 blocks
				==509876==         suppressed: 0 bytes in 0 blocks
				==509876== 
				==509876== For lists of detected and suppressed errors, rerun with: -s
				==509876== ERROR SUMMARY: 2 errors from 2 contexts (suppressed: 0 from 0)
 * 9.
			使用 realloc 的效率是比 malloc要高的，原因可能是：使用 realloc() 可以预先分配较大的内存块，减少内存分配次数？
 */

typedef struct ListNode {
    int data;              
    struct ListNode *next; 
} ListNode;

ListNode* createNode(int data) {
  ListNode *newNode = (ListNode*)malloc(sizeof(ListNode));
  if (newNode == NULL) {
    perror("Error allocating memory for new node");
    exit(EXIT_FAILURE);
  }
  newNode->data = data;
  newNode->next = NULL;
  return newNode;
}

ListNode* appendNode(ListNode **tail, int data) {
	ListNode *newNode = createNode(data);
	(*tail)->next = newNode;

	return newNode;
}

void freeList(ListNode *head) {
	ListNode *temp;
	while (head != NULL) {
		temp = head;
		head = head->next;
		free(temp);
	}
}

uint64_t current_cycles() {
  uint32_t low, high;
  asm volatile("rdtsc" : "=a"(low), "=d"(high));
  return ((uint64_t)low) | ((uint64_t)high << 32);
}

int main(int argc, char *argv[]) {
	/*
  int *p = NULL;
  free(p);
	*/

	// int *p = malloc(sizeof(int));

	// 5
	/*
	int *p = malloc(100 * sizeof(int));
	*(p + 99) = 100;
	*/

	// 6
	// free(p);

	// 7
	/*
	free(p + 99);
	printf("%d\n", *(p + 99));
	*/

	// 8
	uint64_t tick, tick1, time;

  tick = current_cycles();
	ListNode *head = createNode(0);
	ListNode *list_tail = head;
	for(int i = 1; i < 100; i++) {
		list_tail = appendNode(&list_tail, i);
	}
	freeList(head);
  tick1 = current_cycles();
  time = tick1 - tick;
	printf("ListNode: %lu\n",time);

  tick = current_cycles();
	int *vec_head = malloc(sizeof(int));
	*vec_head = 0;
	for(int i = 1; i < 100; i++) {
		vec_head = realloc(vec_head, (i + 1) * sizeof(int));
		*(vec_head + i) = i;
	}
	free(vec_head);
  tick1 = current_cycles();
  time = tick1 - tick;
	printf("realloc: %lu\n",time);

  return 0;
}