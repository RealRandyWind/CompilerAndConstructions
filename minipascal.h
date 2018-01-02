#ifndef MINIPASCAL_H
#define MINIPASCAL_H

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#define NEXT_LINE 1
#define NEXT_LINE_COLUMN 1
#define TEOF 0
#define SEMANTIC_ERROR_MSG "TYPE MISSMATCH (%s)"
#define PARSER_ERROR_MSG "PARSE ERROR (%lu)\n"
#define LEXER_ERROR_MSG "INVALID CHARACTER \'%c\' (%lu)\n"
#define PARSER_SUCCESS_MSG "ACCEPTED\n"


#define true 1
#define false 0
#define nullptr 0
#define nullchr '\0'
#define nullstr ""

typedef int bool_t;
typedef char* string_t;
typedef void* pointer_t;

extern string_t lexeme(string_t into, int* ptrleng);
extern char* yytext;
extern size_t yyleng;

typedef enum operator_e
{
	eset, egt, elt ,eget, elet ,eneq, eadd, esub, emul, eidiv, eimod, eeq
} operator_e;

typedef enum indetifier_e
{
	eprogram, efunction, eprocedure, evariable
} indetifier_e;

typedef enum datatype_e
{
	ereal, einteger, eboolean
} datatype_e;

typedef struct cursor_t
{
	string_t file;
	size_t line, column, nextline, nextcolumn;
} cursor_t;

typedef struct datatype_t
{
	datatype_e type;
	bool_t isarray, isconstant, isargument, isreturn;
} datatype_t;

typedef struct array_t
{
	size_t upper, lower;
	pointer_t data;
} array_t;

typedef union value_u
{
	double real;
	int integer;
	bool_t boolean;
	struct array_t array;
} value_u;

typedef struct identifier_t
{
	indetifier_e type;
	string_t lexeme;
	cursor_t cursor;
	datatype_t datatype;
	value_u value;
	size_t scope, size;
	struct identifier_t *declarations;
} identifier_t;

identifier_t* new_identifier(size_t n);
identifier_t raw_identifier();
cursor_t raw_cursor();
cursor_t cpy_cursor(const cursor_t cursor);

identifier_t* lookup(string_t lexeme, identifier_t* ptridentifer);
void install(identifier_t* identifier);
void update_cursor();
void lexer_error();
void parser_error();
void semantic_error();

#endif