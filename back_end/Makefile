CC=g++
FLAGS=-Iinclude -I../include -Wshadow -Winit-self -Wredundant-decls -Wcast-align -Wundef -Wfloat-equal -Winline -Wunreachable-code -Wmissing-declarations -Wmissing-include-dirs -Wswitch-enum -Wswitch-default -Weffc++ -Wmain -Wextra -Wall -g -pipe -fexceptions -Wcast-qual -Wconversion -Wctor-dtor-privacy -Wempty-body -Wformat-security -Wformat=2 -Wignored-qualifiers -Wlogical-op -Wmissing-field-initializers -Wnon-virtual-dtor -Woverloaded-virtual -Wpointer-arith -Wsign-promo -Wstack-usage=8192 -Wstrict-aliasing -Wstrict-null-sentinel -Wtype-limits -Wwrite-strings -D_DEBUG -D_EJUDGE_CLIENT_SIDE -D_EJC
FILES=main.o list_of_func.o operations_with_files.o name_table.o tokens.o tree.o local_name_table.o launch_back_end.o
TARGET=back_end

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

launch_back_end.o: src/launch_back_end.cpp
	@$(CC) -c $(FLAGS) src/launch_back_end.cpp -o build/launch_back_end.o

list_of_func.o: ../src/list_of_func.cpp
	@$(CC) -c $(FLAGS) ../src/list_of_func.cpp -o build/list_of_func.o

operations_with_files.o: ../src/operations_with_files.cpp
	@$(CC) -c $(FLAGS) ../src/operations_with_files.cpp -o build/operations_with_files.o

name_table.o: ../src/name_table.cpp
	@$(CC) -c $(FLAGS) ../src/name_table.cpp -o build/name_table.o

tokens.o: ../src/tokens.cpp
	@$(CC) -c $(FLAGS) ../src/tokens.cpp -o build/tokens.o

tree.o: ../src/tree.cpp
	@$(CC) -c $(FLAGS) ../src/tree.cpp -o build/tree.o

local_name_table.o: ../src/local_name_table.cpp
	@$(CC) -c $(FLAGS) ../src/local_name_table.cpp -o build/local_name_table.o

clean:
	rm -rf $(TARGET)
	cd build && rm -rf *.o