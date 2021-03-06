// connect4.cpp : Defines the entry point for the console application.
//
//Zhang Song 

//Connect 4 Console C++
#include "stdafx.h"
#include "PPNS.h"
#include "PNS.h"
#include "MCPNS.h"
#include "UCT.h"
#include <fstream>
#include <time.h>
#include "FileOperator.h"

using namespace std;


playerInfo playerOne, playerTwo;
playerInfo* playerAI;
playerInfo* playerHuman;
char board[BoardLength][BoardWidth];
bool solved = false;
Node r;
Node* curNode = NULL;
int lastMove = 0;
int nodeNum = 1;

int main()
{
	cout <<"Node size: " <<sizeof(Node) << endl;

	int dropChoice, win, full, again, initial_board,nBoard;

	curNode = &r;
	cout << "Solving Connect 4 Boards" << endl << endl;
	/*srand(time(NULL));
	generateRandomOpenings(400);*/
	//cout << "Choose Algorithm (pns/ppns/mcpns/mcts): ";
	//cin >> playerOne.playerName;
	playerOne.playerID = 'X';
	//cout << "Player Two please enter your name: ";
	//cin >> playerTwo.playerName;
	playerTwo.playerID = 'O';
	cout << "id of initial board : ";
	cin >> initial_board;
	cout << "amount of board: ";
	cin >> nBoard;

	//board loading
	//

	//initialization
	full = 0;
	win = 0;
	again = 0;
	//DisplayBoard(board);
	srand(time(NULL));
	int count, board_id;

	//pns batch test

	for (count = 0, board_id = initial_board; count <= nBoard; count++, board_id++) {
		//
		////PNS
		//LoadBoard(board, board_id);
		////DisplayBoard(board);
		//std::string resultpndn;
		//resultpndn.append("id");
		//resultpndn.append(std::to_string(board_id));
		//resultpndn.append(";\n");
		//writeIntoFile("PNSTPNDN.txt", resultpndn);
		//std::string resultpns;
		//resultpns.append("id");
		//resultpns.append(std::to_string(board_id));
		//writeIntoFile("PNSTresult.txt", resultpns);
		//cout << board_id << endl;
		//PNS pns;
		//nodeNum = 1;
		//playerAI = &playerOne;
		//playerHuman = &playerTwo;
		//int res = pns.startSolver();
		//RestartWithoutConfirmation(board);
		//srand(time(NULL));

		//MCPNS
		//LoadBoard(board, board_id);
		////DisplayBoard(board);
		//std::string resultpndn;
		//resultpndn.append("id");
		//resultpndn.append(std::to_string(board_id));
		//resultpndn.append(";\n");
		//writeIntoFile("MCPNSTPNDN.txt", resultpndn);
		//std::string resultmcpns;
		//resultmcpns.append("id");
		//resultmcpns.append(std::to_string(board_id));
		//writeIntoFile("MCPNSresult.txt", resultmcpns);
		//MCPNS mcpns;
		//nodeNum = 1;
		//playerAI = &playerOne;
		//playerHuman = &playerTwo;
		//int res = mcpns.startSolver();
		//RestartWithoutConfirmation(board);
		//srand(time(NULL));

		/*PPNS*/
		LoadBoard(board, board_id);
		//DisplayBoard(board);
		std::string resultpndn;
		resultpndn.append("id");
		resultpndn.append(std::to_string(board_id));
		resultpndn.append(";\n");
		writeIntoFile("PPNSTPNDN3.txt", resultpndn);
		std::string resultppns;
		resultppns.append("id");
		resultppns.append(std::to_string(board_id));
		writeIntoFile("PPNSTresult3.txt", resultppns);
		PPNS ppns;
		nodeNum = 1;
		playerAI = &playerOne;
		playerHuman = &playerTwo;
		int res = ppns.startSolver();
		RestartWithoutConfirmation(board);
		srand(time(NULL));

		////MCTS
		//LoadBoard(board, board_id);
		////DisplayBoard(board);
		//std::string resultmcts;
		//resultmcts.append("id");
		//resultmcts.append(std::to_string(board_id));
		//writeIntoFile("MCTSresult.txt", resultmcts);
		//UCT mcts(0);
		//nodeNum = 1;
		//playerAI = &playerOne;
		//playerHuman = &playerTwo;
		//res = mcts.startSolver();
		//RestartWithoutConfirmation(board);
		//srand(time(NULL));

		////UCT
		//LoadBoard(board, board_id);
		////DisplayBoard(board);
		//std::string resultuct;
		//resultuct.append("id");
		//resultuct.append(std::to_string(board_id));
		//writeIntoFile("UCTresult.txt", resultuct);
		//UCT uct(0.7);
		//nodeNum = 1;
		//playerAI = &playerOne;
		//playerHuman = &playerTwo;
		//res = uct.startSolver();
		//RestartWithoutConfirmation(board);
		//srand(time(NULL));
	}
	//PNS

	/*if (strcmp(playerOne.playerName, "pns")==0) {
		for (count = 0, board_id = initial_board + 1; count <= nBoard; count++, ++board_id) {
			cout << board_id << endl;
			PNS pns;
			nodeNum = 1;
			playerAI = &playerOne;
			playerHuman = &playerTwo;
			int res = pns.startSolver();
			RestartWithoutConfirmation(board);
			LoadBoard(board, board_id);
			DisplayBoard(board);
			srand(time(NULL));
		}
	}else if (strcmp(playerOne.playerName, "mcpns")==0) {
		for (count = 0, board_id = initial_board + 1; count <= nBoard; count++, ++board_id) {
			cout << board_id << endl;
			MCPNS mcpns;
			nodeNum = 1;
			playerAI = &playerOne;
			playerHuman = &playerTwo;
			int res = mcpns.startSolver();
			RestartWithoutConfirmation(board);
			LoadBoard(board, board_id);
			DisplayBoard(board);
			srand(time(NULL));
		}
	}else if (strcmp(playerOne.playerName, "ppns")==0) {
		for (count = 0, board_id = initial_board + 1; count <= nBoard; count++, ++board_id) {
			cout << board_id << endl;
			PPNS pns;
			nodeNum = 1;
			playerAI = &playerOne;
			playerHuman = &playerTwo;
			int res = pns.startSolver();
			RestartWithoutConfirmation(board);
			LoadBoard(board, board_id);
			DisplayBoard(board);
			srand(time(NULL));
		}
	}
	else if (strcmp(playerOne.playerName, "mcts") == 0) {
		initial_board++;
		count = 0;
		//#pragma omp parallel for
		for (board_id = initial_board; count <= nBoard; board_id++) {
			//char board_in[BoardLength][BoardWidth];
			LoadBoard(board, board_id);
			std::string result;
			result.append("xx");
			result.append(std::to_string(board_id-1));
			writeIntoFile("UCTresult_n.txt", result);
			cout << board_id << endl;
			UCT pns(0.7);
			nodeNum = 1;
			playerAI = &playerOne;
			playerHuman = &playerTwo;
			int res = pns.startSolver();
			RestartWithoutConfirmation(board);
			DisplayBoard(board);
			srand(time(NULL));
			count++;
		}
	}
	else {
		//nothing to do
		cout << "end program";
		getchar();
	}*/
	
	return 0;
}

