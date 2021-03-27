#include "battleships.hpp"

using namespace std;

void deleteAccount(){
	string name;
	string password;
	askForName(name, false, "", "");
	if (!exists){
		cout << "Name " << name << " does not exist!" << endl;
		return;
	}
	else
	{
		if (!checkPassword(name, password))
			return;
	}
	exists = true;
	string req = "DELETE FROM PLAYERs WHERE NAME='" + name + "'";
	sqlWrapper(req, callback);
	cout << name << " was deleted succesfully!" << endl;
}

bool checkNameAlreadyInDB(const string &name){
	string req = "SELECT NAME, VICTORIES, DEFEATS from PLAYERS WHERE NAME='" + name + "'";
	exists = false;
	sqlWrapper(req, callback);
	if (!exists)
	{
		exists = true;
		return false;
	}
	return true;
}

void changeAttrribute(int flag){
	string name;
	string password;
	string temp;
	string request;
	askForName(name, false, "", "");
	if (!exists){
		cout << "Name " << name << " does not exist!" << endl;
		return;
	}
	else
	{
		if (!checkPassword(name, password)){
			return;
			exists = true;//
		}
		if (flag == param::N)
		{
			cout << "Enter new name: ";
			cin >> temp;
			bool f;
			while (cin.fail() || temp.size() > 10u || temp.size() < 2u || (f = checkNameAlreadyInDB(temp)) == true)
			{
				if (f)
					cout << "Player with such name is already in the game! Enter new name: ";
				else if (temp.size() > 10)
					cout << "Name too long! Should not be longer than 10 symbols. Enter new name: ";
				else if (temp.size() < 2)
					cout << "Name too short! Should not be shorter than 2 symbols. Enter new name: ";
				else
					cout << "Not a valid name! Enter new name: ";
				cin >> temp;
			}
			request = "UPDATE PLAYERS SET NAME='" + temp + "' WHERE NAME='" + name + "';";
			sqlWrapper(request, callback);
			cout << "Name changed succesfully!" << endl;
		}
		else if (flag == param::P)
		{
			createNewPassword(temp);
			request = "UPDATE PLAYERS SET PASSWORD='" + hashFoo(temp) + "' WHERE NAME='" + name + "';";
			sqlWrapper(request, callback);
			cout << "Password changed succesfully!" << endl;
		}
		else if (flag == param::K)
		{
			string key;
			request = "SELECT KEY FROM PLAYERS WHERE NAME='" + name + "';";
			sqlWrapper(request, getKey, &key);
			if (key != ""){
				cout << "Enter old key: ";
				cin >> temp;
				if (hashFoo(temp) != key){
					cout << "Incorrect key!" << endl;
					return;
				}
			}
			cout << "Enter new key: ";
			cin >> temp;
			while (cin.fail() || temp.size() > 10u || temp.size() < 2u)
			{
				if (temp.size() > 10)
					cout << "key too long! Should not be longer than 10 symbols. Enter new key: ";
				else if (temp.size() < 2)
					cout << "key too short! Should not be shorter than 2 symbols. Enter new key: ";
				else
					cout << "Not a valid key! Enter new key: ";
				cin >> temp;
			}
			request = "UPDATE PLAYERS SET KEY='" + hashFoo(temp) + "' WHERE NAME='" + name + "';";
			sqlWrapper(request, callback);
			system("clear");
			cout << "Key changed succesfully!" << endl;
		}
		else if (flag == param::H)
		{
			cout << "Enter new hint: ";
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			getline(cin, temp);
			while (cin.fail() || temp.size() > 20u || temp.size() < 2u)
			{
				if (temp.size() > 20)
					cout << "Hint too long! Should not be longer than 20 symbols. Enter new Hint: ";
				else if (temp.size() < 2)
					cout << "Hint too short! Should not be shorter than 2 symbols. Enter new Hint: ";
				else
					cout << "Not a valid Hint! Enter new Hint: ";
				getline(cin, temp);
			}
			request = "UPDATE PLAYERS SET HINT='" + temp + "' WHERE NAME='" + name + "';";
			sqlWrapper(request, callback);
			system("clear");
			cout << "Hint changed succesfully!" << endl;
		}
	}
	exists = true;
}

void recoverPassword(){
	string name;
	string password;
	string temp;
	string request;
	askForName(name, false, "", "");
	if (!exists){
		cout << "Name " << name << " does not exist!" << endl;
		return;
	}
	string key;
	request = "SELECT KEY FROM PLAYERS WHERE NAME='" + name + "';";
	sqlWrapper(request, getKey, &key);
	if (key == ""){
		cout << "You don't have a key. Recovery is impossible!" << endl;
		return;
	}
	cout << "Enter keyword: ";
	cin >> temp;
	if (hashFoo(temp) != key)
	{
		cout << "Incorrect key!" << endl;
		return;
	}
	for (int i = 0; password.size() < 8; i++)
	{
		if (!name[i])
			i = 0;
		int tmp = name[i];
		password += to_string(tmp);
	}
	request = "UPDATE PLAYERS SET PASSWORD='" + hashFoo(password) + "' WHERE NAME='" + name + "';";
	sqlWrapper(request, callback);
	cout << "Your new password is: " << password << endl;
	cout << "Press any key to exit" << endl;
	read(0,0,1);
	system("clear");
}

void processParameters(char *arg){
	for (int i = 0; arg[i]; i++)
		arg[i] = tolower(arg[i]);
	if (!strncmp(arg, "stats", strlen(arg)))
		print_stats(true, string(), string());
	else if (!strncmp(arg, "delete", strlen(arg)))
		deleteAccount();
	else if (!strncmp(arg, "chname", strlen(arg)))
		changeAttrribute(N);
	else if (!strncmp(arg, "chpass", strlen(arg)))
		changeAttrribute(P);
	else if (!strncmp(arg, "chkey", strlen(arg)))
		changeAttrribute(K);
	else if (!strncmp(arg, "chhint", strlen(arg)))
		changeAttrribute(H);
	else if (!strncmp(arg, "recover", strlen(arg)))
		recoverPassword();
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

