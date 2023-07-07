#include "1.h"
#include "sqlite3.h"
#include <stdio.h>

using namespace std;

const char* SQL = "CREATE TABLE IF NOT EXISTS users(id INTEGER PRIMARY KEY AUTOINCREMENT UNIQUE, username UNIQUE, password)";
const char* SQL1 = "INSERT INTO users(username, password) VALUES('GsD7', 1234)";
const char* SQL2 = "SELECT * FROM users WHERE username='GsD10'";
bool flag = true;
bool flag1 = false;


static int found_user(void* unused, int count, char** data, char** columns)
{
	cout << "user already exist"; flag = false;
	return 0;
}

static int deft(void* unused, int count, char** data, char** columns)
{
	return 0;
}

static int got_pair(void* unused, int count, char** data, char** columns)
{
	cout << "pair of login and password is valid"; flag1 = true;
	return 0;
}


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


int main(int argc, char *argv[])
{
	if (argc >= 2) {
		std::string act = argv[1];
		if (act == "reg") {
			std::string user = argv[2];
			std::string password = argv[3];
			std::string sql = "SELECT * FROM users WHERE username='" + user + "'";
			const char* SQL = sql.c_str();
			Database db;
			db.open();
			db.execute(SQL, found_user);
			if (flag) {
				std::string sql1 = "INSERT INTO users(username, password) VALUES('" + user + "', " + password + ")";
				const char* SQL1 = sql1.c_str();
				db.execute(SQL1, deft);
			}
		}
		else if (act == "ent") {
			std::string user = argv[2];
			std::string password = argv[3];
			std::string sql = "SELECT * FROM users WHERE username='" + user + "' AND password='" + password + "'";
			const char* SQL = sql.c_str();
			Database db;
			db.open();
			db.execute(SQL, got_pair);
			std::string sql1 = "SELECT * FROM users WHERE username='" + user + "' AND password=" + password;
			const char* SQL1 = sql1.c_str();
			db.execute(SQL1, got_pair);
			if (not flag1) {
				cout << "invalid login or password";
			}
		}
		return 0;
	}
	Database db;
	cout << db.open() << "\n";
	cout << db.execute(SQL, deft) << "\n";
	cout << db.execute(SQL1, deft) << "\n";
	cout << db.execute(SQL2, deft) << "\n";
	db.close();

	//cout << argc << "  " << argv[0] << endl;
	return 0;
}
