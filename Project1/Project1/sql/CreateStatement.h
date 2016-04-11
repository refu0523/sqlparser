#ifndef __CREATE_STATEMENT_H__
#define __CREATE_STATEMENT_H__

#include "SQLStatement.h"

namespace sql {
   
    struct ColumnDefinition {
        enum DataType {
            INT,
            VARCHAR
        };

		ColumnDefinition(char* name, DataType type, Expr* ex) :
			name(name),
			length(ex->ival) ,
            type(type) {}

		ColumnDefinition(char* name, DataType type, int length) :
			name(name),
			length(length),
			type(type) {}

        virtual ~ColumnDefinition() {
            delete name;
        }
        char* name;
		int length;
        DataType type;
    };


    struct CreateStatement : SQLStatement {
    

        CreateStatement() :
            SQLStatement(kStmtCreate),
            tableName(NULL),
            columns(NULL) {};

        virtual ~CreateStatement() {
            delete columns;
            delete tableName;
        }

        const char* tableName;
        std::vector<ColumnDefinition*>* columns;
    };

} 
#endif