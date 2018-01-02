CXX ?= g++
COMPILE_FLAGS = -lfl -std=c++98 -pedantic -Wall -Wextra -g -lm
EX6_SRC = node.c ex6.c
EX7_SRC = node.c ex7.c

ex6: ex6-clean ex6-flex
	$(CXX) -o a.out lex.yy.c $(EX6_SRC) $(COMPILE_FLAGS)

ex6-flex: ex6.fl 
	flex $<

ex7: ex7-clean ex7-flex
	$(CXX) -o a.out lex.yy.c $(EX7_SRC) $(COMPILE_FLAGS)

ex7-flex: ex7-llnext
	flex ex7.fl

ex7-llnext: ex7.g
	LLnextgen $<

clean: ex6-clean ex7-clean

ex6-clean:
	rm -f ./lex.yy.c ./ex6.a ./*.o

ex7-clean:
	rm -f ./lex.yy.c ./ex7.a ./ex7.c ./ex7.h ./*.o