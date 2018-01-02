#include "minipascal.h"

static cursor_t _cursor = raw_cursor();

string_t raw_string(size_t leng)
{
    string_t rawdata;

    rawdata = (string_t) malloc((leng + 1) * sizeof(char));
    if(!rawdata) { exit(EXIT_FAILURE); }
    rawdata[leng + 1] = nullchr;
    return rawdata;
}

cursor_t raw_cursor()
{
	cursor_t raw;
	raw.line = 1; raw.nextline = 1;
	return raw;
}

void update_cursor()
{
    string_t text; char c;
    size_t line, size;
    
    /* initialization */
    line = 0; size = 0;
    text = yytext; c = nullchr;

    /* compute column and line */
    _cursor.line = _cursor.nextline;
    while((c = text[size]))
    {
    	/* potential windows linux problem line endings */
        ++size; line += (c == '\n');
    }
    _cursor.nextline += line;
}

void lexer_error()
{
	printf(LEXER_ERROR_MSG, *yytext, _cursor.line);
	exit(EXIT_SUCCESS); /* EXIT_FAILURE */
}

void parser_error()
{
	printf(PARSER_ERROR_MSG, _cursor.line);
	exit(EXIT_SUCCESS); /* EXIT_FAILURE */
}