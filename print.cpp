#include "battleships.hpp"

using namespace std;

void printField(cell *field, bool hidden){
	cout << "\n # a b c d e f g h i j";
	for (size_t i = 0; i < 100; i++)
	{
		if (!(i % 10)){
			cout << endl;
			if (i != 90)
				cout << ' ' << i/10 + 1;
			else
				cout << i/10 + 1;
		}
		if (field[i].isEmpty && !field[i].isDiscovered)
			cout << BLUE << " ~" << RESET;
		else if (field[i].isEmpty && field[i].isDiscovered)
			cout << " .";
		else if (!field[i].isEmpty && field[i].isDiscovered)
			cout << RED << " X" << RESET;
		else{
			if (hidden)
				cout << BLUE << " ~" << RESET;
			else
				cout << GREEN << " O" << RESET;
		}
	}
	cout << '\n' << endl;
}

void print_stats(bool interactive, const string &p1, const string &p2){
	if (interactive)
	{
		string action;
		cout << "press: \n0 to order by id\n1 to order by names\n2 to order by victories\n3 to order by defeats\n\nprint: \nname to find\n" << endl;
		cin >> action;
		if (cin.fail())
			return;
		cout << WHITE << left << setw(11) << "\nNAME" << setw(10) << "VICTORIES" << setw(8) << "DEFEATS" << RESET << endl;
		string request = "";
		while (action != "0" && action != "1" && action != "2" && action != "3" && action.size() < 2)
			cout << "Name too short! Should not be less than 2 symbols\n\npress: \n0 to order by id, \n1 to order by names, \n2 to order by victories\n3 to order by defeats\n\nprint: \nname to find" << endl;
		if (action.size() > 1)
			request = "SELECT NAME, VICTORIES, DEFEATS from PLAYERS WHERE NAME='" + action + "'";
		else if (action == "0")
			request = "SELECT NAME, VICTORIES, DEFEATS from PLAYERS";
		else if (action == "1")
			request = "SELECT NAME, VICTORIES, DEFEATS from PLAYERS ORDER BY NAME";
		else if (action == "2")
			request = "SELECT NAME, VICTORIES, DEFEATS from PLAYERS ORDER BY VICTORIES DESC";
		else if (action == "3")
			request = "SELECT NAME, VICTORIES, DEFEATS from PLAYERS ORDER BY DEFEATS DESC";
		sqlWrapper(request, callback);
		if (notfound)
			cout << "Nothing found!" << endl;
	}
	else
	{
		cout << WHITE << left << setw(11) << "NAME" << setw(10) << "VICTORIES" << setw(8) << "DEFEATS" << RESET << endl;
		pair<const string &, const string &> p = {p1, p2};
		sqlWrapper("SELECT NAME, VICTORIES, DEFEATS from PLAYERS ORDER BY NAME", callback, &p);
	}
}
