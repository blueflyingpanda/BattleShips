#include "battleships.hpp"

using namespace std;

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
					if (decks == 3 && num > 3 && !field[(num - 4) * 10 + alpha].isEmpty){
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
					if (decks == 3 && num < 8 && !field[(num + 2) * 10 + alpha].isEmpty){
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
					if (decks == 3 && alpha > 2 && !field[(num - 1) * 10 + alpha - 3].isEmpty){
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
					if (decks == 3 && alpha < 7 && !field[(num - 1) * 10 + alpha + 3].isEmpty){
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
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
	}
	cout << "Opponent's field:" << endl;
	printField(field, true);
	return true;
}

bool oneMove(string &player, cell *field1, cell *field2, ships &fleet){
	cout << "Press any key to switch to " << PURPLE << player << RESET << endl;
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
	while (!(flag = parseMove(field2, fleet, again)) || again){
		if (!flag)
			cout << "invalid coordinate. ";
		if (again && checkFleetDestroyed(fleet))
			break;
	}
	if (checkFleetDestroyed(fleet))
		return false;
	return true;
}
