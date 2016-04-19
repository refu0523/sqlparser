#ifndef __EXPRESSION_H__
#define __EXPRESSION_H__

#include <stdlib.h>
#include <memory>

namespace sql {

	// Helper function
	char* substr(const char* source, int from, int to);

	typedef enum {
		kExprLiteralString,
		kExprLiteralInt,
		kExprStar,
		kExprColumnRef,
		kExprOperator
	} ExprType;

	typedef struct Expr Expr;


	struct Expr {
		
		typedef enum {
			SIMPLE_OP,
			// Binary
			NOT_EQUALS,
			LESS_EQ,
			GREATER_EQ,
			AND,
			OR,
			// Unary
			NOT,
			UMINUS,
			ISNULL
		} OperatorType;

	

		Expr(ExprType type) :
			type(type),
			expr(NULL),
			expr2(NULL),
			name(NULL),
			table(NULL),
			alias(NULL) {};

		// Interesting side-effect:
		// Making the destructor virtual used to cause segmentation faults
		
		~Expr() {
			delete expr;
			delete expr2;
			delete name;
			delete table;
			delete alias;
		}
		ExprType type;

		Expr* expr=NULL;
		Expr* expr2 = NULL;
		char* name = NULL;
		char* table = NULL;
		char* alias = NULL;
		int64_t ival;

		OperatorType op_type;
		char op_char;

		/**
		* Convenience accessor methods
		*/
		inline bool isType(ExprType e_type) {
			return e_type == type;
		}
		inline bool isLiteral() {
			return isType(kExprLiteralInt) || isType(kExprLiteralString);
		}
		inline bool hasAlias() {
			return alias != NULL;
		}
		inline bool hasTable() {
			return table != NULL;
		}
		inline char* getName() {
			if (alias != NULL) return alias;
			else return name;
		}
		inline bool isSimpleOp() {
			return op_type == SIMPLE_OP;
		}
		inline bool isSimpleOp(char op) {
			return isSimpleOp() && op_char == op;
		}


		/**
		* Static expression constructors
		*/
		static Expr* makeOpUnary(OperatorType op, Expr* expr);
		static Expr* makeOpBinary(Expr* expr1, char op, Expr* expr2);
		static Expr* makeOpBinary(Expr* expr1, OperatorType op, Expr* expr2);

		static Expr* makeLiteral(int64_t val);
		static Expr* makeLiteral(char* val);

		static Expr* makeColumnRef(char* name);
		static Expr* makeColumnRef(char* table, char* name);

	};

#define ALLOC_EXPR(var, type) 		\
	Expr* var;						\
	do {							\
		Expr zero = {type};			\
		var = (Expr*)malloc(sizeof *var);	\
		*var = zero;				\
	} while(0);
#undef ALLOC_EXPR


} // namespace sql

#endif
