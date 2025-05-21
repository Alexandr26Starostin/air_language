;------------------------------------------------------------------------------------------------------------------
;													scanf_number
;take number from stdio and push it in stack
;
;entry: NO
;
;exit: NO
;
;destr: 
;	
;------------------------------------------------------------------------------------------------------------------
scanf_number:

	push rbx
	push r12

	xor eax, eax      		   ; номер syscall (0 = read)
    xor edi, 0      		   ; файловый дескриптор (0 = stdin)
	mov rsi, bytes_of_number   ; буфер для символа
    mov rdx, 20     			   ; количество байт
    syscall         		   ; вызов syscall

	mov rcx, rax

	;mov rax, 1          ; sys_write
    ;mov rdi, 1          ; stdout
    ;mov rsi, bytes_of_number     ; буфер с данными
    ;mov rdx, rcx         ; кол-во прочитанных байт
    ;syscall

	xor eax, eax
	xor ebx, ebx

	prepare_next_byte:

	mov bl, byte [rsi]
	inc rsi

	cmp bl, 0x0A 
	jz end_scanf

	sub bl, '0'

	mov r8, 10
	mul r8
	add rax, rbx

	loop prepare_next_byte

	end_scanf:

	pop r12
	pop rbx

	ret

;------------------------------------------------------------------------------------------------------------------





;------------------------------------------------------------------------------------------------------------------
;													printf_number
;print number in stdio
;
;entry: rdi = number for print
;
;exit: NO
;
;destr:
;	
;------------------------------------------------------------------------------------------------------------------
printf_number:

	;mov rcx, [len_buffer]       ;rcx = index of count free places in buffer
	mov rsi, buffer_for_printf   ;rsi = address of buffer
	xor ecx, ecx                 ;rcx = index_in_buffer = 0

	call print_argument

	;---------------------------------------------------------------------------------------------------------\
	;print_buffer:
    ;interrupt rax = 0x01: print buffer (address = rsi) with len (len = rdx) on flow (flow = rdi) 
    mov rax, 0x01                ;int  

    mov rdx, rcx                 ;rdx == len

    mov rsi, buffer_for_printf   ;rsi == const char* buffer
    mov rdi, 1                   ;rdi == 1 => stdout
    syscall ;!!! change r10, r11
	;---------------------------------------------------------------------------------------------------------

	ret
;------------------------------------------------------------------------------------------------------------------


;------------------------------------------------------------------------------------------------------------------
;													print_argument
;print argument %d (int)
;
;entry: rdi = number for print
;       rcx = index_in_buffer
;       rsi = address of buffer
;
;exit: NO
;
;destr:
;	
;------------------------------------------------------------------------------------------------------------------
print_argument:    ;%d   (for int)

	mov rax, rdi
	
	mov r13, [mask_for_sign]   ;r13 = mask_for_sign
	and rdi, r13   ;rdx = eax and r13

	jz continue_write_int_10  ;rdx == 0 --> unsigned value
							  ;rdx != 0 -->   signed value

	neg eax   ;eax *= -1  ;rax = 0...0not(eax)

	mov [rsi], byte '-'   ;put '-' in buffer         
	inc rdx         ;+1 - next free symbol in buffer
	inc rcx         

	;----------------------------------------------------------------------------------

	continue_write_int_10:

	xor r8, r8  ;r8 = 0 (count of numbers in value symbols for number_10)

	count_next_symbol_in_number_10:  ;take number from value and put it in stack

	cqo          ;rax --> rdx:rax
	mov r9, 10   ;r9  = footing of 10 calculus system 
	div r9       ;rax = rax // 10
				 ;rdx = rax %  10
	inc r8       ;+1 count of numbers in stack
	add rdx, '0' ;numbers --> 'numbers' (ascii)
	push rdx     ;put number in stack

	cmp rax, 0
	jne count_next_symbol_in_number_10 ;rax != 0 --> can put number from rax in stack

	;all numbers from value in stack
	;mov rax, r8    ;rax = count of numbers in stack (count of writing symbols will be counted in print_symbols_from_stack)
	;mov rdx, r12   ;rdx = address on the next free place in buffer 

	;---------------------------------------------------------------

	call print_symbols_from_stack        

	ret
;------------------------------------------------------------------------------------------------------------------

;---------------------------------------------------------------------------------------------------------
;                                       print_symbols_from_stack
;value --> (calculus system) --> array of numbers in stack (little end)
;print this numbers from stack
;
;entry: rsi = address of buffer
;		rcx = index_in_buffer
;		r8  = count of numbers in value symbols for number_10
;exit:  
;
;destr: 
;---------------------------------------------------------------------------------------------------------
print_symbols_from_stack:

    pop r13  ;save address of return for this func
    ;in stack - all numbers

    print_symbol:

        pop rax         ;take symbol from stack to bl
        mov [rsi], al   ;put symbol in buffer         
        inc rsi        ;+1 - next free symbol in buffer
        inc rcx      
        dec r8          ;-1 number in stack

        cmp r8, 0     
        jne print_symbol ;r8 != 0 --> print the next number from stack

    ;r8 == 0 --> all numbers in stack are printed   

    push r13   ;save address of return for this func

    ret
;---------------------------------------------------------------------------------------------------------






;------------------------------------------------------------------------------------------------------------------
;													end_program
;abort program
;
;entry: NO
;
;exit: NO
;
;destr: rdi = code for ending program
;		rax = number of interrupt
;	
;------------------------------------------------------------------------------------------------------------------
end_program:

	xor edi, edi     ;exit64 (0)
	mov rax, 0x3C
	syscall

;------------------------------------------------------------------------------------------------------------------


section .data   ;has data
buffer_for_printf: times 30 db 0  ;buffer for symbols
bytes_of_number: times 30 db 0
mask_for_sign dd 1<<31   ;mask_for_sign in int (for %d)
