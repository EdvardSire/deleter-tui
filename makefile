CC := gcc

program: draw.o
	gcc -lncurses draw.o -o program
	./program

draw.o:
	gcc -c draw.c -o draw.o

.PHONY: debug
debug:
	gcc -g draw.c -lncurses -o program
	lldb program

.PHONY: test
test:
	gcc test.c -o program
	./program
