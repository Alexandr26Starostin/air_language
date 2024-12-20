CC=g++
FLAGS= -Iinclude -I.. -Wshadow -Winit-self -Wredundant-decls -Wcast-align -Wundef -Wfloat-equal -Winline -Wunreachable-code -Wmissing-declarations -Wmissing-include-dirs -Wswitch-enum -Wswitch-default -Weffc++ -Wmain -Wextra -Wall -g -pipe -fexceptions -Wcast-qual -Wconversion -Wctor-dtor-privacy -Wempty-body -Wformat-security -Wformat=2 -Wignored-qualifiers -Wlogical-op -Wmissing-field-initializers -Wnon-virtual-dtor -Woverloaded-virtual -Wpointer-arith -Wsign-promo -Wstack-usage=8192 -Wstrict-aliasing -Wstrict-null-sentinel -Wtype-limits -Wwrite-strings -D_DEBUG -D_EJUDGE_CLIENT_SIDE -D_EJC
FILES=main.o asm_find_file.o launch_asm.o asm_error.o asm_labels.o asm_print.o asm_translate.o

TARGET = assembler
# pathsub
# addprefix
# wildcard

PATCHED_FILES = $(addprefix build/, $(FILES))

all: prepare $(TARGET)

prepare:
	mkdir -p build

$(TARGET): $(FILES)
	@echo "Linling..."
	@$(CC) $(FLAGS) $(PATCHED_FILES) -o $(TARGET)

main.o: src/main.cpp
	@$(CC) -c $(FLAGS) src/main.cpp -o build/main.o

asm_find_file.o: src/asm_find_file.cpp
	@$(CC) -c $(FLAGS) src/asm_find_file.cpp  -o build/asm_find_file.o

launch_asm.o: src/launch_asm.cpp
	@$(CC) -c $(FLAGS) src/launch_asm.cpp  -o build/launch_asm.o

asm_error.o: src/asm_error.cpp
	@$(CC) -c $(FLAGS) src/asm_error.cpp  -o build/asm_error.o

asm_labels.o: src/asm_labels.cpp
	@$(CC) -c $(FLAGS) src/asm_labels.cpp  -o build/asm_labels.o

asm_print.o: src/asm_print.cpp
	@$(CC) -c $(FLAGS) src/asm_print.cpp  -o build/asm_print.o

asm_translate.o: src/asm_translate.cpp
	@$(CC) -c $(FLAGS) src/asm_translate.cpp  -o build/asm_translate.o

clean:
	rm -rf $(TARGET)
	cd build && rm -rf *.o