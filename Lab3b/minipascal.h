#ifndef MINIPASCAL_H
#define MINIPASCAL_H

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#define TEOF 0
#define PARSER_SUCCESS_MSG "ACCEPTED\n"

#define true 1
#define false 0
#define nullptr 0
#define nullchr '\0'
#define nullstr nullptr
#define eunknown 0
#define efirst 1

typedef int bool_t;
typedef char* string_t;
typedef void* pointer_t;

extern string_t lexeme(string_t into, int* ptrleng);
extern string_t yytext;
extern size_t yyleng;

typedef enum operator_e
{
	eset = efirst, egt, elt ,eget, elet ,eneq, eadd, esub, emul, eidiv, eimod, eeq
} operator_e;

typedef enum indetifier_e
{
	eprogram = efirst, efunction, eprocedure, evariable
} indetifier_e;

typedef enum datatype_e
{
	ereal = efirst, einteger, eboolean, evoid
} datatype_e;

typedef struct cursor_t
{
	size_t line, column, bytes, nextline, nextcolumn, nextbytes;
} cursor_t;

typedef struct datatype_t
{
	datatype_e type;
	bool_t isarray, isconstant, isunique;
} datatype_t;

typedef struct identifier_t
{
	indetifier_e type;
	string_t lexeme;
	cursor_t cursor;
	datatype_t datatype;
	size_t declarationcount, argumentcount;
	struct identifier_t *declarations, *arguments;
} identifier_t;

string_t raw_string(size_t leng);
cursor_t raw_cursor();
datatype_t raw_datatype();
identifier_t raw_identifier();

identifier_t* new_identifer(size_t size);

bool_t check(const datatype_t lhs, const datatype_t rhs);
datatype_t datatype_assign(const datatype_t lhs, const datatype_t rhs);
datatype_t datatype_contract(const datatype_t lhs, const datatype_t rhs);

void install(identifier_t identifier);
bool_t lookup(string_t lexeme, identifier_t *ptridentifier);

void update_cursor();
void lexer_error();
void parser_error();
void sematic_error(const datatype_t lhs, const datatype_t rhs);

#endif