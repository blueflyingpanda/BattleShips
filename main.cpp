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
	cout << PURPLE << winner << RESET << YELLOW << " wins!\n" << RESET << "\nRun:\n\"./BattleShips\" to rematch\n\"./BattleShips stats\" to see the Stats\n\"./BattleShips delete\" to delete an account\n\"./BattleShips chname\" to change account's name\n\"./BattleShips chpass\" to change account's password\n\"./BattleShips chhint\" to change account's hint (if password is forgotten)\n\"./BattleShips chkey\" to change account's key (is required to restore password)\n" << RESET << endl;
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

	fillAll(player1, player2, field1, field2);

	while (!checkFleetDestroyed(fleet1) && !checkFleetDestroyed(fleet2))
	{
		if (!oneMove(player1, field1, field2, fleet2))
			break;
		oneMove(player2, field2, field1, fleet1);
	}

	if (checkFleetDestroyed(fleet1))
		setWinnerLoser(player2, player1);
	else
		setWinnerLoser(player1, player2);
	sqlite3_close_v2(db);
	return 0;
}
