; https://www.codewars.com/kata/53da3dbb4a5168369a0000fe

global even_or_odd

section .data
  even db "Even", 0
  odd  db "Odd", 0

section .text
; input: edi = number
; output: rax (a pointer to a statically allocated C-string, will not be freed by tests)
; callee saved registers: rbx, rsp, rbp, r12-r15
even_or_odd:
  mov rax, odd
  mov rcx, even
  and edi, 1
  cmovz rax, rcx
  ret
