#ifndef EX6_H
#define EX6_H

#include "types.h"
#include "node.h"

extern int lex(size_t *ptrleng);
extern const char* text(char *into);

typedef enum EToken
{
	TEOF = 0,
	TADD,
	TSUB,
	TMUL,
	TDIV,
	TPOW,
	TNUMBER,
	TLPAR,
	TRPAR,
	EToken_EnumSize
} EToken;

void throwParseError();
int match(EToken token);
int parseNumber(FNode **ptrnode);
int parseOperator(FNode **ptrnode);
void parseExpression(FNode **ptrnode);
void parse(FNode **ptrnode);

#endif