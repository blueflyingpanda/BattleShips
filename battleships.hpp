#pragma once

#include <string>
#include <sstream>
#include <iostream>
#include <stack>
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

enum dir{
	U,
	R,
	L,
	D,
	LR,
	UD,
	E,
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
extern unsigned long long suspended;

int	callback(void *NotUsed, int argc, char **argv, char **azColName);
bool		checkFleetDestroyed(const ships &f);
bool		checkParameters(int argc, char **argv);
void		fillAll(std::string &player1, std::string &player2, cell *field1, cell *field2);
int	getPassword(void *NotUsed, int argc, char **argv, char **azColName);
void		initGlobalVars();
bool		oneMove(std::string &player, cell *field1, cell *field2, ships &fleet);
bool		plusCheck(int num, int alpha, cell *field);
void		printField(cell *field, bool hidden);
void		print_stats(bool interactive, const std::string &p1, const std::string &p2);
void		sqlWrapper(std::string req, int (*func)(void *, int, char **, char **));
void sqlWrapper(std::string req, int (*func)(void *, int, char **, char **), void *p);
