; p19.asm -- Project Euler #19
;
; How many Sundays fell on the first of the month during the twentieth
; century (1 Jan 1901 to 31 Dec 2000)?
;
; Approach: walk the day-of-week (0=Sunday..6=Saturday) forward one
; month at a time starting from 1 Jan 1900 (a Monday, so dow=1), and
; count every 1st-of-month that lands on dow=0 once the walk reaches
; 1901 or later. Walking from 1900 (instead of starting the count at
; 1901 directly) is just the easiest way to bootstrap dow's initial
; value from the one known fixed point given in the problem.

%include "asmrt.inc"

section .data
    daysInMonth db 31,28,31,30,31,30,31,31,30,31,30,31

section .text
    global entry

; isLeapYear(year) -> 1/0
proc isLeapYear
    args year

    mov rax, [%$year]
    xor rdx, rdx
    mov rcx, 4
    div rcx
    cmp rdx, 0
    jne .notLeap

    mov rax, [%$year]
    xor rdx, rdx
    mov rcx, 100
    div rcx
    cmp rdx, 0
    jne .isLeap

    mov rax, [%$year]
    xor rdx, rdx
    mov rcx, 400
    div rcx
    cmp rdx, 0
    jne .notLeap

.isLeap:
    mov rax, 1
    jmp .done
.notLeap:
    mov rax, 0
.done:

    end
    ret 8

proc entry
    local year
    local month
    local dow
    local count
    local days
    endlocal

    mov qword [%$dow], 1      ; 1 Jan 1900 was a Monday
    mov qword [%$count], 0
    mov qword [%$year], 1900
.yearLoop:
    cmp qword [%$year], 2000
    jg .yearLoopEnd

    mov qword [%$month], 0
.monthLoop:
    cmp qword [%$month], 12
    jge .monthLoopEnd

    cmp qword [%$year], 1901
    jl .skipCheck
    cmp qword [%$dow], 0
    jne .skipCheck
    inc qword [%$count]
.skipCheck:

    mov rax, [%$month]
    movzx rax, byte [daysInMonth + rax]
    mov [%$days], rax

    cmp qword [%$month], 1
    jne .notFeb
    push qword [%$year]
    call isLeapYear
    cmp rax, 0
    je .notFeb
    mov qword [%$days], 29
.notFeb:

    mov rax, [%$dow]
    add rax, [%$days]
    xor rdx, rdx
    mov rcx, 7
    div rcx
    mov [%$dow], rdx

    inc qword [%$month]
    jmp .monthLoop
.monthLoopEnd:

    inc qword [%$year]
    jmp .yearLoop
.yearLoopEnd:

    push 1
    push qword [%$count]
    call ioWriteNum

    push 1
    push 10
    call ioWriteChar

    mov rax, 0
    end
    ret 24
