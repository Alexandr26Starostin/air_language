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
	xor r13, r13

	mov bl, byte [rsi]
	cmp bl, '-'
	jnz skip_minus

	inc rsi
	mov r13, 1

	skip_minus:

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

	cmp r13, 1
	jnz not_do_neg

	;neg eax

	not eax
	inc eax

	not_do_neg:
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
; printf_number:

; 	;mov rcx, [len_buffer]       ;rcx = index of count free places in buffer
; 	mov rsi, buffer_for_printf   ;rsi = address of buffer

; 	mov rcx, 30

; 	null_buffer:

; 		mov [rsi], byte 0
; 		inc rsi

; 	loop null_buffer

; 	mov rsi, buffer_for_printf   ;rsi = address of buffer
; 	xor ecx, ecx                 ;rcx = index_in_buffer = 0

; 	call print_argument

; 	mov al, 0x0A
; 	mov [rsi], al
; 	inc rsi
; 	inc rcx

; 	;---------------------------------------------------------------------------------------------------------\
; 	;print_buffer:
;     ;interrupt rax = 0x01: print buffer (address = rsi) with len (len = rdx) on flow (flow = rdi) 
;     mov rax, 0x01                ;int  

;     mov rdx, rcx                 ;rdx == len

;     mov rsi, buffer_for_printf   ;rsi == const char* buffer
;     mov rdi, 1                   ;rdi == 1 => stdout
;     syscall ;!!! change r10, r11
; 	;---------------------------------------------------------------------------------------------------------


; 	ret
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
; print_argument:    ;%d   (for int)

; 	mov rax, rdi

; 	;mov edi, eax
	
; 	mov r13, [mask_for_sign]   ;r13 = mask_for_sign
; 	and rdi, r13   ;rdx = eax and r13

; 	jz continue_write_int_10  ;rdx == 0 --> unsigned value
; 							  ;rdx != 0 -->   signed value

; 	neg eax   ;eax *= -1  ;rax = 0...0not(eax)

; 	mov [rsi], byte '-'   ;put '-' in buffer         
; 	inc rsi         ;+1 - next free symbol in buffer
; 	inc rcx         

; 	;----------------------------------------------------------------------------------

; 	continue_write_int_10:

; 	xor r8, r8  ;r8 = 0 (count of numbers in value symbols for number_10)

; 	count_next_symbol_in_number_10:  ;take number from value and put it in stack

; 	cqo          ;rax --> rdx:rax
; 	mov r9, 10   ;r9  = footing of 10 calculus system 
; 	div r9       ;rax = rax // 10
; 				 ;rdx = rax %  10
; 	inc r8       ;+1 count of numbers in stack
; 	add rdx, '0' ;numbers --> 'numbers' (ascii)
; 	push rdx     ;put number in stack

; 	cmp rax, 0
; 	jne count_next_symbol_in_number_10 ;rax != 0 --> can put number from rax in stack

; 	;all numbers from value in stack
; 	;mov rax, r8    ;rax = count of numbers in stack (count of writing symbols will be counted in print_symbols_from_stack)
; 	;mov rdx, r12   ;rdx = address on the next free place in buffer 

; 	;---------------------------------------------------------------

; 	call print_symbols_from_stack        

; 	ret
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
; print_symbols_from_stack:

;     pop r13  ;save address of return for this func
;     ;in stack - all numbers

;     print_symbol:

;         pop rax         ;take symbol from stack to bl
;         mov [rsi], al   ;put symbol in buffer         
;         inc rsi        ;+1 - next free symbol in buffer
;         inc rcx      
;         dec r8          ;-1 number in stack

;         cmp r8, 0     
;         jne print_symbol ;r8 != 0 --> print the next number from stack

;     ;r8 == 0 --> all numbers in stack are printed   

;     push r13   ;save address of return for this func

;     ret
;---------------------------------------------------------------------------------------------------------







;==============================================================================
;	Prints string to stdout
;	Entry:		RSI - address of string
;				... - other arguments
;	Exit:		None
;	Destroy:	None
;==============================================================================
printf_number:											;printf(char* string, ...)
	pop r10											;returning address
	push r9											;6th argument
	push r8											;5th argument
	push rcx										;4th argument
	push rdx										;3th argument
	push rsi										;2th argument
	push rdi										;1th argument
	mov r11, rsp									;start of parameters in stack

	push rbp
	push rbx
	push r12
	push r13										;saved registers
	push r14
	push r15

	mov rsi, [r11]									;first format string
	add r11, 8										;next argument

	mov rdi, Buffer									;copy destination
	copy_to_buffer:									;<------------------------------|
		lodsb										;mov al, ds:[rsi] / inc rsi		|
													;								|
		cmp al, '%'									;if (al == '%') zf = 1			|
		je GetArg									;if (zf == 1) goto GetArg		|
													;								|
		cmp al, 0									;if (al == 0) zf = 1			|
 		je end_printf								;if (zf == 1) goto end_printf---|---|
													;								|	|
		call check_buffer							;flush buffer before put symbol?|	|
	jmp copy_to_buffer								;-------------------------------|	|
													;									|
	end_printf:										;<----------------------------------|
	call flush_buffer								;flush the buffer

	pop r15
	pop r14
	pop r13
	pop r12											;return saved registers
	pop rbx
	pop rbp

	add rsp, 48										;return rsp above the 6th arguments

	push r10										;put return address
	ret
