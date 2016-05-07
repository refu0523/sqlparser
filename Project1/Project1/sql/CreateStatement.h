#ifndef __CREATE_STATEMENT_H__
#define __CREATE_STATEMENT_H__

#include "SQLStatement.h"

namespace sql {
   
    struct ColumnDefinition {
        enum DataType {
            INT,
            VARCHAR
        };

		ColumnDefinition(char* name, DataType type, Expr* ex,bool IsPK) :
			name(name),
			length(ex->ival) ,
            type(type),
			IsPK(IsPK){}

		ColumnDefinition(char* name, DataType type, int length,bool IsPK) :
			name(name),
			length(length),
			type(type),
			IsPK(IsPK){}

        virtual ~ColumnDefinition() {
            delete name;
        }
        char* name = NULL;
		int length;
		bool IsPK;
        DataType type;
    };


    struct CreateStatement : SQLStatement {
    
		const char* tableName;
		const char* attrName;
		std::vector<ColumnDefinition*>* columns;
		std::vector<int>* offsets;
		enum CreateType {
			TABLE,
			INDEX
		};
		enum IndexType {
			HASH,
			TREE
		};
		CreateType createType;
		IndexType indexType;
        CreateStatement() :
            SQLStatement(kStmtCreate),
            tableName(NULL),
            columns(NULL),
			offsets(NULL),
			attrName(NULL){};

        virtual ~CreateStatement() {
            delete columns;
            delete tableName;
			delete offsets;
			delete attrName;
        }
		
      

		inline void calculateOffsets(){
			int offset = 0;
			for (int i = 0; i < columns->size(); i++) {
				offsets->push_back(offset);
				offset += columns->at(i)->length;
			}
		}
    };

} 
#endif