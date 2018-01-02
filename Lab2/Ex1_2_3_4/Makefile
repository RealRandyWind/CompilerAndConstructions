NAME ?= ex1
PRE_OUT := lex.yy

CXX ?= g++
COMPILE_FLAGS = -lfl -std=c++98 -pedantic -Wall -Wextra -g -D$(call uc,$(NAME))

.PHONY: all clean

all: main-build

main-build: pre-build
	$(CXX) -o a.out intset.c $(PRE_OUT).c $(COMPILE_FLAGS)

pre-build: $(NAME).fl
	flex $(NAME).fl

clean:
	rm -f ./$(PRE_OUT).c ./$(NAME).a ./a.out