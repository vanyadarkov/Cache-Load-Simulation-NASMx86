.PHONY: all clean run build

all: checker

build: checker

run: checker
	./checker

cache.o: cache.asm
	nasm -f elf $^ -o $@

checker.o: checker.c
	gcc -c -g -m32 $^ -o $@

checker: checker.o cache.o
	gcc -m32 -g $^ -o $@
	rm *.o

clean:
	rm -f checker
	rm -f output/*
