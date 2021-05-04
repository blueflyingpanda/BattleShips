#include "battleships.hpp"

using namespace std;

void AIfill(cell *field){
	int d = 0;
	ships fleet;
	for (; d < fleet.onedecker; d++)
	{
		int pos = rand()%100;
		while(!placeDeck(pos/10, pos%10, field, false))
			pos = pos >= 99 ? 0 : pos + 1;
	}
	for (; d < fleet.onedecker + fleet.doubledecker; d++)
	{
		bool isHorizontal = rand()%2;
		int pos = rand()%100;
		if (isHorizontal)
		{
			for (int i = pos; i < pos + 2;)
			{
				if (pos % 10 == 9 || !placeDeck(i/10, i%10, field, true))
				{
					pos = pos >= 99 ? 0 : pos + 1;
					i = pos;
				}
				else
					i++;
			}
			for (int i = pos; i < pos + 2; i++)
					placeDeck_unsafe(i/10, i%10, field);
		}
		else
		{
			for (int i = pos; i < pos + 11;)
			{
				if (pos >= 90 || !placeDeck(i/10, i%10, field, true))
				{
					pos = pos >= 90 ? 0 : pos + 1;
					i = pos;
				}
				else
					i += 10;
			}
			for (int i = pos; i < pos + 11; i += 10)
					placeDeck_unsafe(i/10, i%10, field);
		}
	}
	for (; d < fleet.onedecker + fleet.doubledecker + fleet.tripledecker; d++)
	{
		bool isHorizontal = rand()%2;
		int pos = rand()%100;
		if (isHorizontal)
		{
			pos = pos % 10 == 9 ? pos + 1 : pos;
			for (int i = pos; i < pos + 3;)
			{
				if (pos % 10 == 8 || !placeDeck(i/10, i%10, field, true))
				{
					if (pos % 10 == 8)
						pos += 2;
					pos = pos >= 98 ? 0 : pos + 1;
					i = pos;
				}
				else
					i++;
			}
			for (int i = pos; i < pos + 3; i++)
				placeDeck_unsafe(i/10, i%10, field);
		}
		else
		{
			for (int i = pos; i < pos + 21;)
			{
				if (pos >= 80 || !placeDeck(i/10, i%10, field, true))
				{
					pos = pos >= 80 ? 0 : pos + 1;
					i = pos;
				}
				else
					i += 10;
			}
			for (int i = pos; i < pos + 21; i += 10)
				placeDeck_unsafe(i/10, i%10, field);
		}
	}
	for (; d < fleet.onedecker + fleet.doubledecker + fleet.tripledecker + fleet.quaddecker; d++)
	{
		bool isHorizontal = rand()%2;
		int counter = 0;
		int pos = rand()%100;
		if (isHorizontal)
		{
			if (pos % 10 == 9)
				pos++;
			else if (pos % 10 == 8)
				pos += 2;
			for (int i = pos; i < pos + 4; counter++)
			{
				if (pos % 10 == 7 || !placeDeck(i/10, i%10, field, true))
				{
					if (pos % 10 == 7)
						pos += 3;
					pos = pos >= 97 ? 0 : pos + 1;
					i = pos;
				}
				else
					i++;
				if (counter > 100)
					break;
			}
			for (int i = pos; i < pos + 4 && counter <= 100; i++)
				placeDeck_unsafe(i/10, i%10, field);
		}
		if (!isHorizontal || counter > 100)
		{
			counter = 0;
			for (int i = pos; i < pos + 31; counter++)
			{
				if (pos >= 70 || !placeDeck(i/10, i%10, field, true))
				{
					pos = pos >= 70 ? 0 : pos + 1;
					i = pos;
				}
				else
					i += 10;
				if (counter > 100)
					break;
			}
			for (int i = pos; i < pos + 31 && counter <= 100; i += 10)
				placeDeck_unsafe(i/10, i%10, field);
		}
		if (counter > 100)
		{
			if (pos % 10 == 9)
				pos++;
			else if (pos % 10 == 8)
				pos += 2;
			for (int i = pos; i < pos + 4;)
			{
				if (pos % 10 == 7 || !placeDeck(i/10, i%10, field, true))
				{
					if (pos % 10 == 7)
						pos += 3;
					pos = pos >= 97 ? 0 : pos + 1;
					i = pos;
				}
				else
					i++;
			}
			for (int i = pos; i < pos + 4; i++)
				placeDeck_unsafe(i/10, i%10, field);
		}
	}
}

