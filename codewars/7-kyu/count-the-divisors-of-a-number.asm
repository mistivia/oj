global divisors

section .text
      
divisors:
    push rbp
    mov rbp, rsp
    push rbx
    
    mov rsi, 0 ; i
    mov rbx, 0 ; cnt
.loop:
    inc rsi
    cmp rsi, rdi
    jg .end
    mov rax, rdi
    xor rdx, rdx
    div rsi
    cmp rdx, 0
    jne  .loop
    inc rbx
    jmp .loop
.end:
    mov rax, rbx
    pop rbx
    pop rbp
    ret
;<--    end of divisors -->