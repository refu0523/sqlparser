
#include <stdlib.h>
#include <string>
#include <iostream>
#include "SQLParser.h"
#include "sqlhelper.h"
using namespace std;
using namespace sql;

int main(int argc, char *argv[]) {
	string s;
	while (1) {

	string query;//= "select a from b;\ninsert into a values (1);";
	getline(cin, query);
	

	// parse a given query
	SQLParserResult* result = SQLParser::parseSQLString(query);

	// check whether the parsing was successful
	if (result->isValid) {
		printf("Parsed successfully!\n");
		printf("Number of statements: %lu\n", result->size());
		SelectStatement* se_st;
		InsertStatement* in_st;
		CreateStatement* cr_st;
		for (SQLStatement* stmt : result->statements) {
			// process the statements...
			//printStatementInfo(stmt);
			switch (stmt->type()) {
			case kStmtSelect:
				se_st = (SelectStatement*)stmt;
				cout << se_st->hasAggregation() << endl;
			
				break;
			case kStmtInsert:
				in_st = (InsertStatement*)stmt;
				break;
			case kStmtCreate:
				cr_st = (CreateStatement*)stmt;
				cout << "cr_st ,Test" << cr_st->columns->front()->length << endl;
				cout << "cr_st " << cr_st->tableName << endl;
				cout << "cr_st " << cr_st->columns->front()->length<<endl;
				cout << "cr_st " << cr_st->columns->front()->name<<endl;
				cout << "cr_st " << cr_st->columns->front()->type << endl;
				break;
			default:
				break;
			}
		}
		
	}
	else {
		printf("Invalid SQL!\n");
	
	}
	}
}