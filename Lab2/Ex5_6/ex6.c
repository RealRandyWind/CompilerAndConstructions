#include "ex6.h"
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

#define ERROR_MSG "SYNTAX ERROR"
#define DEFAULT_DELTABUFFER 32
#define PRIORITY_RANGE 5

static int _scope = 0;
static int _token = TEOF;
static size_t _leng = 0;
static size_t _size = 0;
static char *_text = '\0';

/* private, in this case LL1 it stores the token data in a global private */
void _peek()
{
	_token = lex(&_leng);
	if(_size <= _leng)
	{
		/* increse _text buffer size to fit text() */
		_size = ((_leng / DEFAULT_DELTABUFFER) + 1) * DEFAULT_DELTABUFFER;
		_text = (char*)realloc(_text, _size * sizeof(char));
		if(!_text) { exit(EXIT_FAILURE); }
	}
	text(_text);
}

void throwParseError()
{
	printf(ERROR_MSG);
	exit(EXIT_FAILURE);
}

int match(EToken token)
{
	if(_token != token) { return false; }
	_peek();
	return true;
}

/* <number> */
int parseNumber(FNode **ptrnode)
{
	return (match(TNUMBER) && newNumberNode(atof(_text), INT_MIN, ptrnode));
}

/* <operator> */
int parseOperator(FNode **ptrnode)
{
	int priority;

	priority = PRIORITY_RANGE * _scope;
	return (match(TPOW) && newOperatorNode(*_text, priority + 5, 1, ptrnode))
		|| (match(TMUL) && newOperatorNode(*_text, priority + 4, -1, ptrnode))
		|| (match(TDIV) && newOperatorNode(*_text, priority + 3, -1, ptrnode))
		|| (match(TADD) && newOperatorNode(*_text, priority + 2, -1, ptrnode))
		|| (match(TSUB) && newOperatorNode(*_text, priority + 1, -1, ptrnode));
}

/* <expression> ::= '(' <expression> ')' | <number> [ <operator> <expression> ] */
void parseExpression(FNode **ptrnode)
{
	/* checks ( E ) */
	if(match(TLPAR))
	{
		++_scope;
		/* only succeeds if valid */
		parseExpression(ptrnode);
		/* ensure closing par follows */
		if(!match(TRPAR)) { throwParseError(); }
		--_scope;
		return;
	}
	/* checks N O E */
	if(!parseNumber(ptrnode)) { throwParseError(); }
	if(parseOperator(ptrnode)) { parseExpression(ptrnode); }
}

void parse(FNode **ptrnode)
{
	FNode *node;

	/* entry of parser */
	node = nullptr; _peek();
	parseExpression(&node);
	if(!match(TEOF)) { throwParseError(); }

	if(ptrnode) { (*ptrnode) = node; }
	else { freeNode(node); }

	if(_size) { free(_text); _size = 0; _text ='\0'; _token = TEOF; _scope = 0; }
}