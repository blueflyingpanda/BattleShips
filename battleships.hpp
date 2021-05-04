#pragma once

#include <string>
#include <sstream>
#include <iostream>
#include <stack>
#include <algorithm>
#include <vector>
#include <numeric>
#include <map>
#include <iomanip>
#include <sqlite3.h>
#include <unistd.h>

# define YELLOW "\033[38;2;248;240;57m"
# define PURPLE "\e[1;35m"
# define RED "\e[0;31m"
# define WHITE "\e[1;37m"
# define GREEN "\e[0;32m"
# define BLUE "\e[1;34m"
# define BLACK "\e[1;30m"
# define CYAN "\e[1;36m"
# define RESET   "\033[0m"

# define COMPUTER_NAME "Computer"

enum dir{
	U,
	R,
	L,
	D,
	LR,
	UD,
	E,
};

enum param{
	N,
	P,
	H,
	K,
};

class cell{
public:
	cell() : isEmpty(true), isDiscovered(false) {}
	bool isEmpty;
	bool isDiscovered;
};

class ships{
public:
	ships() : onedecker(4), doubledecker(3), tripledecker(2), quaddecker(1) {}
	int onedecker;
	int doubledecker;
	int tripledecker;
	int quaddecker;
};

extern std::stack<cell *> ctrlz;
extern sqlite3 *db;
extern char *em;
extern bool notfound;
extern bool exists;
extern std::string pass;
extern std::map<std::string, unsigned long long> suspended;

void		askForName(std::string &name, bool second, const std::string &prev, const std::string &num);
void		AIfill(cell *field);
bool		AIMove(cell *field, ships &fleet);
bool		AIMove2(cell *field, ships &fleet);
int			callback(void *NotUsed, int argc, char **argv, char **azColName);
bool		checkFleetDestroyed(const ships &f);
bool		checkPassword(const std::string &name, std::string &password);
bool		checkParameters(int argc, char **argv);
void		createNewPassword(std::string &password);
void		createNewPassword(const std::string &name, std::string &password);
void		fill_and_check_field(cell *field);
void		fillAll(std::string &player1, std::string &player2, cell *field1, cell *field2);
std::string fillName(bool second, const std::string &prev);
std::pair<int, int> findDir(int num, int alpha, cell *field);
int			getKey(void *NotUsed, int argc, char **argv, char **azColName);
int			getPassword(void *NotUsed, int argc, char **argv, char **azColName);
std::string	hashFoo(const std::string &password);
void		initGlobalVars();
bool 		isDead(cell *field, ships &fleet, int num, int alpha, bool &again);
bool		oneMove(std::string &player, cell *field1, cell *field2, ships &fleet);
bool 		placeDeck(int num, int alpha, cell *field, bool checker);
void 		placeDeck_unsafe(int num, int alpha, cell *field);
bool		plusCheck(int num, int alpha, cell *field);
void		printField(cell *field, bool hidden);
void		print_stats(bool interactive, const std::string &p1, const std::string &p2);
void 		remove_impossible_moves(ships &fleet, cell *field);
void 		sinkShip(int num, int alpha, cell *field);
void		sqlWrapper(std::string req, int (*func)(void *, int, char **, char **));
void		sqlWrapper(std::string req, int (*func)(void *, int, char **, char **), void *p);