int LoadBoard(char board[][BoardWidth], int openings_id) {
	//opening from file
	ifstream board_openings("openings.txt");

	if (!board_openings) {
		cout << "Error opening input file" << endl;
		return -1;
	}
	int board_no = 0;
	int count_row = 0;
	int count_col = 0;
	char move;
	for (int i = 0; !board_openings.eof(); i++) {
		board_openings.get(move);
		if (board_no == openings_id) {
			board[count_row][count_col] = move;
			count_col++;
			if (count_row == 6) break;
			if (count_col == 7) {
				count_row++;
				count_col = 0;
			}
		}
		if (move == '\n') board_no++;
	}

	board_openings.close();
}

int PlayerDrop(char board[][BoardWidth], playerInfo& activePlayer)
{
	int dropChoice;
	do
	{
		cout << activePlayer.playerName << "'s Turn ";
		cout << "Please enter a number between 1 and 7: ";
		cin >> dropChoice;

		while (board[0][dropChoice - 1] == 'X' || board[0][dropChoice - 1] == 'O')
		{
			cout << "That row is full, please enter a new row: ";
			cin >> dropChoice;
		}
	} while (dropChoice < 1 || dropChoice > BoardWidth);

	return dropChoice - 1;
}



void CheckBellow(char board[][BoardWidth], playerInfo& activePlayer, int dropChoice)
{
	int length, turn;
	length = BoardLength - 1;
	do
	{
		if (board[length][dropChoice] != 'X' && board[length][dropChoice] != 'O')
		{
			board[length][dropChoice] = activePlayer.playerID;
			break;
		}
		else
			--length;
	} while (1);
}



