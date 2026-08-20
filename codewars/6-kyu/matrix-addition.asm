;; https://www.codewars.com/kata/526233aefd4764272800036f
global mtxadd
extern malloc

section .text

%define t0 r8
%define t0d r8d
%define t1 r9

; <--- [dword RAX] mtxadd(ro [dword RDI] mxa, ro [dword RSI] mxb, RDX n) --->
; Note:
;    <mxa> and <mxb> is 32-byte memory aligned
mtxadd:
    push rbp
    mov rbp, rsp
    
    %define mxa r12
    push r12
    mov r12, rdi
    
    %define mxb r13
    push r13
    mov r13, rsi
    
    %define n rbx
    push rbx
    mov rbx, rdx
    
    %define res r14
    push r14
    
    %define i r15
    push r15
    xor i, i ;; i = 0
    sub rsp, 8
    
    imul n, n ; n is n^2 now
    ; res = malloc(n * n * 4)
    mov t0, n
    shl t0, 2
    mov rdi, t0
    call malloc
    mov res, rax
    
    
.loop:
    ;; while i < n
    cmp i, n
    jge .endloop
        movsx t0, dword [mxa + i * 4]
        movsx t1, dword [mxb + i * 4]
        add t0, t1
        mov dword [res + i * 4], t0d
        inc i
        jmp .loop
.endloop:
    mov rax, res
    add rsp, 8
    pop r15
    pop r14
    pop rbx
    pop r13
    pop r12
    mov rsp, rbp
    pop rbp
    ret
    %undef n
    %undef i
    %undef mxa
    %undef mxb
    %undef res
; -----> endof mtxend <-----
