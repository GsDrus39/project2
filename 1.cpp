#include "1.h"
#include "sqlite3.h"
#include <stdio.h>

using namespace std;

const char* SQL = "CREATE TABLE IF NOT EXISTS users(id INTEGER PRIMARY KEY AUTOINCREMENT UNIQUE, username UNIQUE, password); INSERT INTO users(username, password) VALUES('GsD', 1234)";


class Database {
private:
	sqlite3* db = 0;
	char* err = 0;
public:
	const char* open() {
		if (sqlite3_open("database.dblite", &db)) {
			return sqlite3_errmsg(db);
		}
		else {
			return "0";
		}
	}

	const char* execute(const char* sql) {
		if (sqlite3_exec(db, sql, 0, 0, &err)) {
			const char* a = err;
			//sqlite3_free(err);
			return a;
		}
		else {
			return "0";
		}
	}

	void close() {
		sqlite3_close(db);
	}

};


int main(int argc, char *argv[])
{
	Database db;
	cout << db.open() << "\n";
	cout << db.execute(SQL);
	db.close();

	//cout << argc << "  " << argv[0] << endl;
	return 0;
}
