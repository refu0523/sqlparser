%{


#include "bison_parser.h"
#include "flex_lexer.h"

#include <stdio.h>

using namespace sql;

int yyerror(YYLTYPE* llocp, SQLParserResult** result, yyscan_t scanner, const char *msg) {

	SQLParserResult* list = new SQLParserResult();
	list->isValid = false;
	list->errorMsg = _strdup(msg);
	list->errorLine = llocp->first_line;
	list->errorColumn = llocp->first_column;

	*result = list;
	return 0;
}



%}


// Specify code that is included in the generated .h and .c files
%code requires {
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
}

// Define the names of the created files
%output  "bison_parser.cpp"
%defines "bison_parser.h"

// Tell bison to create a reentrant parser
%define api.pure full

// Prefix the parser
%define api.prefix {sql_}
%define api.token.prefix {SQL_}

%define parse.error verbose
%locations

%initial-action {
	// Initialize
	@$.first_column = 0;
	@$.last_column = 0;
	@$.first_line = 0;
	@$.last_line = 0;
	@$.total_column = 0;
	@$.placeholder_id = 0;
};


%lex-param   { yyscan_t scanner }

// Define additional parameters for yyparse
%parse-param { sql::SQLParserResult** result }
%parse-param { yyscan_t scanner }

%union {
	double fval;
	int64_t ival;
	char* sval;
	uint uval;
	bool bval;

	sql::SQLStatement* statement;
	sql::SelectStatement* 	select_stmt;
	sql::CreateStatement* 	create_stmt;
	sql::InsertStatement* 	insert_stmt;
	sql::PrepareStatement* prep_stmt;

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
}

%token <sval> IDENTIFIER STRING
%token <fval> FLOATVAL
%token <ival> INTVAL
%token <uval> NOTEQUALS LESSEQ GREATEREQ

/* SQL Keywords */
%token VARCHAR BETWEEN
%token INTEGER PRIMARY 
%token CREATE BEFORE COLUMNS PREPARE
%token INSERT SELECT COLUMN
%token TABLES VALUES AFTER
%token INNER 
%token RIGHT TABLE WHERE
%token FROM INTO JOIN LEFT 
%token NULL ADD ALL
%token AND INT KEY NOT AS 
%token OR
%token COUNT SUM 


%type <stmt_list>	statement_list
%type <statement> 	statement preparable_statement
%type <prep_stmt>	prepare_statement
%type <select_stmt> select_statement select_with_paren select_no_paren select_clause
%type <create_stmt> create_statement
%type <insert_stmt> insert_statement
%type <sval> 		table_name opt_alias alias 
%type <uval>	    opt_join_type
%type <table> 		from_clause table_ref table_ref_atomic table_ref_name
%type <table>		join_clause join_table 
%type <expr> 		expr scalar_expr unary_expr binary_expr star_expr expr_alias placeholder_expr
%type <expr> 		column_name literal int_literal num_literal string_literal
%type <expr> 		comp_expr opt_where  
%type <column_t>	column_def
%type <aggregation_t>	aggregation_def

%type <str_vec>		ident_commalist opt_column_list
%type <expr_vec> 	expr_list select_list literal_list
%type <table_vec> 	table_ref_commalist
%type <column_vec>	column_def_commalist
%type <agg_vec>		aggregation_def_commalist


%left		OR
%left		AND
%right		NOT
%right		'=' EQUALS NOTEQUALS 
%nonassoc	'<' '>' LESS GREATER LESSEQ GREATEREQ

%nonassoc	NOTNULL
%nonassoc	ISNULL
%nonassoc	IS				/* sets precedence for IS NULL, etc */
%left		'+' '-'
%left		'*' '/' '%'
%left		'^'

%right 		UMINUS
%left		'[' ']'
%left		'(' ')'
%left		'.'

%%
input:
		statement_list opt_semicolon {
			*result = $1;
		};

statement_list:
		statement { $$ = new SQLParserResult($1); }
	|	statement_list ';' statement { $1->addStatement($3); $$ = $1; };

statement:
		prepare_statement {
			$1->setPlaceholders(yyloc.placeholder_list);
			yyloc.placeholder_list.clear();
			$$ = $1;
		}
	|	preparable_statement;


