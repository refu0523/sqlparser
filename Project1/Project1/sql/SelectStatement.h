#ifndef __SELECT_STATEMENT_H__
#define __SELECT_STATEMENT_H__

#include "SQLStatement.h"
#include "Expr.h"
#include "Table.h"

namespace sql {

	struct AggregationFunction {
		AggregationFunction(char* attribute) :
			attribute(attribute) {}
		virtual ~AggregationFunction() {
			delete attribute;
		}
			char* attribute;
	};

    struct SelectStatement : SQLStatement {
        SelectStatement() :
            SQLStatement(kStmtSelect),
            fromTable(NULL),
            selectList(NULL),
            whereClause(NULL),
            unionSelect(NULL) {};

        virtual ~SelectStatement() {
            delete fromTable;
            delete selectList;
            delete whereClause;          
        }

        TableRef* fromTable;
        std::vector<Expr*>* selectList;
		std::vector<AggregationFunction*>* aggregation_list;
        Expr* whereClause;
		SelectStatement* unionSelect;
    };

} // namespace sql
#endif