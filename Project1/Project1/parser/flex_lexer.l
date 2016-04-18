
%{

#include "../sql/Expr.h"
#include "bison_parser.h"
#include <stdio.h>
#define YY_NO_UNISTD_H
#define TOKEN(name) { return SQL_##name; }

%}


/* Define the output files */
%option header-file="flex_lexer.h"
%option outfile="flex_lexer.cpp"

/* Make reentrant */
%option reentrant
%option bison-bridge
%option nounistd
/* performance tweeks */
%option never-interactive
%option batch

/* other flags */
%option noyywrap
%option nounput
%option warn
%option case-insensitive
%option prefix="sql_"
%option bison-locations 
/* %option nodefault */


%s COMMENT


%%

--				BEGIN(COMMENT);
<COMMENT>[^\n]*	/* skipping comment content until a end of line is read */;
<COMMENT>\n 	BEGIN(INITIAL);


[ \t\n]+	/* skip whitespace */;





VARCHAR		TOKEN(VARCHAR)
BETWEEN		TOKEN(BETWEEN)
COLUMNS		TOKEN(COLUMNS)
INTEGER		TOKEN(INTEGER)
PREPARE		TOKEN(PREPARE)
PRIMARY		TOKEN(PRIMARY)
BEFORE		TOKEN(BEFORE)
CREATE		TOKEN(CREATE)
INSERT		TOKEN(INSERT)
SELECT		TOKEN(SELECT)
TABLES		TOKEN(TABLES)
VALUES		TOKEN(VALUES)
AFTER		TOKEN(AFTER)
INNER		TOKEN(INNER)
RIGHT		TOKEN(RIGHT)
TABLE		TOKEN(TABLE)
WHERE		TOKEN(WHERE)
FROM		TOKEN(FROM)
INTO		TOKEN(INTO)
LEFT		TOKEN(LEFT)
NULL		TOKEN(NULL)
ADD			TOKEN(ADD)
ALL			TOKEN(ALL)
AND			TOKEN(AND)
INT			TOKEN(INT)
KEY			TOKEN(KEY)
NOT			TOKEN(NOT)
AS			TOKEN(AS)
OR			TOKEN(OR)
COUNT		TOKEN(COUNT)
SUM			TOKEN(SUM)

"<>" 		TOKEN(NOTEQUALS)
"<="		TOKEN(LESSEQ)
">="		TOKEN(GREATEREQ)


[-+*/(){},.;<>=^%:?]	{ return yytext[0]; }


[0-9]+"."[0-9]* |
"."[0-9]*	{
	yylval->fval = atof(yytext);
	return SQL_FLOATVAL;
}

[0-9]+	{
	yylval->ival = atol(yytext);
	return SQL_INTVAL;
}

\"[^\"\n]+\" {
	// Crop the leading and trailing quote char
	yylval->sval = sql::substr(yytext, 1, strlen(yytext)-1);
	return SQL_IDENTIFIER;
}

[A-Za-z][A-Za-z0-9_]* {
	yylval->sval = _strdup(yytext);
	return SQL_IDENTIFIER;
}


'[^'\n]*' {
	// Crop the leading and trailing quote char
	yylval->sval = sql::substr(yytext, 1, strlen(yytext)-1);
	return SQL_STRING;
}

. { fprintf(stderr, "[SQL-Lexer-Error] Unknown Character: %c\n", yytext[0]); return 0; }


%%
int yyerror(const char *msg) {
    fprintf(stderr, "[SQL-Lexer-Error] %s\n",msg); return 0;
}