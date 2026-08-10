;; https://www.codewars.com/kata/5526fc09a1bbd946250002dc

global find_outlier

%macro beginfn 1-*
    push rbp
    mov  rbp, rsp
%rep %0
    push %1
    %rotate 1
%endrep
%endmacro

%macro endfn 1-*
%rep %0
    %rotate -1
    pop %1
%endrep
    pop  rbp
%endmacro

section .text

; <----- int find_outlier(const int *v, unsigned len) ----->
find_outlier:
    push rbp
    mov rbp, rsp

    push rsi
    push rdi
    call impl

    pop rbp
    ret
; ---------> end of find_outl <---------

normal_bit:
    %define v [rbp + 16]
    beginfn rsi, rcx, rdx, rbx

    mov rsi, v
    mov rcx, 0
    mov rdx, 0
    
    mov rbx, 0
    movsx rax, dword [rsi + rcx * 4]
    xor rbx, rbx
    test rax, 1
    setz bl
    add rdx, rbx
    
    inc rcx
    mov rbx, 0
    movsx rax, dword [rsi + rcx * 4]
    xor rbx, rbx
    test rax, 1
    setz bl
    add rdx, rbx
    
    inc rcx
    mov rbx, 0
    movsx rax, dword [rsi + rcx * 4]
    xor rbx, rbx
    test rax, 1
    setz bl
    add rdx, rbx

    xor rax, rax
    cmp rdx, 1
    setle al

    endfn rsi, rcx, rdx, rbx
    ret 8

impl:
    %define v [rbp + 16]
    %define len [rbp + 24]
    beginfn rdi, rsi, rcx, r8, r9

    mov rax, v
    push rax
    call normal_bit

    mov rdi, rax
    mov rcx, 0
    mov rsi, v
.loop:
    cmp rcx, len
    jge .endloop
    movsx r8, dword [rsi + rcx * 4]
    mov r9, r8
    and r9, 1
    cmp r9, rdi
    cmovnz rax, r8
    jnz .endloop
    inc rcx
    jmp .loop
.endloop:
    endfn rdi, rsi, rcx, r8, r9
    ret 16