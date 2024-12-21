#! /bin/bash

#solve_square.air
#factorial_with_while.air
#factorial_with_recursive.air
#reverse_program.air

cd ./front_end
./front_end -read_program ../reverse_program.air -write_tree ../ast.txt -write_table ../table.txt

cd ../middle_end
./middle_end -read_tree ../ast.txt -read_table ../table.txt -write_tree ../ast.txt

cd ../back_end
./back_end -read_tree ../ast.txt -read_table ../table.txt -write_asm ../program_in_asm.asm

cd ../assembler
./assembler "../program_in_asm.asm" "../cmd.txt"

cd ../spu
./spu "../cmd.txt"

cd ../
