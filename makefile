CC := gcc
CC_FLAGS := -Wall

program: draw.o
	$(CC) $(CC_FLAGS) -lncurses draw.o -o program
	./program

draw.o:
	$(CC) $(CC_FLAGS) -c draw.c -o draw.o

.PHONY: debug
debug:
	$(CC) $(CC_FLAGS) -g draw.c -lncurses -o program
	lldb program

