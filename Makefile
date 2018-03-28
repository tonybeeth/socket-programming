OBJS:=$(shell ls *.c | sed 's/.c//g')
CC=gcc

default: compile

all: compile

compile: $(OBJS)

%: %.c common.c
	${CC} -o $@ common.c $<

test: compile
	./testme.sh

clean:
	rm -rf *.o *.dat *.txt a.out $(OBJS)
