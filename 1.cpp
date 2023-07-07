#include "1.h"
#include "sqlite3.h"
#include <stdio.h>
#include <random>
#include <stdlib.h>
#include <fstream>
#include <string>

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


class field {
private:
	int minesNumber = 15;
	int fld[10][10];
	int count_mines(int x, int y) {
		if (fld[x][y] == 10) {
			return 10;
		}
		int mines = 0;
		for (int k = -1; k < 2; k++)
		{
			for (int l = -1; l < 2; l++)
			{
				int posI = x + k;
				int posJ = y + l;
				if (l != 2 && k != 2 && posI >= 0 && posI < 10 && posJ >= 0 && posJ < 10 && fld[posI][posJ] == 10)
					mines++;
			}
		}
		return mines;
	}
public:
	int flags[10][10];
	void OpenButton(int i, int j, bool userClick) {
		if (fld[i][j] != 9)
		{
			if (!userClick) return;
			if (fld[i][j] == 10) { cout << i << " " << j << " " << fld[i][j] << endl; return; };
			cout << i << " " << j << " " << fld[i][j] << endl;
			return;
		}
		int a = count_mines(i, j);
		if (a != 0) { cout << i << " " << j << " " << a << endl; fld[i][j] = a;  return; }
		fld[i][j] = 11;
		for (int k = -1; k < 2; k++)
		{
			for (int l = -1; l < 2; l++)
			{
				int posI = i + k;
				int posJ = j + l;
				if (l != 2 && k != 2 && posI >= 0 && posI < 10 && posJ >= 0 && posJ < 10 && fld[posI][posJ] != 10)
				{
					cout << posI << " " << posJ << " " << fld[posI][posJ] << "\n";
					if (fld[posI][posJ] == 9)
						OpenButton(posI, posJ, false);
				}
			}
		}
		std::ofstream out;
		out.open("f.txt");
		if (out.is_open())
		{
			for (int i = 0; i < 10; i++) {
				for (int j = 0; j < 10; j++) {
					out << fld[i][j] << std::endl;
				}
			}

		}
		out.close();
	}

	void unpack() {
		std::string line;
		std::ifstream in("f.txt");
		if (in.is_open())
		{
			for (int i = 0; i < 10; i++) {
				for (int j = 0; j < 10; j++) {
					std::getline(in, line);
					std::string a = line;
					fld[i][j] = atoi(a.c_str());
				}
			}
		}
		in.close();
		std::string line1;
		std::ifstream in1("fl.txt");
		if (in1.is_open())
		{
			for (int i = 0; i < 10; i++) {
				for (int j = 0; j < 10; j++) {
					std::getline(in1, line1);
					std::string a = line1;
					flags[i][j] = atoi(a.c_str());
				}
			}
		}
		in1.close();
	}
	
	void init(int x, int y) {
		for (int i = 0; i < 10; i++) {
			for (int j = 0; j < 10; j++) {
				fld[i][j] = 9;
				flags[i][j] = 0;
				//cout << i << " " << j << " " << fld[i][j] << "\n";
			}
		}
		random_device rd;
		mt19937 gen(rd());
		uniform_int_distribution<> dist(0, 9);
		for (int i = 0; i < minesNumber; i++) {
			int a, b;
			do {
				a = dist(gen);
				b = dist(gen);
			} while ((a == x and b == y) or (fld[a][b] == 10));
			fld[a][b] = 10;
			//cout << a << " " << b << " " << fld[a][b] << "\n";
		}
		int mines = 0;
		for (int k = -1; k < 2; k++)
		{
			for (int l = -1; l < 2; l++)
			{
				int posI = x + k;
				int posJ = y + l;
				if (l != 2 && k != 2 && posI >= 0 && posI < 10 && posJ >= 0 && posJ < 10 && fld[posI][posJ] == 10)
					mines++;
			}
		}
		fld[x][y] = mines;
		if (mines == 0) {
			fld[x][y] = 9;
			OpenButton(x, y, true);
		}
		else {
			cout << x << " " << y << " " << mines << "\n";
		}
		std::ofstream out;
		out.open("f.txt");
		if (out.is_open())
		{
			for (int i = 0; i < 10; i++) {
				for (int j = 0; j < 10; j++) {
					out << fld[i][j] << std::endl;
				}
			}
			
		}
		out.close();
		std::ofstream out1;
		out1.open("fl.txt");
		if (out1.is_open())
		{
			for (int i = 0; i < 10; i++) {
				for (int j = 0; j < 10; j++) {
					out1 << flags[i][j] << std::endl;
				}
			}

		}
		out1.close();
	}
	int get_cell(int x, int y) {
		return fld[x][y];
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
		else if (act == "cre") {
			std::string a = argv[2];
			std::string b = argv[3];
			int i = atoi(a.c_str());
			int j = atoi(b.c_str());
			field field;
			field.init(i, j);
		}
		else if (act == "opn") {
			std::string act1 = argv[4];
			std::string a = argv[2];
			std::string b = argv[3];
			int i = atoi(a.c_str());
			int j = atoi(b.c_str());
			field field;
			field.unpack();
			if (act1 == "Left") {
				field.OpenButton(i, j, true);
			}
			else {
				if (field.flags[i][j] == 1) {
					cout << a << " " << b << " " << 13;
					field.flags[i][j] = 0;
				}
				else {
					cout << a << " " << b << " " << 12;
					field.flags[i][j] = 1;
				}
				std::ofstream out;
				out.open("f.txt");
				if (out.is_open())
				{
					for (int i = 0; i < 10; i++) {
						for (int j = 0; j < 10; j++) {
							out << field.get_cell(i, j) << std::endl;
						}
					}

				}
				out.close();
				std::ofstream out1;
				out1.open("fl.txt");
				if (out1.is_open())
				{
					for (int i = 0; i < 10; i++) {
						for (int j = 0; j < 10; j++) {
							out1 << field.flags[i][j] << std::endl;
						}
					}

				}
				out1.close();
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
