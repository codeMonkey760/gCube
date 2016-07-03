SRCS = $(wildcard src/Source/*.c)
OBJS = $(patsubst src/Source/%.c, bin/%.o,$(SRCS))
