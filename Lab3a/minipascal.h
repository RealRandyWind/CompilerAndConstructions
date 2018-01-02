#ifndef MINIPASCAL_H
#define MINIPASCAL_H

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#define NEXT_LINE 1
#define NEXT_LINE_COLUMN 1
#define TEOF 0
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

extern string_t lexeme(string_t into, int* ptrleng);
extern string_t yytext;
extern size_t yyleng;

typedef struct cursor_t
{
	size_t line, nextline;
} cursor_t;

string_t raw_string(size_t leng);
cursor_t raw_cursor();
void update_cursor();
void lexer_error();
void parser_error();

#endif