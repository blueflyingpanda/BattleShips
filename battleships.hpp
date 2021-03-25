#pragma once

#include <string>
#include <sstream>
#include <iostream>
#include <stack>
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
