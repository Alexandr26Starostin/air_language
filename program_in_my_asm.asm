global _start     ;начало трансляции в NASM
section .text
_start:

	;--------------------------------------------------------------------------------------------
	;Выделение места под глобальные переменные  (src/write_tree_in_nasm.cpp:98)
	;--------------------------------------------------------------------------------------------
	sub rsp, 6 * 8    ;выделение места под глобальные переменные (их количество == 6)
	mov rbx, rsp        ;rbx = указатель на глобальные переменные
	mov r12, rsp
	;--------------------------------------------------------------------------------------------

;описание 0 оператора  (src/write_tree_in_nasm.cpp:482)

;-----------------------------------------------------------------------
;описание функции    (src/write_tree_in_nasm.cpp:766)
;-----------------------------------------------------------------------

уносится_в_небо_машина:    ;имя функции   (src/write_tree_in_nasm.cpp:775)

	sub r12, 3 * 8  ;(src/write_tree_in_nasm.cpp:790)

	mov rsp, r12  ;(src/write_tree_in_nasm.cpp:793)

	;тело функции   (src/write_tree_in_nasm.cpp:805)

	;описание 1 оператора  (src/write_tree_in_nasm.cpp:482)

		;начало var_declaration  long <variable>...   (src/write_tree_in_nasm.cpp:457)

		;начало assign <variable> = <expression>   (src/write_tree_in_nasm.cpp:415)

		;начало expression   (src/write_tree_in_nasm.cpp:418)

		mov r8, 700000   ;временно сохраняет значение константы = 700000   (src/write_tree_in_nasm.cpp:153)
		push r8   ;кладёт значение константы в стек     (src/write_tree_in_nasm.cpp:158)

		;конец expression  (src/write_tree_in_nasm.cpp:424)

		;начало присваивания переменной   (src/write_tree_in_nasm.cpp:429)

		pop r8   ;вытащил константу из стека    (src/write_tree_in_nasm.cpp:226)
		mov [r12 + 0 * 8], r8   ;присвоил значение локальной переменной    (src/write_tree_in_nasm.cpp:231)

		;конец присваивания переменной   (src/write_tree_in_nasm.cpp:435)

		;конец assign   (src/write_tree_in_nasm.cpp:438)

		;конец var_declaration  long <variable>...   (src/write_tree_in_nasm.cpp:466)

	;описание 2 оператора  (src/write_tree_in_nasm.cpp:482)

		;начало var_declaration  long <variable>...   (src/write_tree_in_nasm.cpp:457)

		;начало assign <variable> = <expression>   (src/write_tree_in_nasm.cpp:415)

		;начало expression   (src/write_tree_in_nasm.cpp:418)

		mov r8, 7   ;временно сохраняет значение константы = 7   (src/write_tree_in_nasm.cpp:153)
		push r8   ;кладёт значение константы в стек     (src/write_tree_in_nasm.cpp:158)

		;конец expression  (src/write_tree_in_nasm.cpp:424)

		;начало присваивания переменной   (src/write_tree_in_nasm.cpp:429)

		pop r8   ;вытащил константу из стека    (src/write_tree_in_nasm.cpp:226)
		mov [r12 + 1 * 8], r8   ;присвоил значение локальной переменной    (src/write_tree_in_nasm.cpp:231)

		;конец присваивания переменной   (src/write_tree_in_nasm.cpp:435)

		;конец assign   (src/write_tree_in_nasm.cpp:438)

		;конец var_declaration  long <variable>...   (src/write_tree_in_nasm.cpp:466)

	;описание 3 оператора  (src/write_tree_in_nasm.cpp:482)

		;начало var_declaration  long <variable>...   (src/write_tree_in_nasm.cpp:457)

		;начало assign <variable> = <expression>   (src/write_tree_in_nasm.cpp:415)

		;начало expression   (src/write_tree_in_nasm.cpp:418)

		mov r8, 0   ;временно сохраняет значение константы = 0   (src/write_tree_in_nasm.cpp:153)
		push r8   ;кладёт значение константы в стек     (src/write_tree_in_nasm.cpp:158)

		;конец expression  (src/write_tree_in_nasm.cpp:424)

		;начало присваивания переменной   (src/write_tree_in_nasm.cpp:429)

		pop r8   ;вытащил константу из стека    (src/write_tree_in_nasm.cpp:226)
		mov [r12 + 2 * 8], r8   ;присвоил значение локальной переменной    (src/write_tree_in_nasm.cpp:231)

		;конец присваивания переменной   (src/write_tree_in_nasm.cpp:435)

		;конец assign   (src/write_tree_in_nasm.cpp:438)

		;конец var_declaration  long <variable>...   (src/write_tree_in_nasm.cpp:466)

	;описание 4 оператора  (src/write_tree_in_nasm.cpp:482)

		;начало while   (src/write_tree_in_nasm.cpp:364)

			repeat_while_0:    ;метка для повторного запуска while   (src/write_tree_in_nasm.cpp:373)

			;условие while    (src/write_tree_in_nasm.cpp:376)

			;начало печати сравнения   (src/write_tree_in_nasm.cpp:701)

			;правая часть сравнения   (src/write_tree_in_nasm.cpp:704)

			mov r8, 0   ;временно сохраняет значение константы = 0   (src/write_tree_in_nasm.cpp:153)
			push r8   ;кладёт значение константы в стек     (src/write_tree_in_nasm.cpp:158)

			;левая часть сравнения   (src/write_tree_in_nasm.cpp:710)

			mov r9, [r12 + 0 * 8]      ;вытащил значение локальной переменной   (src/write_tree_in_nasm.cpp:239)
			push r9    ;сохранил значение локальной переменной в стеке   (src/write_tree_in_nasm.cpp:243)

			pop r13   ;r13 = левая часть сравнения   (src/write_tree_in_nasm.cpp:716)

			pop r14   ;r14 = правая часть сравнения  (src/write_tree_in_nasm.cpp:719)

			cmp r13, r14 ;сравнение   (<левая> - <правая>)  (src/write_tree_in_nasm.cpp:722)

			; <л> - <п> != 0    (== то прыжок мимо - je)
			je skip_while_0   ;если условие не верно, то перепрыгни через тело while   (src/write_tree_in_nasm.cpp:382)

			;тело while    (src/write_tree_in_nasm.cpp:385)

			;описание 5 оператора  (src/write_tree_in_nasm.cpp:482)

				;начало assign <variable> = <expression>   (src/write_tree_in_nasm.cpp:415)

				;начало expression   (src/write_tree_in_nasm.cpp:418)

				;начинает вызов функции   (src/write_tree_in_nasm.cpp:910)

				mov r15, rsp  ;сохраняем rsp   (src/write_tree_in_nasm.cpp:913)

				sub rsp, 8  ;место под rip от call   (src/write_tree_in_nasm.cpp:916)

				push r12    ;сохраняет значение старого стекового фрейма  (src/write_tree_in_nasm.cpp:920)

				;записывает аргументы для функции (src/write_tree_in_nasm.cpp:924)

				; начало записи значений аргументов, передаваемых функции (src/write_tree_in_nasm.cpp:1034)

				; 1 аргумент   (src/write_tree_in_nasm.cpp:1053)

				mov r9, [r12 + 1 * 8]      ;вытащил значение локальной переменной   (src/write_tree_in_nasm.cpp:239)
				push r9    ;сохранил значение локальной переменной в стеке   (src/write_tree_in_nasm.cpp:243)

				; конец записи значений аргументов, передаваемых функции  (src/write_tree_in_nasm.cpp:1062)

				mov r12, rsp    ;(src/write_tree_in_nasm.cpp:932)

				;вызывает функцию   (src/write_tree_in_nasm.cpp:989)

				mov rsp, r15    ;(src/write_tree_in_nasm.cpp:992)

				call чтоб_машину_отправить_в_полёт    ;вызов функции   (src/write_tree_in_nasm.cpp:1006)

				push rax    ;кладёт в стек значение, которое вернула функция    (src/write_tree_in_nasm.cpp:1016)

				;конец вызова функции    (src/write_tree_in_nasm.cpp:1019)

				;конец expression  (src/write_tree_in_nasm.cpp:424)

				;начало присваивания переменной   (src/write_tree_in_nasm.cpp:429)

				pop r8   ;вытащил константу из стека    (src/write_tree_in_nasm.cpp:226)
				mov [r12 + 2 * 8], r8   ;присвоил значение локальной переменной    (src/write_tree_in_nasm.cpp:231)

				;конец присваивания переменной   (src/write_tree_in_nasm.cpp:435)

				;конец assign   (src/write_tree_in_nasm.cpp:438)

			;описание 6 оператора  (src/write_tree_in_nasm.cpp:482)

				;начало assign <variable> = <expression>   (src/write_tree_in_nasm.cpp:415)

				;начало expression   (src/write_tree_in_nasm.cpp:418)

				;начало выполнения арифметической операции с двумя аргументами   (src/write_tree_in_nasm.cpp:509)

				;получение правого аргумента   (src/write_tree_in_nasm.cpp:512)

				mov r8, 1   ;временно сохраняет значение константы = 1   (src/write_tree_in_nasm.cpp:153)
				push r8   ;кладёт значение константы в стек     (src/write_tree_in_nasm.cpp:158)

				;получение левого аргумента   (src/write_tree_in_nasm.cpp:518)

				mov r9, [r12 + 0 * 8]      ;вытащил значение локальной переменной   (src/write_tree_in_nasm.cpp:239)
				push r9    ;сохранил значение локальной переменной в стеке   (src/write_tree_in_nasm.cpp:243)

				pop rax   ;вытаскивает первый аргумент (в него же и сохранит результат)   (src/write_tree_in_nasm.cpp:529)
				pop rdx   ;вытаскивает второй аргумент    (src/write_tree_in_nasm.cpp:532)

				sub rax, rdx     ;rax -= rdx     (src/write_tree_in_nasm.cpp:553)

				push rax    ;кладёт в стек результат операции   (src/write_tree_in_nasm.cpp:581)
				;конец выполнения арифметической операции с двумя аргументами  (src/write_tree_in_nasm.cpp:587)

				;конец expression  (src/write_tree_in_nasm.cpp:424)

				;начало присваивания переменной   (src/write_tree_in_nasm.cpp:429)

				pop r8   ;вытащил константу из стека    (src/write_tree_in_nasm.cpp:226)
				mov [r12 + 0 * 8], r8   ;присвоил значение локальной переменной    (src/write_tree_in_nasm.cpp:231)

				;конец присваивания переменной   (src/write_tree_in_nasm.cpp:435)

				;конец assign   (src/write_tree_in_nasm.cpp:438)

			jmp repeat_while_0    ;прыжок на повторный запуск while   (src/write_tree_in_nasm.cpp:392)

			skip_while_0:    ;метка для пропуска while   (src/write_tree_in_nasm.cpp:396)

		;конец while  (src/write_tree_in_nasm.cpp:401)

	;описание 7 оператора  (src/write_tree_in_nasm.cpp:482)

		;начало печати числа   (src/write_tree_in_nasm.cpp:651)

		mov r9, [r12 + 2 * 8]      ;вытащил значение локальной переменной   (src/write_tree_in_nasm.cpp:239)
		push r9    ;сохранил значение локальной переменной в стеке   (src/write_tree_in_nasm.cpp:243)

		mov rdi, str_for_printf    ;   (src/write_tree_in_nasm.cpp:657)

		pop rsi    ;получает число для печати из стека   (src/write_tree_in_nasm.cpp:660)

		call printf_number    ;печатает число в rdi    (src/write_tree_in_nasm.cpp:663)

		;конец печати числа    (src/write_tree_in_nasm.cpp:666)

	;описание 8 оператора  (src/write_tree_in_nasm.cpp:482)

				;завершение функции  (src/write_tree_in_nasm.cpp:878)

		;вычисляет значение, возвращаемое функцией  (src/write_tree_in_nasm.cpp:881)

		mov r8, 0   ;временно сохраняет значение константы = 0   (src/write_tree_in_nasm.cpp:153)
		push r8   ;кладёт значение константы в стек     (src/write_tree_in_nasm.cpp:158)

		pop rax    ;записывает возвращаемое функцией значение  (src/write_tree_in_nasm.cpp:887)

	;конец функции   (src/write_tree_in_nasm.cpp:811)

	add r12, 8*3   ;  (src/write_tree_in_nasm.cpp:814)

	pop r12    ;   (src/write_tree_in_nasm.cpp:817)

	call end_program  ;завершает работу программу  (src/write_tree_in_nasm.cpp:826)

