#include "battleships.hpp"

using namespace std;

stack<cell *> ctrlz;

string fillName(bool second){
	string name;
	string num;
	if (second)
		num = " 2nd ";
	else
		num = " 1st ";
	cout << "Enter" <<  num << "player's name: ";
	cin >> name;
	while (cin.fail())
	{
		cout << "Not a valid name! Enter" <<  num << "player's name: ";
		cin >> name;
	}
	return name;
}

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

void placeDeck_unsafe(int num, int alpha, cell *field){
	field[(num - 1) * 10 + alpha].isEmpty = false;
	ctrlz.push(&field[(num - 1) * 10 + alpha]);
}

bool crossCheck(int num, int alpha, cell *field){
	if (num != 1 && alpha != 0 && !field[(num - 2) * 10 + alpha - 1].isEmpty)
		return false;
	if (num != 10 && alpha != 9 && !field[num * 10 + alpha + 1].isEmpty)
		return false;
	if (num != 10 && alpha != 0 && !field[num * 10 + alpha - 1].isEmpty)
		return false;
	if (num != 1 && alpha != 9 && !field[(num - 2) * 10 + alpha + 1].isEmpty)
		return false;
	return true;
}

bool plusCheck(int num, int alpha, cell *field){
	if (num != 1 && !field[(num - 2) * 10 + alpha].isEmpty)
		return false;
	if (num != 10 && !field[(num) * 10 + alpha].isEmpty)
		return false;
	if (alpha != 0 && !field[(num - 1) * 10 + alpha - 1].isEmpty)
		return false;
	if (alpha != 9 && !field[(num - 1) * 10 + alpha + 1].isEmpty)
		return false;
	return true;
}

bool placeDeck(int num, int alpha, cell *field, bool checker){
	if (!field[(num - 1) * 10 + alpha].isEmpty)
		return false;
	if (!crossCheck(num, alpha, field))
		return false;
	if (!plusCheck(num, alpha, field))
		return false;
	if (!checker)
		placeDeck_unsafe(num, alpha, field);
	return true;
}

bool parseCoordinates(cell *field, string *coordinates, int decks){
	string c1 = coordinates[0];
	c1[0] = tolower(c1[0]);
	int alpha1;
	int num1;
	int alpha2;
	int num2;
	if (decks == 1)
	{
		if(c1[0] < 'a' || c1[0] > 'j')
			return false;
		alpha1 = c1[0] - 'a';
		stringstream ss(string(c1.begin() + 1, c1.end()));
		ss >> num1;
		if (ss.fail() || !ss.eof() || num1 > 10 || num1 < 1)
			return false;
		return placeDeck(num1, alpha1, field, false);
	}
	else
	{
		string c2 = coordinates[1];
		c2[0] = tolower(c2[0]);
		if(c1[0] < 'a' || c1[0] > 'j' || c2[0] < 'a' || c2[0] > 'j')
			return false;
		alpha1 = c1[0] - 'a';
		alpha2 = c2[0] - 'a';
		stringstream ss1(string(c1.begin() + 1, c1.end()));
		ss1 >> num1;
		stringstream ss2(string(c2.begin() + 1, c2.end()));
		ss2 >> num2;
		if (ss1.fail() || !ss1.eof() || num1 > 10 || num1 < 1 || ss2.fail() || !ss2.eof() || num2 > 10 || num2 < 1)
			return false;
		if (alpha1 == alpha2 && abs(num1 - num2) == decks - 1)
		{
			if (num1 < num2)
			{
				for (int i = num1; i <= num2; i++)
					if (!placeDeck(i, alpha1, field, true))
						return false;
				for (int i = num1; i <= num2; i++)
					placeDeck_unsafe(i, alpha1, field);
			}
			else
			{
				for (int i = num2; i <= num1; i++)
					if (!placeDeck(i, alpha1, field, true))
						return false;
				for (int i = num2; i <= num1; i++)
					placeDeck_unsafe(i, alpha1, field);
			}
		}
		else if (num1 == num2 && abs(alpha1 - alpha2) == decks - 1)
		{
			if (alpha1 < alpha2)
			{
				for (int i = alpha1; i <= alpha2; i++)
					if (!placeDeck(num1, i, field, true))
						return false;
				for (int i = alpha1; i <= alpha2; i++)
					placeDeck_unsafe(num1, i, field);
			}
			else
			{
				for (int i = alpha2; i <= alpha1; i++)
					if (!placeDeck(num1, i, field, true))
						return false;
				for (int i = alpha2; i <= alpha1; i++)
					placeDeck_unsafe(num1, i, field);
			}
		}
		else
			return false;
	}
	return true;
}

