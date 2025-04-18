push 4    #начальная позиция стекового фрейма
pop BX

#описание 0 оператора

	#начало var_declaration  double <variable>...

	#начало assign <variable> = <expression>

	#начало expression

	#начало сканирования числа

	in    #считывает число и кладёт его в стек

	#конец сканирования числа

	#конец expression

	#начало присваивания переменной

	pop [0]    #присвоил значение глобальной переменной

	#конец присваивания переменной

	#конец assign

	#конец var_declaration

#описание 1 оператора

	#начало определения функции

	Путник:    #имя функции

		#параметры функции

		#тело функции

		#описание 2 оператора

			#начало печати числа

			#начинает вызов функции

			push BX    #сохраняет значение старого стекового фрейма

			#записывает аргументы для функции

			# начал записи значений аргументов, передаваемых функции

			# 1 аргумент

			push [0]    #вернул значение глобальной переменной

			#конец записи значений аргументов, передаваемых функции

			push BX   #начало старого стекового фрейма

			push 1    #записывает количество локальных переменных в функции

			add    #начало нового стекового фрейма

			pop BX    #сохраняет новое старого стекового фрейма

			#вызывает функцию

			call Герой:    #имя функции

			pop BX    #записывает предыдущее значение начала стекового фрейма

			push AX    #получает значение, вернувшееся от функции

			#конец вызова функции

			out    #печатает число из стека

			#конец печати числа

		#описание 3 оператора

						#завершение функции

			#вычисляет значение, возвращаемое функцией

			push 0    #кладёт в стек число = 0
			pop AX    #записывает возвращаемое функцией значение

		#конец функции

	hlt    #останавливает процессор

#описание 4 оператора

	#начало определения функции

	Герой:    #имя функции

		#параметры функции

		#присваивание значений локальных параметрам функции из стека в оперативную память

		# 1 параметр

		pop [BX+0]    #присвоил значение локальной переменной

		#тело функции

		#описание 5 оператора

			#начало if

				#условие if

				#начало печати сравнения

				#левая часть сравнения

				push [BX+0]    #вернул значение локальной переменной

				#правая часть сравнения

				push 1    #кладёт в стек число = 1
				#операция сравнения

				# <> == <>
				jne skip_if_0:    #если условие не верно, то перепрыгни через тело if

				#тело if

				#описание 6 оператора

										#завершение функции

					#вычисляет значение, возвращаемое функцией

					push 1    #кладёт в стек число = 1
					pop AX    #записывает возвращаемое функцией значение

					ret    #переход по метке к месту вызова функции

				skip_if_0:    #метка для пропуска if

			#конец if

		#описание 7 оператора

						#завершение функции

			#вычисляет значение, возвращаемое функцией

			#начало выполнения арифметической операции с двумя аргументами

			push [BX+0]    #вернул значение локальной переменной

			#начинает вызов функции

			push BX    #сохраняет значение старого стекового фрейма

			#записывает аргументы для функции

			# начал записи значений аргументов, передаваемых функции

			# 1 аргумент

			#начало выполнения арифметической операции с двумя аргументами

			push [BX+0]    #вернул значение локальной переменной

			push 1    #кладёт в стек число = 1
			sub    # exp - exp

			#конец выполнения арифметической операции с двумя аргументами

			#конец записи значений аргументов, передаваемых функции

			push BX   #начало старого стекового фрейма

			push 1    #записывает количество локальных переменных в функции

			add    #начало нового стекового фрейма

			pop BX    #сохраняет новое старого стекового фрейма

			#вызывает функцию

			call Герой:    #имя функции

			pop BX    #записывает предыдущее значение начала стекового фрейма

			push AX    #получает значение, вернувшееся от функции

			#конец вызова функции

			mul    # exp * exp

			#конец выполнения арифметической операции с двумя аргументами

			pop AX    #записывает возвращаемое функцией значение

			ret    #переход по метке к месту вызова функции

		#конец функции

	hlt    #останавливает процессор



hlt