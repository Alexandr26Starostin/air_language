#! /bin/bash

cd ./front_end
./front_end -read_program ../solve_square.air -write_tree ../ast.txt -write_table ../table.txt

cd ../back_end
./back_end -read_tree ../ast.txt -read_table ../table.txt -write_asm ../program_in_asm.asm

cd ../assembler
./assembler "../program_in_asm.asm" "../cmd.txt"

cd ../spu
./spu "../cmd.txt"

cd ../
