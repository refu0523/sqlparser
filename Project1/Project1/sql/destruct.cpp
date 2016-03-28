
#include "Table.h"
#include "SelectStatement.h"

namespace sql {


    TableRef::~TableRef() {
        delete name;
        delete alias;
        delete select;
        delete list;
    }


} // namespace sql