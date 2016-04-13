#ifndef __TABLEREF_H__
#define __TABLEREF_H__

#include "Expr.h"
#include <stdio.h>
#include <vector>

namespace sql {

    struct SelectStatement;
    struct JoinDefinition;
    struct TableRef;
    typedef enum {
        kTableName,
        kTableSelect,
        kTableJoin,
        kTableCrossProduct
    } TableRefType;


    struct TableRef {
        TableRef(TableRefType type) :
            type(type),
            name(NULL),
            alias(NULL),
            list(NULL) {}

		virtual TableRef::~TableRef() {
			delete name;
			delete alias;
			delete list;
		}

        TableRefType type;

        char* name;
        char* alias;

        std::vector<TableRef*>* list;


        inline char* getName() {
            if (alias != NULL) return alias;
            else return name;
        }
    };




} // namespace sql
#endif