;описание 9 оператора  (src/write_tree_in_nasm.cpp:482)

;-----------------------------------------------------------------------
;описание функции    (src/write_tree_in_nasm.cpp:766)
;-----------------------------------------------------------------------

чтоб_машину_отправить_в_полёт:    ;имя функции   (src/write_tree_in_nasm.cpp:775)

	;тело функции   (src/write_tree_in_nasm.cpp:805)

	;описание 10 оператора  (src/write_tree_in_nasm.cpp:482)

		;начало if  (src/write_tree_in_nasm.cpp:320)

			;условие if   (src/write_tree_in_nasm.cpp:325)

			;начало печати сравнения   (src/write_tree_in_nasm.cpp:701)

			;правая часть сравнения   (src/write_tree_in_nasm.cpp:704)

			mov r8, 0   ;временно сохраняет значение константы = 0   (src/write_tree_in_nasm.cpp:153)
			push r8   ;кладёт значение константы в стек     (src/write_tree_in_nasm.cpp:158)

			;левая часть сравнения   (src/write_tree_in_nasm.cpp:710)

			mov r9, [r12 + 0 * 8]      ;вытащил значение локальной переменной   (src/write_tree_in_nasm.cpp:239)
			push r9    ;сохранил значение локальной переменной в стеке   (src/write_tree_in_nasm.cpp:243)

			pop r13   ;r13 = левая часть сравнения   (src/write_tree_in_nasm.cpp:716)

			pop r14   ;r14 = правая часть сравнения  (src/write_tree_in_nasm.cpp:719)

			cmp r13, r14 ;сравнение   (<левая> - <правая>)  (src/write_tree_in_nasm.cpp:722)

			; <л> - <п> <  0    (>= то прыжок мимо - jge)
			jge skip_if_0    ;если условие не верно, то перепрыгни через тело if (src/write_tree_in_nasm.cpp:335)

			;тело if  (src/write_tree_in_nasm.cpp:338)

			;описание 11 оператора  (src/write_tree_in_nasm.cpp:482)

				call end_program  ;завершает работу программу  (src/write_tree_in_nasm.cpp:745)

			skip_if_0:    ;метка для пропуска if    (src/write_tree_in_nasm.cpp:345)

		;конец if   (src/write_tree_in_nasm.cpp:350)

	;описание 12 оператора  (src/write_tree_in_nasm.cpp:482)

		;начало if  (src/write_tree_in_nasm.cpp:320)

			;условие if   (src/write_tree_in_nasm.cpp:325)

			;начало печати сравнения   (src/write_tree_in_nasm.cpp:701)

			;правая часть сравнения   (src/write_tree_in_nasm.cpp:704)

			mov r8, 0   ;временно сохраняет значение константы = 0   (src/write_tree_in_nasm.cpp:153)
			push r8   ;кладёт значение константы в стек     (src/write_tree_in_nasm.cpp:158)

			;левая часть сравнения   (src/write_tree_in_nasm.cpp:710)

			mov r9, [r12 + 0 * 8]      ;вытащил значение локальной переменной   (src/write_tree_in_nasm.cpp:239)
			push r9    ;сохранил значение локальной переменной в стеке   (src/write_tree_in_nasm.cpp:243)

			pop r13   ;r13 = левая часть сравнения   (src/write_tree_in_nasm.cpp:716)

			pop r14   ;r14 = правая часть сравнения  (src/write_tree_in_nasm.cpp:719)

			cmp r13, r14 ;сравнение   (<левая> - <правая>)  (src/write_tree_in_nasm.cpp:722)

			; <л> - <п> == 0    (!= то прыжок мимо - jne)
			jne skip_if_1    ;если условие не верно, то перепрыгни через тело if (src/write_tree_in_nasm.cpp:335)

			;тело if  (src/write_tree_in_nasm.cpp:338)

			;описание 13 оператора  (src/write_tree_in_nasm.cpp:482)

								;завершение функции  (src/write_tree_in_nasm.cpp:878)

				;вычисляет значение, возвращаемое функцией  (src/write_tree_in_nasm.cpp:881)

				mov r8, 1   ;временно сохраняет значение константы = 1   (src/write_tree_in_nasm.cpp:153)
				push r8   ;кладёт значение константы в стек     (src/write_tree_in_nasm.cpp:158)

				pop rax    ;записывает возвращаемое функцией значение  (src/write_tree_in_nasm.cpp:887)

				ret    ;переход по метке к месту вызова функции   (src/write_tree_in_nasm.cpp:892)

			skip_if_1:    ;метка для пропуска if    (src/write_tree_in_nasm.cpp:345)

		;конец if   (src/write_tree_in_nasm.cpp:350)

	;описание 14 оператора  (src/write_tree_in_nasm.cpp:482)

				;завершение функции  (src/write_tree_in_nasm.cpp:878)

		;вычисляет значение, возвращаемое функцией  (src/write_tree_in_nasm.cpp:881)

		;начало выполнения арифметической операции с двумя аргументами   (src/write_tree_in_nasm.cpp:509)

		;получение правого аргумента   (src/write_tree_in_nasm.cpp:512)

		mov r9, [r12 + 0 * 8]      ;вытащил значение локальной переменной   (src/write_tree_in_nasm.cpp:239)
		push r9    ;сохранил значение локальной переменной в стеке   (src/write_tree_in_nasm.cpp:243)

		;получение левого аргумента   (src/write_tree_in_nasm.cpp:518)

		;начинает вызов функции   (src/write_tree_in_nasm.cpp:910)

		mov r15, rsp  ;сохраняем rsp   (src/write_tree_in_nasm.cpp:913)

		sub rsp, 8  ;место под rip от call   (src/write_tree_in_nasm.cpp:916)

		push r12    ;сохраняет значение старого стекового фрейма  (src/write_tree_in_nasm.cpp:920)

		;записывает аргументы для функции (src/write_tree_in_nasm.cpp:924)

		; начало записи значений аргументов, передаваемых функции (src/write_tree_in_nasm.cpp:1034)

		; 1 аргумент   (src/write_tree_in_nasm.cpp:1053)

		;начало выполнения арифметической операции с двумя аргументами   (src/write_tree_in_nasm.cpp:509)

		;получение правого аргумента   (src/write_tree_in_nasm.cpp:512)

		mov r8, 1   ;временно сохраняет значение константы = 1   (src/write_tree_in_nasm.cpp:153)
		push r8   ;кладёт значение константы в стек     (src/write_tree_in_nasm.cpp:158)

		;получение левого аргумента   (src/write_tree_in_nasm.cpp:518)

		mov r9, [r12 + 0 * 8]      ;вытащил значение локальной переменной   (src/write_tree_in_nasm.cpp:239)
		push r9    ;сохранил значение локальной переменной в стеке   (src/write_tree_in_nasm.cpp:243)

		pop rax   ;вытаскивает первый аргумент (в него же и сохранит результат)   (src/write_tree_in_nasm.cpp:529)
		pop rdx   ;вытаскивает второй аргумент    (src/write_tree_in_nasm.cpp:532)

		sub rax, rdx     ;rax -= rdx     (src/write_tree_in_nasm.cpp:553)

		push rax    ;кладёт в стек результат операции   (src/write_tree_in_nasm.cpp:581)
		;конец выполнения арифметической операции с двумя аргументами  (src/write_tree_in_nasm.cpp:587)

		; конец записи значений аргументов, передаваемых функции  (src/write_tree_in_nasm.cpp:1062)

		mov r12, rsp    ;(src/write_tree_in_nasm.cpp:932)

		;вызывает функцию   (src/write_tree_in_nasm.cpp:989)

		mov rsp, r15    ;(src/write_tree_in_nasm.cpp:992)

		call чтоб_машину_отправить_в_полёт    ;вызов функции   (src/write_tree_in_nasm.cpp:1006)

		push rax    ;кладёт в стек значение, которое вернула функция    (src/write_tree_in_nasm.cpp:1016)

		;конец вызова функции    (src/write_tree_in_nasm.cpp:1019)

		pop rax   ;вытаскивает первый аргумент (в него же и сохранит результат)   (src/write_tree_in_nasm.cpp:529)
		pop rdx   ;вытаскивает второй аргумент    (src/write_tree_in_nasm.cpp:532)

		imul edx  ;rdx:rax = rax * rdx (берём значения, что rdx:rax == rax)     (src/write_tree_in_nasm.cpp:560)

		push rax    ;кладёт в стек результат операции   (src/write_tree_in_nasm.cpp:581)
		;конец выполнения арифметической операции с двумя аргументами  (src/write_tree_in_nasm.cpp:587)

		pop rax    ;записывает возвращаемое функцией значение  (src/write_tree_in_nasm.cpp:887)

		ret    ;переход по метке к месту вызова функции   (src/write_tree_in_nasm.cpp:892)

	;конец функции   (src/write_tree_in_nasm.cpp:811)

	add r12, 8*1   ;  (src/write_tree_in_nasm.cpp:814)

	pop r12    ;   (src/write_tree_in_nasm.cpp:817)

	;--------------------------------------------------------------------------------------------
	;Завершение работы программы    (src/write_tree_in_nasm.cpp:114)
	;Возвращает значение, которое к этом моменту хранится в rax
	;--------------------------------------------------------------------------------------------
	mov rdi, rax     ;exit64 (rdi)
	mov rax, 0x3C
	syscall
;--------------------------------------------------------------------------------------------

%INCLUDE "std_lib.s"
