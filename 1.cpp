#include "1.h"
#include "sqlite3.h"
#include <stdio.h>

using namespace std;

const char* SQL = "CREATE TABLE IF NOT EXISTS users(username, password);";

class Database {
private:
	
public:

};


int main(int argc, char *argv[])
{
	sqlite3* db = 0;
	char* err = 0;


	if (sqlite3_open("database.dblite", &db))
		cout << "Ошибка открытия/создания БД: " << sqlite3_errmsg(db);

	else if (sqlite3_exec(db, SQL, 0, 0, &err))
	{
		cout << "Ошибка SQL: " << err;
		sqlite3_free(err);
	}

	sqlite3_close(db);

	cout << argc << "  " << argv[0] << endl;
	return 0;
}
