#include "battleships.hpp"

using namespace std;

std::stack<cell *> ctrlz;
sqlite3 *db;
char *em;
bool notfound;
bool exists;
std::string pass;
unsigned long long suspended;

sqlite3 *init_db(){
	db = NULL;
	if (sqlite3_open_v2("stats.db", &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL) != SQLITE_OK || !db){
		cerr << "FAILED TO OPEN DB" << endl;
		exit(0);
	}
	sqlWrapper("CREATE TABLE IF NOT EXISTS `PLAYERS` (`ID` INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL ,`NAME` varchar(255) UNIQUE NOT NULL,`PASSWORD` varchar(255) NOT NULL,""VICTORIES INT,""DEFEATS INT);", callback);
	return db;
}

void initGlobalVars(){
	suspended = 0;
	exists = true;
	notfound = true;
	db = init_db();
}
