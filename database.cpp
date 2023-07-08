#include "database.h"
using namespace std;


namespace db {
	class Database {
	private:
		sqlite3* db = 0;
		char* err = 0;
	public:
		const char* open() {
			if (sqlite3_open("out\\build\\x64-debug\\database.dblite", &db)) {
				return sqlite3_errmsg(db);
			}
			else {
				return "0";
			}
		}

		const char* execute(const char* sql, static int callback(void* unused, int count, char** data, char** columns)) {
			if (sqlite3_exec(db, sql, callback, NULL, &err)) {
				return err;
			}
			else {
				return "0";
			}
		}

		void close() {
			sqlite3_close(db);
		}
	};
}