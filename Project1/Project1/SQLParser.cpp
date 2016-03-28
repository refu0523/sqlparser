
#include "SQLParser.h"
#include "parser/bison_parser.h"
#include "parser/flex_lexer.h"
#include <stdio.h>
#include <string>


namespace sql {

    SQLParser::SQLParser() {
        fprintf(stderr, "SQLParser only has static methods atm! Do not initialize!\n");
    }


    SQLParserResult* SQLParser::parseSQLString(const char *text) {
        SQLParserResult* result = NULL;
        yyscan_t scanner;
        YY_BUFFER_STATE state;

        if (sql_lex_init(&scanner)) {
            // couldn't initialize
            fprintf(stderr, "[Error] SQLParser: Error when initializing lexer!\n");
            return NULL;
        }

        state = sql__scan_string(text, scanner);

        if (sql_parse(&result, scanner)) {
            // Returns an error stmt object
            return result;
        }

        sql__delete_buffer(state, scanner);

        sql_lex_destroy(scanner);
        return result;
    }


    SQLParserResult* SQLParser::parseSQLString(const std::string& text) {
        return parseSQLString(text.c_str());
    }


} // namespace sql