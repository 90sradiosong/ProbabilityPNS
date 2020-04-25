#pragma once
#include "stdafx.h"
#include <list>
#include "iostream"
#define MaxSimulationNum 60
#define BoardWidth 7
#define BoardLength 6
#define MaxValue 1000000000000
#define Limit 35000000
#define epsilon 0.001

using namespace std;

extern char board[BoardLength][BoardWidth];

struct playerInfo
{
	char playerName[81];
	char playerID;
};

struct Node
{
	char index;
	double pn;
	double dn;
	//int visit;
	list<Node> child;
};

extern playerInfo playerOne, playerTwo;
extern bool solved;
extern Node r;
extern Node* curNode;
extern int lastMove;
extern playerInfo* playerAI;
extern playerInfo* playerHuman;
extern int nodeNum;

int LoadBoard(char board[][BoardWidth], int openings_id);

int PlayerDrop(char board[][BoardWidth], playerInfo& activePlayer);

void CheckBellow(char board[][BoardWidth], playerInfo& activePlayer, int dropChoice);

void DisplayBoard(char board[][BoardWidth]);

int CheckFour(char board[][BoardWidth], playerInfo& activePlayer);

int FullBoard(char board[][BoardWidth]);

void PlayerWin(playerInfo& activePlayer);

int restart(char board[][BoardWidth]);

int RestartWithoutConfirmation(char board[][BoardWidth]);

bool simulate(playerInfo& activePlayer);

void delMove(int index, playerInfo& activePlayer, char board[][BoardWidth]);

bool setMove(int index, playerInfo& activePlayer, char board[][BoardWidth]);

playerInfo* changeSide(playerInfo* activePlayer);

int findThreat(playerInfo& activePlayer, char board[][BoardWidth]);

int generateRandomOpenings(int board_sum);

