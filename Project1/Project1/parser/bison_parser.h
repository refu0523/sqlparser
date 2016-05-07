/* A Bison parser, made by GNU Bison 3.0.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2013 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

#ifndef YY_SQL_BISON_PARSER_H_INCLUDED
# define YY_SQL_BISON_PARSER_H_INCLUDED
/* Debug traces.  */
#ifndef SQL_DEBUG
# if defined YYDEBUG
#if YYDEBUG
#   define SQL_DEBUG 1
#  else
#   define SQL_DEBUG 0
#  endif
# else /* ! defined YYDEBUG */
#  define SQL_DEBUG 0
# endif /* ! defined YYDEBUG */
#endif  /* ! defined SQL_DEBUG */
#if SQL_DEBUG
extern int sql_debug;
#endif
/* "%code requires" blocks.  */
#line 29 "bison_parser.y" /* yacc.c:1909  */

// %code requires block	

#include "../sql/statements.h"
#include "../SQLParserResult.h"
#include "parser_typedef.h"

// Auto update column and line number
#define YY_USER_ACTION \
    yylloc->first_line = yylloc->last_line; \
    yylloc->first_column = yylloc->last_column; \
    for(int i = 0; yytext[i] != '\0'; i++) { \
    	yylloc->total_column++; \
        if(yytext[i] == '\n') { \
            yylloc->last_line++; \
            yylloc->last_column = 0; \
        } \
        else { \
            yylloc->last_column++; \
        } \
    }

#line 75 "bison_parser.h" /* yacc.c:1909  */

/* Token type.  */
#ifndef SQL_TOKENTYPE
# define SQL_TOKENTYPE
  enum sql_tokentype
  {
    SQL_IDENTIFIER = 258,
    SQL_STRING = 259,
    SQL_FLOATVAL = 260,
    SQL_INTVAL = 261,
    SQL_NOTEQUALS = 262,
    SQL_LESSEQ = 263,
    SQL_GREATEREQ = 264,
    SQL_VARCHAR = 265,
    SQL_BETWEEN = 266,
    SQL_INTEGER = 267,
    SQL_PRIMARY = 268,
    SQL_CREATE = 269,
    SQL_BEFORE = 270,
    SQL_COLUMNS = 271,
    SQL_PREPARE = 272,
    SQL_INSERT = 273,
    SQL_SELECT = 274,
    SQL_COLUMN = 275,
    SQL_TABLES = 276,
    SQL_VALUES = 277,
    SQL_AFTER = 278,
    SQL_INNER = 279,
    SQL_RIGHT = 280,
    SQL_TABLE = 281,
    SQL_WHERE = 282,
    SQL_FROM = 283,
    SQL_INTO = 284,
    SQL_LEFT = 285,
    SQL_NULL = 286,
    SQL_ADD = 287,
    SQL_ALL = 288,
    SQL_AND = 289,
    SQL_INT = 290,
    SQL_KEY = 291,
    SQL_NOT = 292,
    SQL_AS = 293,
    SQL_OR = 294,
    SQL_ON = 295,
    SQL_INDEX = 296,
    SQL_COUNT = 297,
    SQL_SUM = 298,
    SQL_HASH = 299,
    SQL_TREE = 300,
    SQL_EQUALS = 301,
    SQL_LESS = 302,
    SQL_GREATER = 303,
    SQL_NOTNULL = 304,
    SQL_ISNULL = 305,
    SQL_IS = 306,
    SQL_UMINUS = 307
  };
#endif

/* Value type.  */
#if ! defined SQL_STYPE && ! defined SQL_STYPE_IS_DECLARED
typedef union SQL_STYPE SQL_STYPE;
union SQL_STYPE
{
#line 83 "bison_parser.y" /* yacc.c:1909  */

	double fval;
	int64_t ival;
	char* sval;
	uint uval;
	bool bval;

	sql::SQLStatement* statement;
	sql::SelectStatement* 	select_stmt;
	sql::CreateStatement* 	create_stmt;
	sql::InsertStatement* 	insert_stmt;
	
	
	sql::TableRef* table;
	sql::Expr* expr;
	sql::ColumnDefinition* column_t;
	sql::AggregationFunction* aggregation_t;

	sql::SQLParserResult* stmt_list;

	std::vector<char*>* str_vec;
	std::vector<sql::TableRef*>* table_vec;
	std::vector<sql::ColumnDefinition*>* column_vec;
	std::vector<sql::Expr*>* expr_vec;
	std::vector<sql::AggregationFunction*>* agg_vec;


#line 168 "bison_parser.h" /* yacc.c:1909  */
};
# define SQL_STYPE_IS_TRIVIAL 1
# define SQL_STYPE_IS_DECLARED 1
#endif

/* Location type.  */
#if ! defined SQL_LTYPE && ! defined SQL_LTYPE_IS_DECLARED
typedef struct SQL_LTYPE SQL_LTYPE;
struct SQL_LTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};
# define SQL_LTYPE_IS_DECLARED 1
# define SQL_LTYPE_IS_TRIVIAL 1
#endif



int sql_parse (sql::SQLParserResult** result, yyscan_t scanner);

#endif /* !YY_SQL_BISON_PARSER_H_INCLUDED  */
