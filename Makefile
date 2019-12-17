

C_SRCS = $(wildcard src/*.c)
CPP_SRCS = $(wildcard src/*.cpp)

C_OBJS = $(C_SRCS:src/%.c=obj/%.o)
CPP_OBJS = $(CPP_SRCS:src/%.cpp=obj/%.opp)

CFLAGS = -Iinclude

obj/%.o: src/%.c
	$(CC) -c $< -o $@ $(CFLAGS)

obj/%.opp: src/%.cpp
	$(CXX) -c $< -o $@ $(CFLAGS)

mariokart: $(C_OBJS) $(CPP_OBJS)
	echo $@

all: mariokart

clean:
	rm -f obj/*

.PHONY: all
.DEFAULT_GOAL := all
