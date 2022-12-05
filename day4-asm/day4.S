/* SPDX-License-Identifier: GPL-2.0 */
#define SCORE %rbx

.section .rodata
str: .asciz "%d\n"

.text
read_num:
    xor %eax, %eax
1:
    movzbl (%rdi), %ecx
    cmp $'0', %cl
    jb 1f
    cmp $'9', %cl
    ja 1f
    sub $'0', %cl
    imul $10, %eax
    add %ecx, %eax
    inc %rdi
    jmp 1b
1:
    ret

/* Out params %rdi, %rsi, %rdx, %rcx - retval %rax */
/* does not use the SYSV calling convention */
read_pairs:
    call read_num
    push %rax
    inc %rdi
    call read_num
    mov %rax, %rsi
    inc %rdi
    call read_num
    mov %rax, %rdx
    inc %rdi
    call read_num
    mov %rax, %rcx
    pop %rdi
    xor %eax, %eax
    ret
    
.global main
main:
    push %rbp
    mov %rsp, %rbp
    sub $128, %rsp
    xor %ebx, %ebx
1:
    mov %rsp, %rdi
    mov $128, %esi
    mov stdin(%rip), %rdx
    call fgets@PLT
    test %rax, %rax
    jz 2f
    mov %rsp, %rdi
    call read_pairs
    /* x1 = rdi, y1 = rsi, x2 = rdx, y2 = rcx */
    /* (x1 <= y2 && x2 <= y1) */
    cmp %rcx, %rdi
    ja 1b
    cmp %rsi, %rdx
    ja 1b
    inc %ebx
    jmp 1b
2:
    lea str(%rip), %rdi
    mov %ebx, %esi
    call printf
    mov %rbp, %rsp
    pop %rbp
    ret
