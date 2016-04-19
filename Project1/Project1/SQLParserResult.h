#ifndef __SQLPARSERRESULT__
#define __SQLPARSERRESULT__
#include "sql/SQLStatement.h"

namespace sql {

	class SQLParserResult {
	public:

		SQLParserResult();
		SQLParserResult(SQLStatement* stmt);
		virtual ~SQLParserResult();

		void addStatement(SQLStatement* stmt);
		SQLStatement* getStatement(int id);
		size_t size();
		std::vector<SQLStatement*>statements;
		bool isValid;

		const char* errorMsg= NULL;
		int errorLine;
		int errorColumn;
	};

}

#endif // __SQLPARSERRESULT__