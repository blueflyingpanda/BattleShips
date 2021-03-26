#include "battleships.hpp"

using namespace std;

void processParameters(char *arg){
	for (int i = 0; arg[i]; i++)
		arg[i] = tolower(arg[i]);
	if (!strncmp(arg, "stats", strlen(arg)))
		print_stats(true, string(), string());
	// else if (!strncmp(arg, "delete", strlen(arg)))
	// else if (!strncmp(arg, "chname", strlen(arg)))
	// else if (!strncmp(arg, "chpass", strlen(arg)))
	// else if (!strncmp(arg, "chkey", strlen(arg)))
	// else if (!strncmp(arg, "chhint", strlen(arg)))
}

bool checkParameters(int argc, char **argv){
	if (argc > 2){
		cout << "More than one parameter" << endl;
		return true;
	}
	if (argc == 2)
	{
		processParameters(argv[1]);
		return true;
	}
	return false;
}

