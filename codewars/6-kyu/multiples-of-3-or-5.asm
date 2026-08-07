global mul3or5

section .text

; <----- unsigned mul3or5(int num) ----->
mul3or5:
    movsxd rdi, edi
    mov r8, 0 ;; i
    mov rsi, 0 ;; sum
.loop:
    inc r8
    cmp r8, rdi
    jge .end
    xor rdx, rdx
    mov rax, r8
    mov rcx, 3
    idiv rcx
    cmp rdx, 0
    jz .add
    xor rdx, rdx
    mov rax, r8
    mov rcx, 5
    idiv rcx
    cmp rdx, 0
    jz .add
    jmp .loop
.add:
    add rsi, r8
    jmp .loop
.end:
    mov rax, rsi
    ret
; ---------> end of mul3or5 <---------