preparable_statement:
		select_statement { $$ = $1; }
	|	create_statement { $$ = $1; }
	|	insert_statement { $$ = $1; };


prepare_statement:
		PREPARE IDENTIFIER ':' preparable_statement {
			$$ = new PrepareStatement();
			$$->name = $2;
			$$->query = new SQLParserResult($4);
		}
	|	PREPARE IDENTIFIER '{' statement_list opt_semicolon '}' {
			$$ = new PrepareStatement();
			$$->name = $2;
			$$->query = $4;
		};



/*create*/
create_statement:
		
		CREATE TABLE table_name '(' column_def_commalist ')' {
			$$ = new CreateStatement(CreateStatement::kTable);
			$$->tableName = $3;
			$$->columns = $5;
		};


column_def_commalist:
		column_def { $$ = new std::vector<ColumnDefinition*>(); $$->push_back($1); }
	|	column_def_commalist ',' column_def { $1->push_back($3); $$ = $1; };

column_def:
		IDENTIFIER INT {
			$$ = new ColumnDefinition($1,  ColumnDefinition::INT,0);
		}
	|	IDENTIFIER VARCHAR '(' int_literal ')' {
			$$ = new ColumnDefinition($1,  ColumnDefinition::VARCHAR,$4);
		};


/*insert statement*/

insert_statement:
		INSERT INTO table_name opt_column_list VALUES '(' literal_list ')' {
			$$ = new InsertStatement(InsertStatement::kInsertValues);
			$$->tableName = $3;
			$$->columns = $4;
			$$->values = $7;
		}
	|	INSERT INTO table_name opt_column_list select_no_paren {
			$$ = new InsertStatement(InsertStatement::kInsertSelect);
			$$->tableName = $3;
			$$->columns = $4;
			$$->select = $5;
		};


opt_column_list:
		'(' ident_commalist ')' { $$ = $2; }
	|	/* empty */ { $$ = NULL; };

/*select*/

select_statement:
		select_with_paren
	|	select_no_paren;

select_with_paren:
		'(' select_no_paren ')' { $$ = $2; }
	|	'(' select_with_paren ')' { $$ = $2; };

select_no_paren:
		select_clause {
			$$ = $1;
		};


select_clause:
		SELECT select_list from_clause opt_where  {
			$$ = new SelectStatement();
			$$->selectList = $2;
			$$->fromTable = $3;
			$$->whereClause = $4;
		};

select_list:
		expr_list
	|	aggregation_def_commalist;

from_clause:
		FROM table_ref { $$ = $2; };

opt_where:
		WHERE expr { $$ = $2; }
	|	/* empty */ { $$ = NULL; };

aggregation_def_commalist:
		aggregation_def { $$ = new std::vector<AggregationFunction*>(); $$->push_back($1); };
	
aggregation_def:
		COUNT '(' IDENTIFIER ')'{ 
			$$ = new AggregationFunction($3);
		}
	|	SUM	  '(' IDENTIFIER ')'{
			$$ = new AggregationFunction($3);
		};

/*expression*/
expr_list:
		expr_alias { $$ = new std::vector<Expr*>(); $$->push_back($1); }
	|	expr_list ',' expr_alias { $1->push_back($3); $$ = $1; };

literal_list:
		literal { $$ = new std::vector<Expr*>(); $$->push_back($1); }
	|	literal_list ',' literal { $1->push_back($3); $$ = $1; };

expr_alias:
		expr opt_alias {
			$$ = $1;
			$$->alias = $2;
		};

expr:
		'(' expr ')' { $$ = $2; }
	|	scalar_expr
	|	unary_expr
	|	binary_expr;

scalar_expr:
		column_name
	|	star_expr
	|	literal;

unary_expr:
		'-' expr { $$ = Expr::makeOpUnary(Expr::UMINUS, $2); }
	|	NOT expr { $$ = Expr::makeOpUnary(Expr::NOT, $2); };