bool plusCheckOne(int num, int alpha, cell *field){
	int dirs = 4;
	if (num == 1 || field[(num - 2) * 10 + alpha].isDiscovered)
		dirs--;
	if (num == 10 || field[(num) * 10 + alpha].isDiscovered)
		dirs--;
	if (alpha == 0 || field[(num - 1) * 10 + alpha - 1].isDiscovered)
		dirs--;
	if (alpha == 9 || field[(num - 1) * 10 + alpha + 1].isDiscovered)
		dirs--;
	return dirs != 0;
}

void remove_impossible_moves(ships &fleet, cell *field){
	if (!fleet.onedecker && fleet.doubledecker)
	{
		for (size_t i = 0; i < 100; i++)
		{
			if (!field[i].isDiscovered && field[i].isEmpty){
				if (!plusCheckOne(i/10 + 1, i%10, field)){
					field[i].isDiscovered = true;
				}
			}
		}
	}
	if (!fleet.onedecker && !fleet.doubledecker && fleet.tripledecker)
	{
		int lu = 0;
		int ld = 10;
		int ru = 1;
		int rd = 11;
		for(;rd < 100; lu++, ld++, ru++, rd++){
			int eight = 8;
			if (field[lu].isDiscovered)
				eight -= 2;
			else
			{
				if (lu < 10 || field[lu - 10].isDiscovered)
					eight--;
				if (!lu % 10 || field[lu - 1].isDiscovered)
					eight--;
			}
			if (field[ru].isDiscovered)
				eight -= 2;
			else
			{
				if (ru < 10 || field[ru - 10].isDiscovered)
					eight--;
				if (ru % 10 == 9 || field[ru + 1].isDiscovered)
					eight--;
			}
			if (field[ld].isDiscovered)
				eight -= 2;
			else
			{
				if (ld > 89 || field[ld + 10].isDiscovered)
					eight--;
				if (!ld % 10 || field[ld - 1].isDiscovered)
					eight--;
			}
			if (field[rd].isDiscovered)
				eight -= 2;
			else
			{
				if (rd > 89 || field[rd + 10].isDiscovered)
					eight--;
				if (rd % 10 == 9 || field[rd + 1].isDiscovered)
					eight--;
			}
			if (eight == 0)
			{
				field[lu].isDiscovered = true;
				field[ld].isDiscovered = true;
				field[ru].isDiscovered = true;
				field[rd].isDiscovered = true;
			}
		}
	}
	if (!fleet.onedecker && !fleet.doubledecker && !fleet.tripledecker)
	{
		int lu = 0;
		int lm = 10;
		int ld = 20;
		int mu = 1;
		int md = 21;
		int ru = 2;
		int rd = 22;
		int rm = 12;
		for(;rd < 100; lu++, ld++, ru++, rd++){
			int twelve = 12;
			if (field[lu].isDiscovered)
				twelve -= 2;
			else
			{
				if (lu < 10 || field[lu - 10].isDiscovered)
					twelve--;
				if (!lu % 10 || field[lu - 1].isDiscovered)
					twelve--;
			}
			if (field[mu].isDiscovered)
				twelve--;
			else if (mu < 10 || field[ru - 10].isDiscovered)
				twelve--;
			if (field[ru].isDiscovered)
				twelve -= 2;
			else
			{
				if (ru < 10 || field[ru - 10].isDiscovered)
					twelve--;
				if (ru % 10 == 9 || field[ru + 1].isDiscovered)
					twelve--;
			}
			if (field[lm].isDiscovered)
				twelve--;
			else if (!(lm % 10) || field[lm - 1].isDiscovered)
					twelve--;
			if (field[rm].isDiscovered)
				twelve--;
			else if (rm % 10 == 9 || field[rm + 1].isDiscovered)
					twelve--;
			if (field[ld].isDiscovered)
				twelve -= 2;
			else
			{
				if (ld > 89 || field[ld + 10].isDiscovered)
					twelve--;
				if (!(ld % 10) || field[ld - 1].isDiscovered)
					twelve--;
			}
			if (field[md].isDiscovered)
				twelve--;
			else if (md > 89 || field[md + 10].isDiscovered)
					twelve--;
			if (field[rd].isDiscovered)
				twelve -= 2;
			else
			{
				if (rd > 89 || field[rd + 10].isDiscovered)
					twelve--;
				if (rd % 10 == 9 || field[rd + 1].isDiscovered)
					twelve--;
			}
			if (twelve == 0)
			{
				field[lu].isDiscovered = true;
				field[ld].isDiscovered = true;
				field[ru].isDiscovered = true;
				field[rd].isDiscovered = true;
				field[lm].isDiscovered = true;
				field[mu].isDiscovered = true;
				field[rm].isDiscovered = true;
				field[md].isDiscovered = true;
			}
		}
	}
}

