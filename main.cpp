#include "battleships.hpp"

using namespace std;

bool checkFleetDestroyed(const ships &f){
	return f.onedecker == 0 && f.doubledecker == 0 && f.tripledecker == 0 && f.quaddecker == 0;
}

void setWinnerLoser(const string &winner, const string &loser){
	string tmp = "UPDATE PLAYERS SET VICTORIES=VICTORIES+1 WHERE NAME='" + winner +"';";
	sqlWrapper(tmp, callback);
	tmp = "UPDATE PLAYERS SET DEFEATS=DEFEATS+1 WHERE NAME='" + loser +"';";
	sqlWrapper(tmp, callback);
	cout << PURPLE << winner << RESET << YELLOW << " wins!\n" << RESET << "\nRun:\n\"./BattleShips\" to rematch\n\"./BattleShips stats\" to see the Stats\n\"./BattleShips delete\" to delete an account\n\"./BattleShips chname\" to change account's name\n\"./BattleShips chpass\" to change account's password\n\"./BattleShips chhint\" to change account's hint (if password is forgotten)\n\"./BattleShips chkey\" to change account's key (is required to restore password)\n\"./BattleShips recover\" to restore the password (if key is provided)\n" << RESET << endl;
	print_stats(false, winner, loser);
}

int main(int argc, char **argv){
	initGlobalVars();
	if (checkParameters(argc, argv))
		return 0;

	ships	fleet1;
	ships	fleet2;
	string	player1;
	string	player2;
	cell	field1[100];
	cell	field2[100];

	cout << "press 1 for pvp OR press 0 for pve: ";
	bool isPVP = true;
	cin >> isPVP;
	if (isPVP){
		fillAll(player1, player2, field1, field2);
		while (!checkFleetDestroyed(fleet1) && !checkFleetDestroyed(fleet2))
		{
			if (!oneMove(player1, field1, field2, fleet2))
				break;
			oneMove(player2, field2, field1, fleet1);
		}
	}
	else{
		player2 = COMPUTER_NAME;
		AIfill(field2);
		while ((player1 = fillName(false, "")) == "");
		fill_and_check_field(field1);
		cout << "The game starts!" << endl;
		////////////////////////////////////
		while (!checkFleetDestroyed(fleet1) && !checkFleetDestroyed(fleet2))
		{
			if (!oneMove(player1, field1, field2, fleet2))
				break;
			while(!checkFleetDestroyed(fleet1) && AIMove(field1, fleet1));
			cout << PURPLE << COMPUTER_NAME << RESET << " made a move!" << endl;
		}
		////////////////////////////////////
		/*TWO COMPUTERS ARE PLAYING!*/
		/**/
		/*
		while (!checkFleetDestroyed(fleet1) && !checkFleetDestroyed(fleet2))
		{
			while(!checkFleetDestroyed(fleet2) && AIMove(field2, fleet2));
			cout << PURPLE << player1 << RESET << " made a move taking advice from " << PURPLE << player2 << RESET << "!" << endl;
			printField(field2, false);
			cout << fleet2.onedecker << ' ' << fleet2.doubledecker << ' ' << fleet2.tripledecker << ' ' << fleet2.quaddecker << endl;
			if (checkFleetDestroyed(fleet2))
				break;
			while(!checkFleetDestroyed(fleet1) && AIMove2(field1, fleet1));
			cout << PURPLE << player2 << RESET << " made a move!" << endl;
			printField(field1, false);
			cout << fleet1.onedecker << ' ' << fleet1.doubledecker << ' ' << fleet1.tripledecker << ' ' << fleet1.quaddecker << endl;
			// read(0,0,1);
		}
		*/
		/**/
	}
	if (checkFleetDestroyed(fleet1))
		setWinnerLoser(player2, player1);
	else
		setWinnerLoser(player1, player2);
	sqlite3_close_v2(db);
	return 0;
}