;==============================================================================

;==============================================================================
;	Check buffer on overflow
;	Entry:		RDI - current ip in buffer
;	Exit:		None
;	Destroy:	RDI
;==============================================================================
check_buffer:
	push rbx										;save rbx
	xor rbx, rbx									;rbx = 0
	add rbx, rdi									;rbx += rdi
	sub rbx, Buffer									;rbx -= Buffer

	cmp rbx, buffer_size							;if (rbx == buffer_size) zf = 1
	jb skip_flush									;if (zf < 0) goto skip_flush ---|
		call flush_buffer							;call buffer flush				|
		mov rdi, Buffer								;set rdi on buffer start		|
													;								|
	skip_flush:										;<------------------------------|
	stosb											;mov [rdi], al / inc rdi
	pop rbx											;return rbx
	ret
;==============================================================================

;==============================================================================
;	Flush buffer
;	Entry:		None
;	Exit:		None
;	Destroy:	RSI, RDX, RAX, RDI
;==============================================================================
flush_buffer:
	push rsi										;save rsi
	push rdx										;save rdx
	push rax										;save rax
	push rcx										;save rcx
	push r11										;save r11 which is changed by syscall
	mov rsi, Buffer									;rsi = &Buffer
	mov rdx, buffer_size							;rdx = buffer len
	mov rax, 0x01									;write
	mov rdi, Stdout									;output descriptor
	syscall				 							;system instruction

	mov rcx, buffer_size							;rcx = size of buffer
	mov rdi, Buffer									;rdi = buffer start
	xor rax, rax									;rax = 0
	rep stosb										;while (rcx--) stosb
	pop r11											;return r11
	pop rcx											;return rcx
	pop rax											;return rax
	pop rdx											;return rdx
	pop rsi											;return rsi
	ret
;==============================================================================
;	Calculate string length
;	Entry:		RSI - address of string
;	Exit:		RCX - string length
;	Destroy:	RCX, AL
;==============================================================================
strlen:												;strlen(rsi)
	push rsi										;save rsi
	xor rcx, rcx									;rcx = 0
	count:											;<----------------------------------|
		lodsb										;mov al, ds:[rsi] / inc rsi			|
		cmp al, 0									;if (al == 0) zf = 1				|
		je end_count								;if (zf == 1) goto end_count----|	|
		add rcx, 1									;rcx += 2						|	|
	jmp count										;-------------------------------|---|
	end_count:										;<------------------------------|
	pop rsi											;back rsi | string start
ret
;==============================================================================

;==============================================================================
;	Get argument for printf
;	Entry:		RSI - current position in printf string
;	Exit:		None
;	Destroy:	RCX, AL
;==============================================================================
GetArg:
	lodsb											;mov al, ds:[rsi] / inc rsi

	cmp al, '%'										;if (al == '%') zf = 1
	jne skip_percent								;if (zf != 1) goto skip_percent	----|
	call check_buffer								;call buffer checking				|
	jmp _default_									;goto _default_						|
													;									|
	skip_percent:									;<----------------------------------|
	sub al, 'b'										;al -= 'b'
	cmp al, 'x' - 'b'								;if (al == 'x' - 'b') zf = 1
	ja _default_									;if (>) goto _default_
	and rax, 0xFF									;rax &= 0xFF
	shl rax, 3										;rax *= 8
	add rax, jump_table								;rax += &jump_table

	push rsi										;save rsi
	mov rsi, [r11]									;rsi = new arg
	add r11, 8

	jmp [rax]										;goto jump_table + al * 8
;==============================================================================

;==============================================================================
;	Handler of parameters
;	Entry:		None
;	Exit:		None
;	Destroy:	None
;==============================================================================
_s_:
	call strlen										;calc len of string
	str_to_buffer:									;put str symbol to Buffer <---------|
		lodsb										;mov al, [rsi] / inc rsi			|
		call check_buffer							;check buffer overflow / put symbol	|
	loop str_to_buffer								;-----------------------------------|
	pop rsi											;return rsi to format string
	jmp _default_									;goto _default_

_c_:
	mov rax, rsi									;rax = rsi | put symbol to rax
	call check_buffer								;check buffer overflow / put symbol
	pop rsi											;return rsi to format string
	jmp _default_									;goto _default_

_b_:
	mov rcx, 1										;amount of bytes per one symbol
	mov rbx, 0x01									;mask for binary
	jmp _numbers_2_systems_							;goto _numbers_2_systems_

_o_:
	mov rcx, 3										;amount of bytes per one symbol
	mov rbx, 0x07									;mask for oct
	jmp _numbers_2_systems_							;goto _numbers_2_systems_

_x_:
	mov rcx, 4										;amount of bytes per one symbol
	mov rbx, 0x0F									;mask for hex
	jmp _numbers_2_systems_							;goto _numbers_2_systems_