bool AIMove(cell *field, ships &fleet){
	bool again = false;
	static int dir_tried[4] = {0, 0, 0, 0};
 	static vector<int> hit_pos;
	int pos;
	dir d;
	if (hit_pos.empty())
	{
		pos = rand()%100;
		int tmp = rand()%2;
		while(field[pos].isDiscovered && tmp)
			pos = pos == 99 ? 0 : pos + 1;
		while(field[pos].isDiscovered && !tmp)
			pos = pos == 0 ? 99 : pos - 1;
	}
	else if (hit_pos.size() == 1)
	{
		d = static_cast<dir>(rand()%4);
		switch (d)
		{
		case U:
			if (hit_pos[0] > 9 && !field[hit_pos[0] - 10].isDiscovered)
				pos = hit_pos[0] - 10;
			else if (hit_pos[0] % 10 != 9 && !field[hit_pos[0] + 1].isDiscovered)
				pos = hit_pos[0] + 1;
			else if (hit_pos[0] < 90 && !field[hit_pos[0] + 10].isDiscovered)
				pos = hit_pos[0] + 10;
			else if (hit_pos[0] % 10 != 0 && !field[hit_pos[0] - 1].isDiscovered)
				pos = hit_pos[0] - 1;
			else
				cout << "U ERROR!\n";
			break;
		case R:
			if (hit_pos[0] % 10 != 9 && !field[hit_pos[0] + 1].isDiscovered)
				pos = hit_pos[0] + 1;
			else if (hit_pos[0] < 90 && !field[hit_pos[0] + 10].isDiscovered)
				pos = hit_pos[0] + 10;
			else if (hit_pos[0] % 10 != 0 && !field[hit_pos[0] - 1].isDiscovered)
				pos = hit_pos[0] - 1;
			else if (hit_pos[0] > 9 && !field[hit_pos[0] - 10].isDiscovered)
				pos = hit_pos[0] - 10;
			else
				cout << "R ERROR!\n";
			break;
		case D:
			if (hit_pos[0] < 90 && !field[hit_pos[0] + 10].isDiscovered)
				pos = hit_pos[0] + 10;
			else if (hit_pos[0] % 10 != 0 && !field[hit_pos[0] - 1].isDiscovered)
				pos = hit_pos[0] - 1;
			else if (hit_pos[0] > 9 && !field[hit_pos[0] - 10].isDiscovered)
				pos = hit_pos[0] - 10;
			else if (hit_pos[0] % 10 != 9 && !field[hit_pos[0] + 1].isDiscovered)
				pos = hit_pos[0] + 1;
			else
				cout << "D ERROR!\n";
			break;
		case L:
			if (hit_pos[0] % 10 != 0 && !field[hit_pos[0] - 1].isDiscovered)
				pos = hit_pos[0] - 1;
			else if (hit_pos[0] > 9 && !field[hit_pos[0] - 10].isDiscovered)
				pos = hit_pos[0] - 10;
			else if (hit_pos[0] % 10 != 9 && !field[hit_pos[0] + 1].isDiscovered)
				pos = hit_pos[0] + 1;
			else if (hit_pos[0] < 90 && !field[hit_pos[0] + 10].isDiscovered)
				pos = hit_pos[0] + 10;
			else
				cout << "L ERROR!\n";
			break;
		default:
			cout << "DEFAULT ERROR!\n";
			break;
		}
	}
	else if (hit_pos.size() == 2)
	{
		if (hit_pos[0]%10 == hit_pos[1]%10)
		{
			if (!dir_tried[U] && !dir_tried[D])
			{
				if (min(hit_pos[0], hit_pos[1]) < 10 || field[min(hit_pos[0], hit_pos[1]) - 10].isDiscovered){
					pos = max(hit_pos[0], hit_pos[1]) + 10;
					dir_tried[D] = true;
				}
				else if (max(hit_pos[0], hit_pos[1]) > 89 || field[max(hit_pos[0], hit_pos[1]) + 10].isDiscovered){
					pos = min(hit_pos[0], hit_pos[1]) - 10;
					dir_tried[U] = true;
				}
				else
				{
					bool isUp = rand()%2;
					if (isUp){
						pos = min(hit_pos[0], hit_pos[1]) - 10;
						dir_tried[U] = true;
					}
					else{
						pos = max(hit_pos[0], hit_pos[1]) + 10;
						dir_tried[D] = true;
					}
				}
			}
			else if (!dir_tried[U]){
				pos = min(hit_pos[0], hit_pos[1]) - 10;
				dir_tried[U] = true;
			}
			else if (!dir_tried[D]){
				pos = max(hit_pos[0], hit_pos[1]) + 10;
				dir_tried[D] = true;
			}
		}
		else
		{
			if (!dir_tried[L] && !dir_tried[R])
			{
				if (min(hit_pos[0]%10, hit_pos[1]%10) == 0 || field[min(hit_pos[0], hit_pos[1]) - 1].isDiscovered){
					pos = max(hit_pos[0], hit_pos[1]) + 1;
					dir_tried[R] = true;
				}
				else if (max(hit_pos[0]%10, hit_pos[1]%10) == 9 || field[max(hit_pos[0], hit_pos[1]) + 1].isDiscovered){
					pos = min(hit_pos[0], hit_pos[1]) - 1;
					dir_tried[L] = true;
				}
				else
				{
					bool isLeft = rand()%2;
					if (isLeft){
						pos = min(hit_pos[0], hit_pos[1]) - 1;
						dir_tried[L] = true;
					}
					else{
						pos = max(hit_pos[0], hit_pos[1]) + 1;
						dir_tried[R] = true;
					}
				}
			}
			else if (!dir_tried[L]){
				pos = min(hit_pos[0], hit_pos[1]) - 1;
				dir_tried[L] = true;
			}
			else if (!dir_tried[R]){
				pos = max(hit_pos[0], hit_pos[1]) + 1;
				dir_tried[R] = true;
			}
		}
	}
	else
	{
		if (hit_pos[0]%10 == hit_pos[1]%10)
		{
			if (dir_tried[U] != 2 && dir_tried[D] != 2)
			{
				if (min(hit_pos[2], min(hit_pos[0], hit_pos[1])) < 10 || field[min(hit_pos[2], min(hit_pos[0], hit_pos[1])) - 10].isDiscovered){
					pos = max(hit_pos[2], max(hit_pos[0], hit_pos[1])) + 10;
					dir_tried[D]++;
				}
				else if (max(hit_pos[2], max(hit_pos[0], hit_pos[1])) > 89 || field[max(hit_pos[2], max(hit_pos[0], hit_pos[1])) + 10].isDiscovered){
					pos = min(hit_pos[2], min(hit_pos[0], hit_pos[1])) - 10;
					dir_tried[U]++;
				}
				else
				{
					bool isUp = rand()%2;
					if (isUp){
						pos = min(hit_pos[2], min(hit_pos[0], hit_pos[1])) - 10;
						dir_tried[U]++;
					}
					else{
						pos = max(hit_pos[2], max(hit_pos[0], hit_pos[1])) + 10;
						dir_tried[D]++;
					}
				}
			}
			else if (dir_tried[U] != 2){
				pos = min(hit_pos[2], min(hit_pos[0], hit_pos[1])) - 10;
				dir_tried[U]++;
			}
			else if (dir_tried[D] != 2){
				pos = max(hit_pos[2], max(hit_pos[0], hit_pos[1])) + 10;
				dir_tried[D]++;
			}
		}
		else
		{
			if (dir_tried[L] != 2 && dir_tried[R] != 2)
			{
				if (min(hit_pos[2]%10, min(hit_pos[0]%10, hit_pos[1]%10)) == 0 || field[min(hit_pos[2], min(hit_pos[0], hit_pos[1])) - 1].isDiscovered){
					pos = max(hit_pos[2], max(hit_pos[0], hit_pos[1])) + 1;
					dir_tried[R]++;
				}
				else if (max(hit_pos[2]%10, max(hit_pos[0]%10, hit_pos[1]%10)) == 9 || field[max(hit_pos[2], max(hit_pos[0], hit_pos[1])) + 1].isDiscovered){
					pos = min(hit_pos[2], min(hit_pos[0], hit_pos[1])) - 1;
					dir_tried[L]++;
				}
				else
				{
					bool isLeft = rand()%2;
					if (isLeft){
						pos = min(hit_pos[2], min(hit_pos[0], hit_pos[1])) - 1;
						dir_tried[L]++;
					}
					else{
						pos = max(hit_pos[2], max(hit_pos[0], hit_pos[1])) + 1;
						dir_tried[R]++;
					}
				}
			}
			else if (!dir_tried[L]){
				pos = min(hit_pos[2], min(hit_pos[0], hit_pos[1])) - 1;
				dir_tried[L]++;
			}
			else if (!dir_tried[R]){
				pos = max(hit_pos[2], max(hit_pos[0], hit_pos[1])) + 1;
				dir_tried[R]++;
			}
		}
	}
	field[pos].isDiscovered = true;
	if (!field[pos].isEmpty)
	{
		if (isDead(field, fleet, pos/10 + 1, pos%10, again)){
			hit_pos.resize(0);
			bzero(&dir_tried, sizeof(dir_tried));
		}
		else
			hit_pos.push_back(pos);
	}
	remove_impossible_moves(fleet, field);
	return again;
}

