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
            select(NULL),
            list(NULL),
            join(NULL) {}

         

		virtual TableRef::~TableRef() {
			delete name;
			delete alias;
			delete select;
			delete list;
		}

        TableRefType type;

        char* name;
        char* alias;

        SelectStatement* select;
        std::vector<TableRef*>* list;
        JoinDefinition* join;


        inline char* getName() {
            if (alias != NULL) return alias;
            else return name;
        }
    };


    /**
     * @enum JoinType
     * Types of joins
     */
    typedef enum {
        kJoinInner,
        kJoinOuter,
        kJoinLeft,
        kJoinRight,
    } JoinType;


    /**
     * @struct JoinDefinition
     * @brief Definition of a join table
     */
    struct JoinDefinition {
        JoinDefinition() :
            left(NULL),
            right(NULL),
            condition(NULL),
            type(kJoinInner) {}

        virtual ~JoinDefinition() {
            delete left;
            delete right;
            delete condition;
        }

        TableRef* left;
        TableRef* right;
        Expr* condition;

        JoinType type;
    };



} // namespace sql
#endif
