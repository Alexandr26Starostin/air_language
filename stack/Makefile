CC=g++
FLAGS=-Wshadow -Winit-self -Wredundant-decls -Wcast-align -Wundef -Wfloat-equal -Winline -Wunreachable-code -Wmissing-declarations -Wmissing-include-dirs -Wswitch-enum -Wswitch-default -Weffc++ -Wmain -Wextra -Wall -g -pipe -fexceptions -Wcast-qual -Wconversion -Wctor-dtor-privacy -Wempty-body -Wformat-security -Wformat=2 -Wignored-qualifiers -Wlogical-op -Wmissing-field-initializers -Wnon-virtual-dtor -Woverloaded-virtual -Wpointer-arith -Wsign-promo -Wstack-usage=8192 -Wstrict-aliasing -Wstrict-null-sentinel -Wtype-limits -Wwrite-strings -D_DEBUG -D_EJUDGE_CLIENT_SIDE -D_EJC
DEFINES=-D NOT_DEBUG_ASSERT -D CANARY_STK_NOT -D CANARY_STK_DATA_NOT -D HASH_STK_NOT -D MEMSET_NOT
FILES=main.o hash.o stk_error.o operations_with_stack.o

TARGET = stack
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
	@$(CC) -c $(FLAGS) $(DEFINES) src/main.cpp  -o build/main.o

hash.o: src/hash.cpp
	@$(CC) -c $(FLAGS) $(DEFINES) src/hash.cpp -o build/hash.o

stk_error.o: src/stk_error.cpp
	@$(CC) -c $(FLAGS) $(DEFINES) src/stk_error.cpp -o build/stk_error.o

operations_with_stack.o: src/operations_with_stack.cpp
	@$(CC) -c $(FLAGS) $(DEFINES) src/operations_with_stack.cpp -o build/operations_with_stack.o

clean:
	rm -rf $(TARGET)
	cd build && rm -rf *.o