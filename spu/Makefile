CC=g++
FLAGS= -Iinclude -I.. -I../stack/include -Wshadow -Winit-self -Wredundant-decls -Wcast-align -Wundef -Wfloat-equal -Winline -Wunreachable-code -Wmissing-declarations -Wmissing-include-dirs -Wswitch-enum -Wswitch-default -Weffc++ -Wmain -Wextra -Wall -g -pipe -fexceptions -Wcast-qual -Wconversion -Wctor-dtor-privacy -Wempty-body -Wformat-security -Wformat=2 -Wignored-qualifiers -Wlogical-op -Wmissing-field-initializers -Wnon-virtual-dtor -Woverloaded-virtual -Wpointer-arith -Wsign-promo -Wstack-usage=8192 -Wstrict-aliasing -Wstrict-null-sentinel -Wtype-limits -Wwrite-strings -D_DEBUG -D_EJUDGE_CLIENT_SIDE -D_EJC
FILES=main.o launch_spu.o spu_find_file.o spu_error.o spu_print.o spu_run.o spu_draw_picture.o     hash.o stk_error.o operations_with_stack.o

TARGET = spu
# pathsub
# addprefix
# wildcard

PATCHED_FILES = $(addprefix build/, $(FILES))

all: prepare $(TARGET)

prepare:
	mkdir -p build

$(TARGET): $(FILES)
	@echo "Linling..."
	@$(CC) $(FLAGS) $(PATCHED_FILES)  -o $(TARGET)

main.o: src/main.cpp
	@$(CC) -c $(FLAGS) src/main.cpp  -o build/main.o

launch_spu.o: src/launch_spu.cpp
	@$(CC) -c $(FLAGS) src/launch_spu.cpp -o build/launch_spu.o

spu_find_file.o: src/spu_find_file.cpp
	@$(CC) -c $(FLAGS) src/spu_find_file.cpp  -o build/spu_find_file.o

spu_error.o: src/spu_error.cpp
	@$(CC) -c $(FLAGS) src/spu_error.cpp  -o build/spu_error.o

spu_print.o: src/spu_print.cpp
	@$(CC) -c $(FLAGS) src/spu_print.cpp  -o build/spu_print.o

spu_run.o: src/spu_run.cpp
	@$(CC) -c $(FLAGS) src/spu_run.cpp  -o build/spu_run.o

spu_draw_picture.o: src/spu_draw_picture.cpp
	@$(CC) -c $(FLAGS) src/spu_draw_picture.cpp  -o build/spu_draw_picture.o	





hash.o: ../stack/src/hash.cpp
	@$(CC) -c $(FLAGS) $(DEFINES) ../stack/src/hash.cpp -o build/hash.o

stk_error.o: ../stack/src/stk_error.cpp
	@$(CC) -c $(FLAGS) $(DEFINES) ../stack/src/stk_error.cpp -o build/stk_error.o

operations_with_stack.o: ../stack/src/operations_with_stack.cpp
	@$(CC) -c $(FLAGS) $(DEFINES) ../stack/src/operations_with_stack.cpp -o build/operations_with_stack.o

clean:
	rm -rf $(TARGET)
	cd build && rm -rf *.o