///////////////////////////////////////////////////////////////////////////////////////
bool AIMove2(cell *field, ships &fleet){
	bool again = false;
	static int dir_tried2[4] = {0, 0, 0, 0};
 	static vector<int> hit_pos2;
	int pos;
	dir d;
	if (hit_pos2.empty())
	{
		pos = rand()%100;
		int tmp = rand()%2;
		while(field[pos].isDiscovered && tmp)
			pos = pos == 99 ? 0 : pos + 1;
		while(field[pos].isDiscovered && !tmp)
			pos = pos == 0 ? 99 : pos - 1;
	}
	else if (hit_pos2.size() == 1)
	{
		d = static_cast<dir>(rand()%4);
		switch (d)
		{
		case U:
			if (hit_pos2[0] > 9 && !field[hit_pos2[0] - 10].isDiscovered)
				pos = hit_pos2[0] - 10;
			else if (hit_pos2[0] % 10 != 9 && !field[hit_pos2[0] + 1].isDiscovered)
				pos = hit_pos2[0] + 1;
			else if (hit_pos2[0] < 90 && !field[hit_pos2[0] + 10].isDiscovered)
				pos = hit_pos2[0] + 10;
			else if (hit_pos2[0] % 10 != 0 && !field[hit_pos2[0] - 1].isDiscovered)
				pos = hit_pos2[0] - 1;
			else
				cout << "U ERROR!\n";
			break;
		case R:
			if (hit_pos2[0] % 10 != 9 && !field[hit_pos2[0] + 1].isDiscovered)
				pos = hit_pos2[0] + 1;
			else if (hit_pos2[0] < 90 && !field[hit_pos2[0] + 10].isDiscovered)
				pos = hit_pos2[0] + 10;
			else if (hit_pos2[0] % 10 != 0 && !field[hit_pos2[0] - 1].isDiscovered)
				pos = hit_pos2[0] - 1;
			else if (hit_pos2[0] > 9 && !field[hit_pos2[0] - 10].isDiscovered)
				pos = hit_pos2[0] - 10;
			else
				cout << "R ERROR!\n";
			break;
		case D:
			if (hit_pos2[0] < 90 && !field[hit_pos2[0] + 10].isDiscovered)
				pos = hit_pos2[0] + 10;
			else if (hit_pos2[0] % 10 != 0 && !field[hit_pos2[0] - 1].isDiscovered)
				pos = hit_pos2[0] - 1;
			else if (hit_pos2[0] > 9 && !field[hit_pos2[0] - 10].isDiscovered)
				pos = hit_pos2[0] - 10;
			else if (hit_pos2[0] % 10 != 9 && !field[hit_pos2[0] + 1].isDiscovered)
				pos = hit_pos2[0] + 1;
			else
				cout << "D ERROR!\n";
			break;
		case L:
			if (hit_pos2[0] % 10 != 0 && !field[hit_pos2[0] - 1].isDiscovered)
				pos = hit_pos2[0] - 1;
			else if (hit_pos2[0] > 9 && !field[hit_pos2[0] - 10].isDiscovered)
				pos = hit_pos2[0] - 10;
			else if (hit_pos2[0] % 10 != 9 && !field[hit_pos2[0] + 1].isDiscovered)
				pos = hit_pos2[0] + 1;
			else if (hit_pos2[0] < 90 && !field[hit_pos2[0] + 10].isDiscovered)
				pos = hit_pos2[0] + 10;
			else
				cout << "L ERROR!\n";
			break;
		default:
			cout << "DEFAULT ERROR!\n";
			break;
		}
	}
	else if (hit_pos2.size() == 2)
	{
		if (hit_pos2[0]%10 == hit_pos2[1]%10)
		{
			if (!dir_tried2[U] && !dir_tried2[D])
			{
				if (min(hit_pos2[0], hit_pos2[1]) < 10 || field[min(hit_pos2[0], hit_pos2[1]) - 10].isDiscovered){
					pos = max(hit_pos2[0], hit_pos2[1]) + 10;
					dir_tried2[D] = true;
				}
				else if (max(hit_pos2[0], hit_pos2[1]) > 89 || field[max(hit_pos2[0], hit_pos2[1]) + 10].isDiscovered){
					pos = min(hit_pos2[0], hit_pos2[1]) - 10;
					dir_tried2[U] = true;
				}
				else
				{
					bool isUp = rand()%2;
					if (isUp){
						pos = min(hit_pos2[0], hit_pos2[1]) - 10;
						dir_tried2[U] = true;
					}
					else{
						pos = max(hit_pos2[0], hit_pos2[1]) + 10;
						dir_tried2[D] = true;
					}
				}
			}
			else if (!dir_tried2[U]){
				pos = min(hit_pos2[0], hit_pos2[1]) - 10;
				dir_tried2[U] = true;
			}
			else if (!dir_tried2[D]){
				pos = max(hit_pos2[0], hit_pos2[1]) + 10;
				dir_tried2[D] = true;
			}
		}
		else
		{
			if (!dir_tried2[L] && !dir_tried2[R])
			{
				if (min(hit_pos2[0]%10, hit_pos2[1]%10) == 0 || field[min(hit_pos2[0], hit_pos2[1]) - 1].isDiscovered){
					pos = max(hit_pos2[0], hit_pos2[1]) + 1;
					dir_tried2[R] = true;
				}
				else if (max(hit_pos2[0]%10, hit_pos2[1]%10) == 9 || field[max(hit_pos2[0], hit_pos2[1]) + 1].isDiscovered){
					pos = min(hit_pos2[0], hit_pos2[1]) - 1;
					dir_tried2[L] = true;
				}
				else
				{
					bool isLeft = rand()%2;
					if (isLeft){
						pos = min(hit_pos2[0], hit_pos2[1]) - 1;
						dir_tried2[L] = true;
					}
					else{
						pos = max(hit_pos2[0], hit_pos2[1]) + 1;
						dir_tried2[R] = true;
					}
				}
			}
			else if (!dir_tried2[L]){
				pos = min(hit_pos2[0], hit_pos2[1]) - 1;
				dir_tried2[L] = true;
			}
			else if (!dir_tried2[R]){
				pos = max(hit_pos2[0], hit_pos2[1]) + 1;
				dir_tried2[R] = true;
			}
		}
	}
	else
	{
		if (hit_pos2[0]%10 == hit_pos2[1]%10)
		{
			if (dir_tried2[U] != 2 && dir_tried2[D] != 2)
			{
				if (min(hit_pos2[2], min(hit_pos2[0], hit_pos2[1])) < 10 || field[min(hit_pos2[2], min(hit_pos2[0], hit_pos2[1])) - 10].isDiscovered){
					pos = max(hit_pos2[2], max(hit_pos2[0], hit_pos2[1])) + 10;
					dir_tried2[D]++;
				}
				else if (max(hit_pos2[2], max(hit_pos2[0], hit_pos2[1])) > 89 || field[max(hit_pos2[2], max(hit_pos2[0], hit_pos2[1])) + 10].isDiscovered){
					pos = min(hit_pos2[2], min(hit_pos2[0], hit_pos2[1])) - 10;
					dir_tried2[U]++;
				}
				else
				{
					bool isUp = rand()%2;
					if (isUp){
						pos = min(hit_pos2[2], min(hit_pos2[0], hit_pos2[1])) - 10;
						dir_tried2[U]++;
					}
					else{
						pos = max(hit_pos2[2], max(hit_pos2[0], hit_pos2[1])) + 10;
						dir_tried2[D]++;
					}
				}
			}
			else if (dir_tried2[U] != 2){
				pos = min(hit_pos2[2], min(hit_pos2[0], hit_pos2[1])) - 10;
				dir_tried2[U]++;
			}
			else if (dir_tried2[D] != 2){
				pos = max(hit_pos2[2], max(hit_pos2[0], hit_pos2[1])) + 10;
				dir_tried2[D]++;
			}
		}
		else
		{
			if (dir_tried2[L] != 2 && dir_tried2[R] != 2)
			{
				if (min(hit_pos2[2]%10, min(hit_pos2[0]%10, hit_pos2[1]%10)) == 0 || field[min(hit_pos2[2], min(hit_pos2[0], hit_pos2[1])) - 1].isDiscovered){
					pos = max(hit_pos2[2], max(hit_pos2[0], hit_pos2[1])) + 1;
					dir_tried2[R]++;
				}
				else if (max(hit_pos2[2]%10, max(hit_pos2[0]%10, hit_pos2[1]%10)) == 9 || field[max(hit_pos2[2], max(hit_pos2[0], hit_pos2[1])) + 1].isDiscovered){
					pos = min(hit_pos2[2], min(hit_pos2[0], hit_pos2[1])) - 1;
					dir_tried2[L]++;
				}
				else
				{
					bool isLeft = rand()%2;
					if (isLeft){
						pos = min(hit_pos2[2], min(hit_pos2[0], hit_pos2[1])) - 1;
						dir_tried2[L]++;
					}
					else{
						pos = max(hit_pos2[2], max(hit_pos2[0], hit_pos2[1])) + 1;
						dir_tried2[R]++;
					}
				}
			}
			else if (!dir_tried2[L]){
				pos = min(hit_pos2[2], min(hit_pos2[0], hit_pos2[1])) - 1;
				dir_tried2[L]++;
			}
			else if (!dir_tried2[R]){
				pos = max(hit_pos2[2], max(hit_pos2[0], hit_pos2[1])) + 1;
				dir_tried2[R]++;
			}
		}
	}
	field[pos].isDiscovered = true;
	if (!field[pos].isEmpty)
	{
		if (isDead(field, fleet, pos/10 + 1, pos%10, again)){
			hit_pos2.resize(0);
			bzero(&dir_tried2, sizeof(dir_tried2));
		}
		else
			hit_pos2.push_back(pos);
	}
	remove_impossible_moves(fleet, field);
	return again;
}