void DisplayBoard(char board[][BoardWidth])
{
	int rows = BoardLength, columns = BoardWidth, i, ix;
	for (i = 0; i < rows; i++)
	{
		cout << "| ";
		for (ix = 0; ix < columns; ix++)
		{
			if (board[i][ix] != 'X' && board[i][ix] != 'O')
				board[i][ix] = '*';
			cout << board[i][ix]<<" ";
		}
		cout << "|" << endl;
	}
}



int CheckFour(char board[][BoardWidth], playerInfo& activePlayer)

{
	char XO;
	int win;

	XO = activePlayer.playerID;
	win = 0;

	for (int i = BoardLength - 1; i >= 0; --i)
	{
		for (int ix = BoardWidth - 1; ix >= 0; --ix)
		{
			if (board[i][ix] == XO &&
				i >= 3 &&
				ix >= 3 &&
				board[i - 1][ix - 1] == XO &&
				board[i - 2][ix - 2] == XO &&
				board[i - 3][ix - 3] == XO)
			{
				win = 1;
			}

			if (board[i][ix] == XO &&
				ix >= 3 &&
				board[i][ix - 1] == XO &&
				board[i][ix - 2] == XO &&
				board[i][ix - 3] == XO)
			{
				win = 1;
			}

			if (board[i][ix] == XO &&
				i >= 3 &&
				board[i - 1][ix] == XO &&
				board[i - 2][ix] == XO &&
				board[i - 3][ix] == XO)
			{
				win = 1;
			}

			if (board[i][ix] == XO &&
				i >= 3 &&
				ix + 3 < BoardWidth &&
				board[i - 1][ix + 1] == XO &&
				board[i - 2][ix + 2] == XO &&
				board[i - 3][ix + 3] == XO)
			{
				win = 1;
			}

			if (board[i][ix] == XO &&
				ix + 3 < BoardWidth &&
				board[i][ix + 1] == XO &&
				board[i][ix + 2] == XO &&
				board[i][ix + 3] == XO)
			{
				win = 1;
			}
		}
	}
	return win;
}

int FullBoard(char board[][BoardWidth])
{
	int full;
	full = 0;

	for (int i = 0; i < BoardWidth; ++i)
	{
		if (board[0][i] != '*')
			++full;
	}
	return full;
}

void PlayerWin(playerInfo& activePlayer)
{
	cout << endl << activePlayer.playerName <<" "<< activePlayer .playerID << " Connected Four, You Win!" << endl;
}

int restart(char board[][BoardWidth])
{
	int restart;
	cout << "Would you like to restart? Yes(1) No(2): ";
	cin >> restart;

	solved = false;
	r.child.clear();
	r.index = 0;
	r.pn = 0;
	r.dn = 0;
	nodeNum = 1;
	uctr.pn = 0;
	uctr.dn = 0;
	uctr.index = 0;
	uctr.visit = 0;
	uctr.child.clear();
	uctr.action.clear();
	

	if (restart == 1)
	{
		for (int i = 0; i < BoardLength; i++)
		{
			for (int ix = 0; ix < BoardWidth; ix++)
			{
				board[i][ix] = '*';
			}
		}
	}
	else
		cout << "Goodbye!" << endl;
	return restart;
}
int RestartWithoutConfirmation(char board[][BoardWidth]) {
	

	solved = false;
	r.child.clear();
	r.index = 0;
	r.pn = 0;
	r.dn = 0;
	nodeNum = 1;
	uctr.pn = 0;
	uctr.dn = 0;
	uctr.index = 0;
	uctr.visit = 0;
	uctr.child.clear();
	uctr.action.clear();


	
		for (int i = 0; i < BoardLength; i++)
		{
			for (int ix = 0; ix < BoardWidth; ix++)
			{
				board[i][ix] = '*';
			}
		}
	
	return 1;
}

