#include "battleships.hpp"

using namespace std;

int getPassword(void *NotUsed, int argc, char **argv, char **azColName) {
	pass = argv[1];
	return 0;
}

int getKey(void *NotUsed, int argc, char **argv, char **azColName){
	string *s = reinterpret_cast<string *>(NotUsed);
	if (argv[0])
		*s = argv[0];
	else
		*s = "";
	return 0;
}

int callback(void *NotUsed, int argc, char **argv, char **azColName) {
	if (NotUsed){
		string p1 = reinterpret_cast<pair<const string &, const string &> *>(NotUsed)->first;
		string p2 = reinterpret_cast<pair<const string &, const string &> *>(NotUsed)->second;
		if (p1 == argv[0])
			cout << GREEN << left << setw(11) << argv[0] << setw(10) << argv[1] << setw(8) << argv[2] << RESET << endl;
		else if (p2 == argv[0])
			cout << RED << left << setw(11) << argv[0] << setw(10) << argv[1] << setw(8) << argv[2] << RESET << endl;
		else
			cout << left << setw(11) << argv[0] << setw(10) << argv[1] << setw(8) << argv[2] << endl;
	}
	else if(exists){
		cout << left << setw(11) << argv[0] << setw(10) << argv[1] << setw(8) << argv[2] << endl;
		notfound = false;
	}
	exists = true;
	return 0;
}

void sqlWrapper(string req, int (*func)(void *, int, char **, char **)){
	if (sqlite3_exec(db, req.c_str(), func, NULL, &em) != SQLITE_OK){
			cerr << RED << req << RESET << endl;
			cerr << em << endl;
	}
}

void sqlWrapper(string req, int (*func)(void *, int, char **, char **), void *p){
	if (sqlite3_exec(db, req.c_str(), func, p, &em) != SQLITE_OK){
			cerr << RED << req << RESET << endl;
			cerr << em << endl;
	}
}