int fillField(cell *field, int &i){
	char answer;
	ships fleet;
	string coordinates[2];
	printField(field, false);
	for (; i < fleet.onedecker; i++)
	{
		if (!i)
			cout << "example: \"A10\" or \"c4\"" << endl;
		cout << "Place " << i + 1 << " onedecker: ";
		cin >> coordinates[0];
		while (!parseCoordinates(field, coordinates, 1) || cin.fail()){
			cout << "Invalid coordinates! Place " << i + 1 << " onedecker: ";
			cin >> coordinates[0];
		}
		printField(field, false);
	}
	for (; i < fleet.onedecker + fleet.doubledecker; i++)
	{
		if (!i)
			cout << "example: \"a9 A10\" or \"B9 A9\"" << endl;
		cout << "Place " << i + 1 - fleet.onedecker << " doubledecker: ";
		cin >> coordinates[0];
		cin >> coordinates[1];
		while (!parseCoordinates(field, coordinates, 2) || cin.fail()){
			cout << "Invalid coordinates! Place " << i + 1 - fleet.onedecker << " doubledecker: ";
			cin >> coordinates[0];
			cin >> coordinates[1];
		}
		printField(field, false);
	}
	for (; i < fleet.onedecker + fleet.doubledecker + fleet.tripledecker; i++)
	{
		if (!i)
			cout << "example: \"a8 A10\" or \"c9 A9\"" << endl;
		cout << "Place " << i + 1 - (fleet.onedecker + fleet.doubledecker) << " tripledecker: ";
		cin >> coordinates[0];
		cin >> coordinates[1];
		while (!parseCoordinates(field, coordinates, 3) || cin.fail()){
			cout << "Invalid coordinates! Place " << i + 1 - (fleet.onedecker + fleet.doubledecker) << " tripledecker: ";
			cin >> coordinates[0];
			cin >> coordinates[1];
		}
		printField(field, false);
	}
	for (; i < fleet.onedecker + fleet.doubledecker + fleet.tripledecker + fleet.quaddecker; i++)
	{
		if (!i)
			cout << "example: \"a7 A10\" or \"d9 A9\"" << endl;
		cout << "Place " << i + 1 - (fleet.onedecker + fleet.doubledecker + fleet.tripledecker) << " quaddecker: ";
		cin >> coordinates[0];
		cin >> coordinates[1];
		while (!parseCoordinates(field, coordinates, 4) || cin.fail()){
			cout << "Invalid coordinates! Place " << i + 1 - (fleet.onedecker + fleet.doubledecker + fleet.tripledecker) << " quaddecker: ";
			cin >> coordinates[0];
			cin >> coordinates[1];
		}
		printField(field, false);
	}
	cout << "Is everything correct? Print \"y\" for YES or \"n\" for NO \"l\" to UNDO last changes: ";
	cin >> answer;
	if (cin.fail() || answer == 'n')
		return 0;
	else if (answer == 'l')
		return -1;
	return 1;
}

void fill_and_check_field(cell *field){
	int flag;
	char answer;
	int cmdz[10] = {4, 3, 3, 2, 2, 2, 1, 1, 1, 1};
	int index = 0;
	while((flag = fillField(field, index)) < 1)
	{
		if (flag == 0){
			index = 0;
			for (int i = 0; i < 100; i++)
				field[i].isEmpty = true;
		}
		if (flag < 0){
			bool again = true;
			for (int i = 0; again && !ctrlz.empty() && i < 10; i++)
			{
				for(int x = 0; x < cmdz[i]; x++){
					ctrlz.top()->isEmpty = true;
					ctrlz.pop();
				}
				index--;
				printField(field, false);
				cout << "UNDO one more? Print \"y\" for YES or \"n\" for NO: ";
				cin >> answer;
				if (answer != 'y')
					again = false;
			}
		}
	}
}

