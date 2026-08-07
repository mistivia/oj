;; https://www.codewars.com/kata/526c7363236867513f0005ca

global is_leap_year

section .text

; bool is_leap_year(unsigned year)
is_leap_year:
    xor rdx, rdx
    mov rax, rdi
    mov rcx, 4
    idiv rcx
    cmp rdx, 0
    jnz .false
    xor rdx, rdx
    mov rax, rdi
    mov rcx, 100
    idiv rcx
    cmp rdx, 0
    jnz .true
    xor rdx, rdx
    mov rax, rdi
    mov rcx, 400
    idiv rcx
    cmp rdx, 0
    jz .true
.false:
    mov rax, 0
    ret
.true:
    mov rax, 1
    ret