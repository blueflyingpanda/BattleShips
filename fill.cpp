#include "battleships.hpp"

using namespace std;

bool findNum(const string &password){
	for (int i = 0; password[i]; i++)
		if (password[i] >= '0' && password[i] <= '9')
			return true;
	return false;
}

bool findCap(const string &password){
	for (int i = 0; password[i]; i++)
		if (password[i] >= 'A' && password[i] <= 'Z')
			return true;
	return false;
}

string hashFoo(const string &password){
	string hashedPassword;
	char array[] = {'!', '#', '$', '%', '&'};
	for (int i = 0; password[i]; i++)
	{

		int temp = i%2 ? 21 : 42;
		int tmp = password[i];
		int t = tmp * temp;
		char ch = i%3 == 0 ? 'a' : i%3 == 1 ? 'A' : '!';
		char c;
		if (ch != '!')
			c = (tmp * temp) % 26 + ch;
		else
			c = array[(tmp * temp) % 5];
		hashedPassword += to_string(tmp * temp) + c;
	}
	return hashedPassword;
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

void placeDeck_unsafe(int num, int alpha, cell *field){
	field[(num - 1) * 10 + alpha].isEmpty = false;
	ctrlz.push(&field[(num - 1) * 10 + alpha]);
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

//UTILS↑

void askForName(string &name, bool second, const string &prev, const string &num){
	cout << "Enter" <<  num << "player's name: ";
	cin >> name;
	while (cin.fail() || (second && prev == name) || name.size() > 10u || name.size() < 2u)
	{
		if (second && prev == name)
			cout << "Player with such name is already in the game! Enter " <<  num << "player's name: ";
		else if (name.size() > 10)
			cout << "Name too long! Should not be longer than 10 symbols. Enter " <<  num << "player's name: ";
		else if (name.size() < 2)
			cout << "Name too short! Should not be shorter than 2 symbols. Enter " <<  num << "player's name: ";
		else
			cout << "Not a valid name! Enter" <<  num << "player's name: ";
		cin >> name;
	}
	string req = "SELECT NAME, VICTORIES, DEFEATS from PLAYERS WHERE NAME='" + name + "'";
	exists = false;
	sqlWrapper(req, callback);
}

void createNewPassword(string &password){
	cout << "Create new password: ";
	cin >> password;
	while (cin.fail() || password.size() > 16 || password.size() < 8 || !findCap(password) || !findNum(password))
	{
		if (password.size() > 16)
			cout << "Password too long! Should not be longer than 16 symbols. Create new password: ";
		else if (password.size() < 8)
			cout << "Password too short! Should not be shorter than 8 symbols. Create new password: ";
		else if (!findCap(password) || !findNum(password))
			cout << "Password should contain at least 1 capital letter and 1 number. Create new password: ";
		else
			cout << "Not a valid password! Create new password: ";
		cin >> password;
	}
}

void createNewPassword(const string &name, string &password){
	cout << "Create " <<  name << "'s password: ";
	cin >> password;
	while (cin.fail() || password.size() > 16 || password.size() < 8 || !findCap(password) || !findNum(password))
	{
		if (password.size() > 16)
			cout << "Password too long! Should not be longer than 16 symbols. Create "<<  name << "'s password: ";
		else if (password.size() < 8)
			cout << "Password too short! Should not be shorter than 8 symbols. Create "<<  name << "'s password: ";
		else if (!findCap(password) || !findNum(password))
			cout << "Password should contain at least 1 capital letter and 1 number. Create " <<  name << "'s password: ";
		else
			cout << "Not a valid password! Create "<<  name << "'s password: ";
		cin >> password;
	}
}

bool checkPassword(const string &name, string &password){
	if (suspended[name] > time(0))
	{
		cout << "0 attemps remained! Wait for a minute and try to remember your password :(" << endl;
		cout << suspended[name] - time(0) << " seconds left" << endl;
		return false;
	}
	cout << "Enter " <<  name << "'s password: ";
	cin >> password;
	string hint;
	string request = "SELECT HINT from PLAYERS WHERE NAME='" + name + "'";
	string req = "SELECT NAME, PASSWORD from PLAYERS WHERE NAME='" + name + "'";
	sqlWrapper(req, getPassword);
	sqlWrapper(request, getKey, &hint);
	for (int i = 1; hashFoo(password) != pass; i++)
	{
		if (i == 3){
			cout << "0 attemps remained! Wait for a minute and try to remember your password :(\nTry \"./BattleShips recover\" to restore the password" << endl;
			suspended[name] = time(0) + 60;
			return false;
		}
		cout << "Wrong password! " << 3 - i << " attemps remained!" << endl;
		if (i == 2 && hint != "")
			cout << "Hint: " << hint << endl;
		cout << "Enter " <<  name << "'s password: ";
		cin >> password;
	}
	return true;
}

string fillName(bool second, const string &prev){
	string name;
	string password;
	string num;
	if (second)
		num = " 2nd ";
	else
		num = " 1st ";
	askForName(name, second, prev, num);
	if (!exists)
		createNewPassword(name, password);
	else if (!checkPassword(name, password)){
		exists = true;//
		return "";
	}
	exists = true;
	stringstream ss;
	ss << "INSERT OR IGNORE INTO PLAYERS(NAME,PASSWORD,VICTORIES,DEFEATS)""VALUES('" << name << "','" << hashFoo(password) << "', 0, 0);";
	string tmp = ss.str();
	const char *request = tmp.c_str();
	sqlWrapper(request, callback);
	return name;
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

void fillAll(string &player1, string &player2, cell *field1, cell *field2){
	while ((player1 = fillName(false, "")) == "");
	while ((player2 = fillName(true, player1)) == "");
	fill_and_check_field(field1);
	system("clear");
	cout << "Press any key to switch to " << PURPLE << player2 << RESET << endl;
	read(0, 0, 1);
	fill_and_check_field(field2);
	system("clear");
	cout << "The game starts!" << endl;
}