bool checkFleetDestroyed(const ships &f){
	return f.onedecker == 0 && f.doubledecker == 0 && f.tripledecker == 0 && f.quaddecker == 0;
}

std::pair<int, int> findDir(int num, int alpha, cell *field){
	int decksLeft = 0;
	if (num != 1 && !field[(num - 2) * 10 + alpha].isEmpty && num != 10 && !field[(num) * 10 + alpha].isEmpty)
	{
		if (!field[(num - 2) * 10 + alpha].isDiscovered)
			decksLeft++;
		if (!field[(num) * 10 + alpha].isDiscovered)
			decksLeft++;
		if (num > 2 && !field[(num - 3) * 10 + alpha].isEmpty && !field[(num - 3) * 10 + alpha].isDiscovered)
			decksLeft++;
		if (num < 9 &&  !field[(num + 1) * 10 + alpha].isEmpty && !field[(num + 1) * 10 + alpha].isDiscovered)
			decksLeft++;
		return {dir::UD, decksLeft};
	}
	if (num != 1 && !field[(num - 2) * 10 + alpha].isEmpty)
	{
		if (!field[(num - 2) * 10 + alpha].isDiscovered)
			decksLeft++;
		if (num > 2 && field[(num - 3) * 10 + alpha].isEmpty)
			return {dir::U, decksLeft};
		if (num > 2 && !field[(num - 3) * 10 + alpha].isDiscovered)
			decksLeft++;
		if (num > 3 && field[(num - 4) * 10 + alpha].isEmpty)
			return {dir::U, decksLeft};
		if (num > 3 && !field[(num - 4) * 10 + alpha].isDiscovered)
			decksLeft++;
		return {dir::U, decksLeft};
	}
	if (num != 10 && !field[(num) * 10 + alpha].isEmpty)
	{
		if (!field[(num) * 10 + alpha].isDiscovered)
			decksLeft++;
		if (num < 9 && field[(num + 1) * 10 + alpha].isEmpty)
			return {dir::D, decksLeft};
		if(num < 9 && !field[(num + 1) * 10 + alpha].isDiscovered)
			decksLeft++;
		if (num < 8 && field[(num + 2) * 10 + alpha].isEmpty)
			return {dir::D, decksLeft};
		if(num < 8 && !field[(num + 2) * 10 + alpha].isDiscovered)
			decksLeft++;
		return {dir::D, decksLeft};
	}
	if (alpha != 0 && !field[(num - 1) * 10 + alpha - 1].isEmpty && alpha != 9 && !field[(num - 1) * 10 + alpha + 1].isEmpty)
	{
		if (!field[(num - 1) * 10 + alpha - 1].isDiscovered)
			decksLeft++;
		if (!field[(num - 1) * 10 + alpha + 1].isDiscovered)
			decksLeft++;
		if (alpha > 1 && !field[(num - 1) * 10 + alpha - 2].isEmpty && !field[(num - 1) * 10 + alpha - 2].isDiscovered)
			decksLeft++;
		if (alpha < 8 && !field[(num - 1) * 10 + alpha + 2].isEmpty && !field[(num - 1) * 10 + alpha + 2].isDiscovered)
			decksLeft++;
		return {dir::LR, decksLeft};
	}
	if (alpha != 0 && !field[(num - 1) * 10 + alpha - 1].isEmpty)
	{
		if (!field[(num - 1) * 10 + alpha - 1].isDiscovered)
			decksLeft++;
		if (alpha > 1 && field[(num - 1) * 10 + alpha - 2].isEmpty)
			return {dir::L, decksLeft};
		if (alpha > 1 && !field[(num - 1) * 10 + alpha - 2].isDiscovered)
			decksLeft++;
		if (alpha > 2 && field[(num - 1) * 10 + alpha - 3].isEmpty)
			return {dir::L, decksLeft};
		if (alpha > 2 && !field[(num - 1) * 10 + alpha - 3].isDiscovered)
			decksLeft++;
		return {dir::L, decksLeft};
	}
	if (alpha != 9 && !field[(num - 1) * 10 + alpha + 1].isEmpty)
	{
		if (!field[(num - 1) * 10 + alpha + 1].isDiscovered)
			decksLeft++;
		if (alpha < 8 && field[(num - 1) * 10 + alpha + 2].isEmpty)
			return {dir::R, decksLeft};
		if (alpha < 8 && !field[(num - 1) * 10 + alpha + 2].isDiscovered)
			decksLeft++;
		if (alpha < 7 && field[(num - 1) * 10 + alpha + 3].isEmpty)
			return {dir::R, decksLeft};
		if (alpha < 7 && !field[(num - 1) * 10 + alpha + 3].isDiscovered)
			decksLeft++;
		return {dir::R, decksLeft};
	}
	return {dir::E, decksLeft};
}