binary_expr:
		comp_expr
	|	expr '-' expr	{ $$ = Expr::makeOpBinary($1, '-', $3); }
	|	expr '+' expr	{ $$ = Expr::makeOpBinary($1, '+', $3); }
	|	expr '/' expr	{ $$ = Expr::makeOpBinary($1, '/', $3); }
	|	expr '*' expr	{ $$ = Expr::makeOpBinary($1, '*', $3); }
	|	expr '%' expr	{ $$ = Expr::makeOpBinary($1, '%', $3); }
	|	expr '^' expr	{ $$ = Expr::makeOpBinary($1, '^', $3); }
	|	expr AND expr	{ $$ = Expr::makeOpBinary($1, Expr::AND, $3); }
	|	expr OR expr	{ $$ = Expr::makeOpBinary($1, Expr::OR, $3); };


comp_expr:
		expr '=' expr		{ $$ = Expr::makeOpBinary($1, '=', $3); }
	|	expr NOTEQUALS expr	{ $$ = Expr::makeOpBinary($1, Expr::NOT_EQUALS, $3); }
	|	expr '<' expr		{ $$ = Expr::makeOpBinary($1, '<', $3); }
	|	expr '>' expr		{ $$ = Expr::makeOpBinary($1, '>', $3); }
	|	expr LESSEQ expr	{ $$ = Expr::makeOpBinary($1, Expr::LESS_EQ, $3); }
	|	expr GREATEREQ expr	{ $$ = Expr::makeOpBinary($1, Expr::GREATER_EQ, $3); };


column_name:
		IDENTIFIER { $$ = Expr::makeColumnRef($1); }
	|	IDENTIFIER '.' IDENTIFIER { $$ = Expr::makeColumnRef($1, $3); };

literal:
		string_literal
	|	num_literal
	|	placeholder_expr;

string_literal:
		STRING { $$ = Expr::makeLiteral($1); };


num_literal:
		FLOATVAL { $$ = Expr::makeLiteral($1); }
	|	int_literal;

int_literal:
		INTVAL { $$ = Expr::makeLiteral($1); };

star_expr:
		'*' { $$ = new Expr(kExprStar); };


placeholder_expr:
		'?' {
			$$ = Expr::makePlaceholder(yylloc.total_column);
			yyloc.placeholder_list.push_back($$);
		};
		
table_ref:
		table_ref_atomic
	|	table_ref_atomic ',' table_ref_commalist {
			$3->push_back($1);
			auto tbl = new TableRef(kTableCrossProduct);
			tbl->list = $3;
			$$ = tbl;
		};


table_ref_atomic:
		table_ref_name
	|	'(' select_statement ')' alias {
			auto tbl = new TableRef(kTableSelect);
			tbl->select = $2;
			tbl->alias = $4;
			$$ = tbl;
		}
	|	join_clause;


table_ref_commalist:
		table_ref_atomic { $$ = new std::vector<TableRef*>(); $$->push_back($1); }
	|	table_ref_commalist ',' table_ref_atomic { $1->push_back($3); $$ = $1; };


table_ref_name:
		table_name opt_alias {
			auto tbl = new TableRef(kTableName);
			tbl->name = $1;
			tbl->alias = $2;
			$$ = tbl;
		};

table_name:
		IDENTIFIER
	|	IDENTIFIER '.' IDENTIFIER;


alias:	
		AS IDENTIFIER { $$ = $2; }
	|	IDENTIFIER;

opt_alias:
		alias
	|	/* empty */ { $$ = NULL; };


join_clause:
		join_table opt_join_type JOIN join_table 
		{ 
			$$ = new TableRef(kTableJoin);
			$$->join = new JoinDefinition();
			$$->join->type = (JoinType) $2;
			$$->join->left = $1;
			$$->join->right = $4;
		};

opt_join_type:
		INNER 	{ $$ = kJoinInner; }
	|	LEFT 	{ $$ = kJoinLeft; }
	|	RIGHT 	{ $$ = kJoinRight; }
	|	/* empty, default */ 	{ $$ = kJoinInner; };

join_table:
		'(' select_statement ')' alias {
			auto tbl = new TableRef(kTableSelect);
			tbl->select = $2;
			tbl->alias = $4;
			$$ = tbl;
		}
	|	table_ref_name;

opt_semicolon:
		';'
	|	/* empty */;


ident_commalist:
		IDENTIFIER { $$ = new std::vector<char*>(); $$->push_back($1); }
	|	ident_commalist ',' IDENTIFIER { $1->push_back($3); $$ = $1; };

%%