bool setMove(int index, playerInfo& activePlayer, char tempboard[][BoardWidth])
{
	int length, turn;
	length = BoardLength - 1;
	do
	{
		if (tempboard[length][index] != 'X' && tempboard[length][index] != 'O')
		{
			tempboard[length][index] = activePlayer.playerID;
			return true;
		}
		else
			--length;
	} while (length >=0);
}

playerInfo* changeSide(playerInfo* activePlayer)
{
	if (activePlayer == playerAI)
		return playerHuman;
	else
		return playerAI;
}

int findThreat(playerInfo & activePlayer, char tempboard[][BoardWidth])
{
	for (int i = 0; i < BoardWidth; i++)
	{
		if (tempboard[0][i] != '*') continue;
		setMove(i, *changeSide(&activePlayer), tempboard);
		if (CheckFour(board, *changeSide(&activePlayer)) == 1)
		{
			delMove(i, *changeSide(&activePlayer), tempboard);
			return i;
		}
		delMove(i, *changeSide(&activePlayer), tempboard);
	}
	return -1;
}

void delMove(int index, playerInfo& activePlayer, char tempboard[][BoardWidth])
{
	int length, turn;
	length = 0;
	turn = 0;

	do
	{
		if (tempboard[length][index] == 'X' || tempboard[length][index] == 'O')
		{
			tempboard[length][index] = '*';
			turn = 1;
		}
		else
			length++;

	} while (turn != 1);
}

bool simulate(playerInfo& activePlayer)
{
	playerInfo* player = &activePlayer;
	char tempboard[BoardLength][BoardWidth];
	memcpy(tempboard, board, BoardLength*BoardWidth * sizeof(char));
	while (1)
	{
		/*int i = findThreat(*player, tempboard);
		if( i == -1)*/
		int	i = rand() % BoardWidth;
		if (setMove(i, *player, tempboard))
			player = changeSide(player);
		if (CheckFour(tempboard, *playerAI) == 1)
			return true;
		if (CheckFour(tempboard, *playerHuman) == 1 || FullBoard(tempboard) == BoardWidth)
			return false;
	}
}

int generateRandomOpenings(int board_sum) {
	ofstream saved_boards("random_openings.txt", ios::app);

	if (!saved_boards) {
		cout << "Error opening output file" << endl;
		return -1;
	}

	int board_no = 0;
	for (int i = 0; i < board_sum; i++) {
		cout << i << endl;
		char temp_board[BoardLength][BoardWidth];
		playerOne.playerID = 'X';
		playerTwo.playerID = 'O';

		for (int step = 0; step < 6; step++) {
			int move = rand() % BoardWidth;
			if(setMove(move, playerOne, temp_board))cout<<"yes1"<<endl;
			if (CheckFour(temp_board, playerOne) == 1) {
				i--;
				break;
			}
			move = rand() % BoardWidth;
			if (setMove(move, playerTwo, temp_board))cout << "yes2" << endl;
			if (CheckFour(temp_board, playerTwo) == 1) {
				i--;
				break;
			}
		}
		DisplayBoard(temp_board);

		//write board to file
		for (int row = 0; row < 6; row++){
			for (int col = 0; col < 7; col++) {
				if (temp_board[row][col] == ' ')saved_boards << "*";
				else saved_boards << temp_board[row][col];
			}
		}
		
		//restart board
		for (int i = 0; i < BoardLength; i++)
		{
			for (int ix = 0; ix < BoardWidth; ix++)
			{
				temp_board[i][ix] = '*';
			}
		}

		//change line
		saved_boards << endl;
	}

	saved_boards.close();

	cout << "boards generation done";
}