void sinkShip(int num, int alpha, cell *field)
{
	if (num != 1 && alpha != 0)
		field[(num - 2) * 10 + alpha - 1].isDiscovered = true;
	if (num != 10 && alpha != 9)
		field[num * 10 + alpha + 1].isDiscovered = true;
	if (num != 10 && alpha != 0)
		field[num * 10 + alpha - 1].isDiscovered = true;
	if (num != 1 && alpha != 9)
		field[(num - 2) * 10 + alpha + 1].isDiscovered = true;
	if (num != 1)
		field[(num - 2) * 10 + alpha].isDiscovered = true;
	if (num != 10)
		field[(num) * 10 + alpha].isDiscovered = true;
	if (alpha != 0)
		field[(num - 1) * 10 + alpha - 1].isDiscovered = true;
	if (alpha != 9)
		field[(num - 1) * 10 + alpha + 1].isDiscovered = true;
}

bool parseMove(cell *field, ships &fleet, bool &again){
	string c;
	cout << "Enter coordinate: ";
	cin >> c;
	if (cin.fail())
		return false;
	int alpha;
	int num;
	c[0] = tolower(c[0]);
	if(c[0] < 'a' || c[0] > 'j')
		return false;
	alpha = c[0] - 'a';
	stringstream ss(string(c.begin() + 1, c.end()));
	ss >> num;
	if (ss.fail() || !ss.eof() || num > 10 || num < 1 || field[(num - 1) * 10 + alpha].isDiscovered)
		return false;
	field[(num - 1) * 10 + alpha].isDiscovered = true;
	if (!field[(num - 1) * 10 + alpha].isEmpty)
	{
		cout << "YOU HIT THE TARGET!" << endl;
		again = true;
		if (fleet.onedecker && plusCheck(num, alpha, field))
		{
			fleet.onedecker--;
			sinkShip(num, alpha, field);
		}
		else
		{
			pair<int, int> d = findDir(num, alpha, field);
			if (d.first == dir::E){
				cout << "ERROR!\n";
				exit(0);
			}
			if (d.second == 0)
			{
				sinkShip(num, alpha, field);
				int decks = 2;
				bool isTrippleDecker = true;
				if (d.first == dir::UD)
				{
					sinkShip(num - 1, alpha, field);
					sinkShip(num + 1, alpha, field);
					if (num > 2 && !field[(num - 3) * 10 + alpha].isEmpty){
						sinkShip(num - 2, alpha, field);
						isTrippleDecker = false;
					}
					else if (num < 9 && !field[(num + 1) * 10 + alpha].isEmpty){
						sinkShip(num + 2, alpha, field);
						isTrippleDecker = false;
					}
					if (!isTrippleDecker)
						fleet.quaddecker--;
					else
						fleet.tripledecker--;
				}
				else if (d.first == dir::U)
				{
					sinkShip(num - 1, alpha, field);
					if (num > 2 && !field[(num - 3) * 10 + alpha].isEmpty){
						sinkShip(num - 2, alpha, field);
						decks++;
					}
					if (num > 3 && !field[(num - 4) * 10 + alpha].isEmpty){
						sinkShip(num - 3, alpha, field);
						decks++;
					}
					switch (decks)
					{
					case 2:
						fleet.doubledecker--;
						break;
					case 3:
						fleet.tripledecker--;
						break;
					case 4:
						fleet.quaddecker--;
						break;
					}
				}
				else if (d.first == dir::D)
				{
					sinkShip(num + 1, alpha, field);
					if (num < 9 && !field[(num + 1) * 10 + alpha].isEmpty){
						sinkShip(num + 2, alpha, field);
						decks++;
					}
					if (num < 8 && !field[(num + 2) * 10 + alpha].isEmpty){
						sinkShip(num + 3, alpha, field);
						decks++;
					}
					switch (decks)
					{
					case 2:
						fleet.doubledecker--;
						break;
					case 3:
						fleet.tripledecker--;
						break;
					case 4:
						fleet.quaddecker--;
						break;
					}
				}
				else if (d.first == dir::LR)
				{
					sinkShip(num, alpha - 1, field);
					sinkShip(num, alpha + 1, field);
					if (alpha > 1 && !field[(num - 1) * 10 + alpha - 2].isEmpty){
						sinkShip(num, alpha - 2, field);
						isTrippleDecker = false;
					}
					else if (alpha < 8 && !field[(num - 1) * 10 + alpha + 2].isEmpty){
						sinkShip(num, alpha + 2, field);
						isTrippleDecker = false;
					}
					if (!isTrippleDecker)
						fleet.quaddecker--;
					else
						fleet.tripledecker--;
				}
				else if (d.first == dir::L)
				{
					sinkShip(num, alpha - 1, field);
					if (alpha > 1 && !field[(num - 1) * 10 + alpha - 2].isEmpty){
						sinkShip(num, alpha - 2, field);
						decks++;
					}
					if (alpha > 2 && !field[(num - 1) * 10 + alpha - 3].isEmpty){
						sinkShip(num, alpha - 3, field);
						decks++;
					}
					switch (decks)
					{
					case 2:
						fleet.doubledecker--;
						break;
					case 3:
						fleet.tripledecker--;
						break;
					case 4:
						fleet.quaddecker--;
						break;
					}
				}
				else if (d.first == dir::R)
				{
					sinkShip(num, alpha + 1, field);
					if (alpha < 8 && !field[(num - 1) * 10 + alpha + 2].isEmpty){
						sinkShip(num, alpha + 2, field);
						decks++;
					}
					if (alpha < 7 && !field[(num - 1) * 10 + alpha + 3].isEmpty){
						sinkShip(num, alpha + 3, field);
						decks++;
					}
					switch (decks)
					{
					case 2:
						fleet.doubledecker--;
						break;
					case 3:
						fleet.tripledecker--;
						break;
					case 4:
						fleet.quaddecker--;
						break;
					}
				}
			}
		}
	}
	else{
		cout << "YOU MISSED!" << endl;
		again = false;
	}
	cout << "Opponent's field:" << endl;
	printField(field, true);
	return true;
}

