NAME = minipascal
FLEX_OUT := lex.yy
YACC_OUT := y.tab
SRC :=  $(NAME).c $(FLEX_OUT).c $(YACC_OUT).c
CXX ?= gcc
COMPILE_FLAGS = -lfl -std=c++98 -pedantic -Wall -Wextra -g -x=c -DDEBUG;
TESTS := $(patsubst %.in,%.out,$(wildcard tests/*.in))

.PHONY: all clean-all

all: build

build: flex-build
	$(CXX) -o a.out $(SRC) $(COMPILE_FLAGS)

flex-build: yacc-build
	flex $(NAME).fl

yacc-build: $(NAME).ya
	yacc -d $(NAME).ya

test: build clean-test ./a.out $(TESTS)
	echo $(TESTS)
	cat tests.log

./tests/%.out: ./tests/%.in
	./a.out < $< > $@ || true
	(diff $@ $@.expected > $@.diff && echo $*: passed >> tests.log) || echo $*: faild >> tests.log

clean:
	rm -f ./a.out ./$(FLEX_OUT).c ./$(YACC_OUT).c ./$(YACC_OUT).h

clean-test:
	rm -f ./tests/*.out ./tests/*.diff ./tests.log

clean-all: clean clean-test