_default_:
	jmp copy_to_buffer								;goto copy_to_buffer
;==============================================================================

;==============================================================================
;	Handler %d parameter
;	Entry:		RSI - number to transform
;	Exit:		None
;	Destroy:	None
;==============================================================================
_d_:
	mov rax, rsi									;rax = rsi | put number to rax

	push r13										;save r13
	mov r13, [sign_mask]							;r13 = 1 << 31
	and rax, r13									;rax &= 1 << 31
	jz unsigned										;if (zf != 0) goto unsigned	--------|
	mov rax, '-'									;rax = '-'							|
	call check_buffer								;check buffer overflow | put symbol	|
	mov rax, rsi									;prepare number to negative			|
	neg eax											;rax *= -1							|
	mov rsi, rax									;rsi = rax							|
													;									|
	unsigned:										;<----------------------------------|
		pop r13										;return r13
		push rdi									;save current Buffer ip
		mov rdi, trans_buffer						;rdi = &trans_buffer
		mov rcx, 10									;rcx = 10 | ss base
		mov rax, rsi								;rax = rsi

	division:										;<----------------------------------------------|
		cqo											;dd rax -> dq rdx:rax by sign bit duplicating	|
		div rcx										;rdx:rax /= rcx // rax - result // rdx - part	|
		add rdx, '0'								;rdx += 30										|
		mov [rdi], rdx								;trans_buffer[i] = 'c'							|
		inc rdi										;rdi++											|
		cmp rax, 0									;if (rax == 0) zf = 0							|
	ja division										;if (zf > 0) goto division ---------------------|

	jmp _to_printf_buffer_							;goto _to_printf_buffer_
;==============================================================================

;==============================================================================
;	Handler of binary system parameter
;	Entry:		RCX - binary base
;				RBX - mask
;				RSI - number
;	Exit:		None
;	Destroy:	None
;==============================================================================
_numbers_2_systems_:
	push rdi										;save current Buffer ip
	mov rdi, trans_buffer							;rdi = &trans_buffer_

	transform:										;<----------------------------------|
		mov rax, rsi								;rax = rsi							|
		and rax, rbx								;use mask							|
		mov rdx, symbols_array						;rdx = &symbols_array				|
		add rdx, rax								;rdx += rax | get symbol ASCII code	|
		mov al, [rdx]								;al = [rdx] | symbol				|
		stosb										;mov [rdi], al / inc rdi			|
		shr rsi, cl									;rsi >> cl							|
		cmp rsi, 0									;if (rsi == 0) zf = 0				|
	ja transform									;if (zf > 0) goto transform --------|

	jmp _to_printf_buffer_							;goto _to_printf_buffer_
;==============================================================================

;==============================================================================
;	From transform buffer to printf buffer
;	Entry:		RDI - pointer to end of transform buffer
;	Exit:		None
;	Destroy:	None
;==============================================================================
_to_printf_buffer_:
	mov rsi, rdi									;rsi = rdi
	pop rdi											;return current Buffer ip

	mov rcx, rsi									;rcx = rsi
	sub rcx, trans_buffer							;rcx = rsi - trans_buffer
	inc rcx											;rcx++

	put_to_buffer:									;<--------------------------------------|
		mov al, [rsi]								;al = [rsi] | symbol from trans_buffer	|
		call check_buffer							;check Buffer overflow | put symbol		|
		mov [rsi], dword 0							;[rsi] = 0								|
		dec rsi										;rsi--									|
	loop put_to_buffer								;while(rcx--) goto put_to_buffer -------|

	pop rsi											;return current format string ip
	jmp _default_									;goto _default_
;==============================================================================






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


;------------------------------------------------------------------------------------------------------------------
;													sqrt_number
;
;
;entry: rdi = number for sqrt
;
;exit: NO
;
;destr:
;	
;------------------------------------------------------------------------------------------------------------------
sqrt_number:

	cvtsi2sd xmm0, rdi
    sqrtsd xmm0, xmm0
    cvtsd2si rax, xmm0

	ret
;------------------------------------------------------------------------------------------------------------------


section .data   ;has data
buffer_for_printf: times 30 db 0  ;buffer for symbols

str_for_printf db "%d", 0x0A, 0x00

bytes_of_number: times 30 db 0
mask_for_sign dd 1<<31   ;mask_for_sign in int (for %d)

Stdout 			equ 0x01							;descriptor of stdout
buffer_size 	equ 128								;size of buffer
trans_buff_size	equ 64								;size of translator buffer
sign_mask		dd 1<<31

section .rodata
jump_table 	dq 	_b_
			dq	_c_
			dq 	_d_
			times 'o' - 'd' - 1 dq _default_
			dq 	_o_
			times 's' - 'o' - 1 dq _default_
			dq	_s_
			times 'x' - 's' - 1 dq _default_
			dq	_x_
symbols_array db '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'

section .bss										;start non-prog segment
Buffer 			resb buffer_size					;Init buffer
trans_buffer 	resb trans_buff_size				;Init trans buffer













