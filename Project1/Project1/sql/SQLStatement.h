#ifndef __SQLSTATEMENT_H__
#define __SQLSTATEMENT_H__

#include "Expr.h"
#include <vector>
typedef unsigned int uint;
namespace sql {
	typedef enum {
		kStmtError,
		kStmtSelect,
		kStmtInsert,
		kStmtCreate,
	} StatementType;

	struct SQLStatement {
		SQLStatement(StatementType type) :
			_type(type) {};

		virtual ~SQLStatement() {}

		virtual StatementType type() {
			return _type;
		}

	private:
		StatementType _type;
	};

}
#endif
