global nth_fib

section .text

; https://www.codewars.com/kata/522551eee9abb932420004a0
; <--- unsigned long long nth_fib(int n) --->
nth_fib:
    movsx rdi, edi
    cmp rdi, 1
    jg .two
    mov rax, 0
    ret
.two:
    cmp rdi, 2
    jg .begin
    mov rax, 1
    ret
.begin:
    mov rcx, 2
    mov r8, 0 ; a
    mov r9, 1 ; b
.loop:
    cmp rcx, rdi
    jge .done
    mov r10, r9
    add r10, r8
    mov r8, r9
    mov r9, r10
    inc rcx
    jmp .loop
.done:
    mov rax, r9
    ret
; ---------> endof nth_fib <---------
