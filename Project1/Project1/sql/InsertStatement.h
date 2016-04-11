#ifndef __INSERT_STATEMENT_H__
#define __INSERT_STATEMENT_H__

#include "SQLStatement.h"
#include "SelectStatement.h"

namespace sql {

	struct InsertStatement : SQLStatement {


		InsertStatement() :
			SQLStatement(kStmtInsert),
			tableName(NULL),
			columns(NULL),
			values(NULL) {}

		virtual ~InsertStatement() {
			delete tableName;
			delete columns;
			delete values;
		}

		const char* tableName;
		std::vector<char*>* columns;
		std::vector<Expr*>* values;
	};

}
#endif