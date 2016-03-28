#ifndef __PREPARE_STATEMENT_H__
#define __PREPARE_STATEMENT_H__

#include "../SQLParserResult.h"
#include "SQLStatement.h"
#include "SelectStatement.h"
#include <algorithm>

namespace sql {
    
    struct PrepareStatement : SQLStatement {
        PrepareStatement() :
            SQLStatement(kStmtPrepare),
            name(NULL),
            query(NULL) {}

        virtual ~PrepareStatement() {
            delete query;
            delete name;
        }

       
        void setPlaceholders(std::vector<void*> ph) {
            for (void* e : ph) {
                if (e != NULL)
                    placeholders.push_back((Expr*) e);
            }
            // Sort by col-id
            std::sort(placeholders.begin(), placeholders.end(), [](Expr* i, Expr* j) -> bool { return (i->ival < j->ival); });

            // Set the placeholder id on the Expr. This replaces the previously stored column id
            for (uint i = 0; i < placeholders.size(); ++i) placeholders[i]->ival = i;
        }

        const char* name;
        SQLParserResult* query;
        std::vector<Expr*> placeholders;
    };

} // namsepace sql
#endif