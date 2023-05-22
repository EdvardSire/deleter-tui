.PHONY: run
run:
	gcc draw.c -lncurses && ./a.out

.PHONY: debug
debug:
	gcc -g draw.c -lncurses && lldb a.out

.PHONY: test
test:
	gcc test.c && ./a.out
