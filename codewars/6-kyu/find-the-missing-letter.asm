global find_missing_letter

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

; <--- char find_missing_letter(const char *s, int n) --->
find_missing_letter:
    push rsi
    push rdi
    call impl
    ret

impl:
    beginfn rsi, rbx, rcx
    %define s [rbp+16]
    %define n [rbp+24]

    mov rcx, 0
    mov rsi, s
    xor rax, rax
    xor rbx, rbx
    mov al, byte [rsi+rcx]

.loop:
    inc rcx
    mov bl, byte [rsi+rcx]
    sub rax, rbx
    cmp rax, -1
    je .normal
    sub rbx, 1
    mov rax, rbx
    jmp .endloop
.normal:
    mov rax, rbx
    jmp .loop
.endloop: 
    endfn rsi, rbx, rcx
    ret 16