NAME ?= ex5
PRE_OUT := lex.yy
TEST_IN := 
CXX ?= g++
COMPILE_FLAGS = -lfl -std=c++98 -pedantic -Wall -Wextra -g -lm

.PHONY: all clean

all: main-build

main-build: pre-build
	$(CXX) -o a.out $(PRE_OUT).c $(COMPILE_FLAGS)

pre-build: $(NAME).fl
	flex $(NAME).fl

clean:
	rm -f ./$(PRE_OUT).c ./$(NAME).a ./a.out