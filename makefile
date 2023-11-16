CC := gcc
CC_FLAGS := -Wall
CC_DEBUG_FLAGS := -g

SRCS := $(wildcard *.c)
OBJS := $(SRCS:c=o)
LIBS := ncurses
TARGET := program

all: $(TARGET)
	./program

$(TARGET): $(OBJS)
	$(CC) $(CC_FLAGS) $(CC_DEBUG_FLAGS) -l$(LIBS) -o $@ $(OBJS)

# draw.o:
# 	$(CC) $(CC_FLAGS) -c draw.c -o draw.o
%.o: %.c
	$(CC) $(CC_FLAGS) $(CC_DEBUG_FLAGS) -c $< -o $@

.PHONY: debug
debug:
	$(CC) $(CC_FLAGS) $(CC_DEBUG_FLAGS) -l$(LIBS) -o program $(OBJS)
	lldb program

.PHONY: clean
clean:
	rm -f *.o $(TARGET)
	rm -rf *.dSYM
