#include "battleships.hpp"

using namespace std;

std::stack<cell *> ctrlz;
sqlite3 *db;
char *em;
bool notfound;
bool exists;
std::string pass;
std::map<std::string, unsigned long long> suspended;

sqlite3 *init_db(){
	srand(time(0));
	db = NULL;
	if (sqlite3_open_v2("stats.db", &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL) != SQLITE_OK || !db){
		cerr << "FAILED TO OPEN DB" << endl;
		exit(0);
	}
	sqlWrapper("CREATE TABLE IF NOT EXISTS `PLAYERS` (`ID` INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL ,`NAME` varchar(255) UNIQUE NOT NULL,""VICTORIES INT,""DEFEATS INT, `PASSWORD` varchar(255) NOT NULL, `KEY` varchar(255),`HINT` varchar(255));", callback);
	stringstream ss;
	ss << "INSERT OR IGNORE INTO PLAYERS(NAME,VICTORIES,DEFEATS)""VALUES('" << "computer" << "', 0, 0);";
	string tmp = ss.str();
	const char *request = tmp.c_str();
	sqlWrapper("INSERT OR IGNORE INTO PLAYERS(NAME,PASSWORD,VICTORIES,DEFEATS)""VALUES('Computer','', 0, 0);", callback);

	return db;
}

void initGlobalVars(){
	exists = true;
	notfound = true;
	db = init_db();
}