int main(){
	ships	fleet1;
	ships	fleet2;
	// string	player1 = fillName(false);
	// string	player2 = fillName(true);
	cell	field1[100];
	cell	field2[100];
	fill_and_check_field(field1);
	system("clear");
	cout << "Press any key to switch to the second player" << endl;
	read(0, 0, 1);
	fill_and_check_field(field2);
	system("clear");
	cout << "The game starts!" << endl;
	while (!checkFleetDestroyed(fleet1) && !checkFleetDestroyed(fleet2))
	{
		cout << "Press any key to switch to the first player" << endl;
		read(0, 0, 1);
		system("clear");
		cout << "Press any key to start your move" << endl;
		read(0, 0, 1);
		cout << "Your field:" << endl;
		printField(field1, false);
		cout << "Opponent's field:" << endl;
		printField(field2, true);
		bool again = false;
		bool flag;
		while (!(flag = parseMove(field2, fleet2, again)) || again){
			if (!flag)
				cout << "invalid coordinate. ";
			if (again && checkFleetDestroyed(fleet2))
				break;
		}
		if (checkFleetDestroyed(fleet2))
			break;
		cout << "Press any key to switch to the second player" << endl;
		read(0, 0, 1);
		system("clear");
		cout << "Press any key to start your move" << endl;
		read(0, 0, 1);
		cout << "Your field:" << endl;
		printField(field2, false);
		cout << "Opponent's field:" << endl;
		printField(field1, true);
		while (!(flag = parseMove(field1, fleet1, again)) || again){
			if (!flag)
				cout << "invalid coordinate. ";
			if (again && checkFleetDestroyed(fleet1))
				break;
		}
		if (checkFleetDestroyed(fleet1))
			break;
	}
	if (checkFleetDestroyed(fleet1))
		cout << "second player wins! Run \"./BattleShips\" to rematch" << endl;
	else
		cout << "first player wins! Run \"./BattleShips\" to rematch" << endl;
	return 0;
}
