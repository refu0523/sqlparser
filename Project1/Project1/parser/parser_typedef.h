#ifndef __PARSER_TYPEDEF_H__
#define __PARSER_TYPEDEF_H__

#include <vector>


#ifndef YYtypeDEF_YY_SCANNER_T
#define YYtypeDEF_YY_SCANNER_T
typedef void* yyscan_t;
#endif


#define YYSTYPE SQL_STYPE
#define YYLTYPE SQL_LTYPE


struct SQL_CUST_LTYPE {
	int first_line;
	int first_column;
	int last_line;
	int last_column;

	int total_column;

	// Placeholder
	int placeholder_id;
	std::vector<void*> placeholder_list;
};

#define SQL_LTYPE SQL_CUST_LTYPE
#define SQL_LTYPE_IS_DECLARED 1




#endif