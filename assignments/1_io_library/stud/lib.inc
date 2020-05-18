section .text

exit:
	xor rdi, rdi
	mov rax, 60
	syscall

string_length:
	mov rax, 0
.loop:
	cmp byte[rdi+rax], 0
	je  .end

	inc rax
	jmp .loop
.end:
    ret

print_string:
	push rdi
	call string_length
	pop rdi

	mov rdx, rax
	mov rsi, rdi
	mov rdi, 1
	mov rax, 1
	syscall
	
    ret


print_char:
	push rdi
	mov rax, 1
	mov rsi, rsp
	mov rdi, 1
	mov rdx, 1
	syscall
	pop rdi
    ret

print_newline:
	mov rdi, 0xa
	call print_char
    ret


print_uint:
	push r12
	push rbx
	mov rbx, 10
	mov rax, rdi
	mov r12, rsp

	push 0
.loop:
	xor rdx, rdx
	div rbx
	add rdx, 0x30
	mov [rsp-1], dl
	sub rsp, 1
	cmp rax, 0
	je .end
	jmp .loop
.end
	mov rdi, rsp
	call print_string

	mov rsp, r12
	pop rbx
	pop r12
	ret
	

print_int:
	test rdi, rdi
	jns print_uint
	push rdi
	mov rdi, '-'
	call print_char
	pop rdi
	neg rdi
	jmp print_uint

string_equals:
	mov rdx, 0
	mov rax, 1
	push r12
	push r13
.loop:
	mov r12b, [rdi+rdx]
	mov r13b, [rsi+rdx]
	cmp r12b, r13b
	jz .end
	cmp r12b, r13b
	jne .incorrect
	inc rdx
	jmp .loop

.incorrect:
	mov rax, 0
.end:
	pop r13
	pop r12
    ret


read_char:
	sub rsp, 1
	mov rax, 0
	mov rdi, 0
	mov rsi, rsp
	mov rdx, 1
	syscall

	mov rax, [rsi]


	add rsp, 1
    ret 

read_word:
	push r12
	push r13
	push r14
	mov r12, rdi
	mov r13, rsi
	xor r14, r14  ; calculate length
	dec r13
.input_char:
	call read_char
	cmp al, 0x20
	je .end_fn
	cmp al, 0x09
	je .end_fn
	cmp al, 0x0a
	je .end_fn
	cmp al, 0x0d
	je .end_fn
	cmp al, 0
	je .end_fn

	cmp r13, r14
	jl .end
	mov byte[r12+r14], al
	inc r14
	jmp .input_char
.zero_length:
	xor rax, rax
	jmp .end
.end_fn:
	mov byte[r12+r14], 0
	mov rax, r12
	mov rdx, r14
.end:
	pop r14
	pop r13
	pop r12
    ret

; rdi points to a string
; returns rax: number, rdx : length
parse_uint:
    xor rax, rax
    ret

; rdi points to a string
; returns rax: number, rdx : length
parse_int:
    xor rax, rax
    ret 


string_copy:
	call string_length
	sub rdx, 1
	cmp rax, rdx
	je .copy
	
	mov rax, 0
	jmp .end
.copy:
	push r12
	push r13
	mov rax, rsi

.loop:
	mov r12, [rdi]
	mov [rsi], r12
	inc rsi
	inc rdi
	dec rdx
	jz .pop

	jmp .loop
.pop:
	pop r13
	pop r12
	
.end:
    ret
