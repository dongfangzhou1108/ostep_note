/*
0000000000001149 <main>:
    1149:       f3 0f 1e fa             endbr64
    114d:       55                      push   %rbp
    114e:       53                      push   %rbx
    114f:       48 83 ec 08             sub    $0x8,%rsp
    1153:       b8 64 00 00 00          mov    $0x64,%eax
    1158:       83 e8 01                sub    $0x1,%eax
    115b:       75 fb                   jne    1158 <main+0xf>
    115d:       bb 00 00 00 00          mov    $0x0,%ebx
    1162:       48 8d 2d 9b 0e 00 00    lea    0xe9b(%rip),%rbp        # 2004 <_IO_stdin_used+0x4>
    1169:       89 da                   mov    %ebx,%edx
    116b:       48 89 ee                mov    %rbp,%rsi
    116e:       bf 02 00 00 00          mov    $0x2,%edi
    1173:       b8 00 00 00 00          mov    $0x0,%eax
    1178:       e8 d3 fe ff ff          call   1050 <__printf_chk@plt>
    117d:       83 c3 01                add    $0x1,%ebx
    1180:       83 fb 64                cmp    $0x64,%ebx
    1183:       75 e4                   jne    1169 <main+0x20>
    1185:       b8 64 00 00 00          mov    $0x64,%eax
    118a:       83 e8 01                sub    $0x1,%eax
    118d:       75 fb                   jne    118a <main+0x41>
    118f:       b8 00 00 00 00          mov    $0x0,%eax
    1194:       48 83 c4 08             add    $0x8,%rsp
    1198:       5b                      pop    %rbx
    1199:       5d                      pop    %rbp
    119a:       c3                      ret
 */
#include <stdio.h>

int main(int argc, char *argv[]) {
  int array[100];

  for (int i = 0; i < 100; i++)
    array[i] = 1;

  for (int i = 0; i < 100; i++)
    printf("%d\n", i);

  for (int i = 0; i < 100; i++)
    array[i] += 1;

  return 0;
}