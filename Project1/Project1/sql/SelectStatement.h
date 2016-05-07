#ifndef __SELECT_STATEMENT_H__
#define __SELECT_STATEMENT_H__

#include "SQLStatement.h"
#include "Expr.h"
#include "Table.h"

namespace sql {

	struct AggregationFunction {
		typedef enum {
			kSum,
			kCount
		} AggregationType;

		AggregationFunction(AggregationType type,Expr* attribute) :
			attribute(attribute),
			type(type){}
		virtual ~AggregationFunction() {
			delete attribute;
		}
		Expr* attribute;
		AggregationType type;
	};
	struct SelectStatement : SQLStatement {
		SelectStatement() :
			SQLStatement(kStmtSelect),
			fromTable(NULL),
			selectList(NULL),
			whereClause(NULL) {};

		virtual ~SelectStatement() {
			delete fromTable;
			delete selectList;
			delete whereClause;
			delete aggregation_list;
		}
		std::vector<AggregationFunction*>* aggregation_list=NULL;
		std::vector<TableRef*>* fromTable = NULL;
		std::vector<Expr*>* selectList = NULL;
		Expr* whereClause = NULL;
		bool IsAggregationExist = false;
		bool IsSelectListExist = false;
		
		inline bool hasAggregation(){
			return IsAggregationExist;
		}
		inline bool hasSelect() {
			return IsSelectListExist;
		}
		inline bool hasWhere() {
			if (whereClause == NULL)
				return false;
			else
				return true;
		}
	};
}
#endif