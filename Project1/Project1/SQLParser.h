#ifndef __SQLPARSER_H_
#define __SQLPARSER_H_

#include "SQLParserResult.h"
#include "sql/statements.h"

namespace sql {

	class SQLParser {
	public:
		static SQLParserResult* parseSQLString(const char* sql);
		static SQLParserResult* parseSQLString(const std::string& sql);

	private:
		SQLParser();
	};


}


#endif