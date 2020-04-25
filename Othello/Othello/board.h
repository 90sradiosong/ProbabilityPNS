#pragma once
#include <iomanip>
#include <cmath>
#include <vector>
#include <iostream>
#include <fstream>
#include <time.h>
#include "position.h"
#include "treenode.h"
using namespace std;

class board
{
private:

public:
	int height;
	int width;
	//default constructor(initialization)
	board(int P = 0, int h = 8, int w = 8) {
		//  1 stands for the white side;
		//  0 stands for the dark side;
		// -1 stands for empty grids.
		memset(reversiBoard, -1, sizeof(reversiBoard));
		if (h == w) {
			switch (h) {
			case 8: reversiBoard[3][3] = 1;
					reversiBoard[4][4] = 1;
					reversiBoard[3][4] = 0;
					reversiBoard[4][3] = 0; 
					break;
			case 6:	reversiBoard[2][2] = 1;
					reversiBoard[3][3] = 1;
					reversiBoard[2][3] = 0;
					reversiBoard[3][2] = 0; 
					break;
			case 4:	reversiBoard[1][1] = 1;
					reversiBoard[2][2] = 1;
					reversiBoard[1][2] = 0;
					reversiBoard[2][1] = 0; break;
			default:cout << "size invalid, use default 8 x 8";
					h = 8;
					w = 8;
					reversiBoard[3][3] = 1;
					reversiBoard[4][4] = 1;
					reversiBoard[3][4] = 0;
					reversiBoard[4][3] = 0; 
					break;
			}
		}
		else {
			cout << "size invalid, use default 8 x 8";
			h = 8;
			w = 8;

			reversiBoard[3][3] = 1;
			reversiBoard[4][4] = 1;
			reversiBoard[3][4] = 0;
			reversiBoard[4][3] = 0;
		}
		
		/*reversiBoard[3][3] = 1; 
		reversiBoard[4][4] = 1;
		reversiBoard[3][4] = 0;
		reversiBoard[4][3] = 0;*/
		//default role: white
		curPlayer = P;
		//outputBoard();
		height = h;
		width = w;
	}

	//copy constructor
	board(board &B) {
		width = B.width;
		height = B.height;
		for (int i = 0; i < width; i++)
			for (int j = 0; j < height; j++)
				reversiBoard[i][j] = B.reversiBoard[i][j];
		curPlayer = B.curPlayer;
	}

	//copy another board
	void copy(board B) {
		width = B.width;
		height = B.height;
		for (int i = 0; i < width; i++)
			for (int j = 0; j < height; j++)
				reversiBoard[i][j] = B.reversiBoard[i][j];
		curPlayer = B.curPlayer;
	}

	//print boards to console
	void outputBoard() {
		cout << "[O goes first]\n";
		cout << "Current Board Status:\n    ";
		for (int i = 0; i < width; i++) {
			cout << setw(4) << i;
		}
		cout << endl;
		for (int i = 0; i < width; i++) {
			cout << i << " : ";
			for (int j = 0; j < height; j++) {
				if (reversiBoard[i][j] == -1)
					cout << setw(4) << "";
				else
				{
					if(reversiBoard[i][j] == 0)
						cout << setw(4) << 'O';
					else 
						cout << setw(4) << 'X';
				}
				//	cout << setw(4) << reversiBoard[i][j];
			}
			cout << endl;
		}
		cout << endl;
	}

	//place a piece on position pos if pos is valid
	////pos: the position where a piece is to be placed
	bool placeOn(position pos, int &player) {
		curPlayer = player;
		if (!isValidStep(pos)) return false;
		//if (reversiBoard[pos.x][pos.y] != -1) return false;
		reversiBoard[pos.x][pos.y] = player;
		flip(pos);
		player = !player;
		return true;
	}
	//UNFINISHED CODE DO NOT USE
	bool placeOff(position pos) {
		reversiBoard[pos.x][pos.y] = -1;
		return true;
	}

	//judge if a step is valid
	bool isValidStep(position &pos) {
		int directions = -1;
		int x = pos.x, y = pos.y;
		for (int i = -1; i < 2; i++) {
			for (int j = -1; j < 2; j++) {
				if (i == 0 && j == 0) continue;
				directions++;
				//check all directions to see if it is valid;
				//1. a piece of the opponents' beside it
				//2. on that direction, there is a piece of my own side;
				if (reversiBoard[x + i][y + j] != abs(1 - curPlayer)) continue;
				int cnt = 1;
				bool cont = true;// continue if still valid 
				while (cnt++ && cont) {
					int x1 = x + cnt*i, y1 = y + cnt*j;//(x1, y1)stands for checking same-side pieces
					if (x1 < 0 || x1 > (width-1) || y1 < 0 || y1 > (height-1)) break;
					//check if this is a piece of the same side
					if (reversiBoard[x1][y1] == curPlayer) {
						pos.dir[directions] = 1;
						break;
					}
					if (reversiBoard[x1][y1] == -1) {
						break;
					}
				}
			}
		}
		int sum = 0;
		for (int i = 0; i < height; i++) sum += pos.dir[i];
		if (sum) return true;
		return false;
	}

	//attain all valid choices for the current step and store them in "vector<position> curValid"
	bool getAllValidMoves(vector<position> &curValid) {
		//determines whether each step is valid
		for (int i = 0; i < width; i++) {
			for (int j = 0; j < height; j++) {
				if (reversiBoard[i][j] == -1) {
					position p(i,j);
					if (isValidStep(p)) {
						//reversiBoard[i][j] = 2;
						curValid.push_back(p);
					}
				}
			}
		}
		if (curValid.empty()) return false;  
		else return true;
	}
	
	//get current game status for white pieces;
	//2: win;
	//1: lose;
	//0: draw;
	//-1: game is going on	
	int getStatus(vector<position> &tmp) {	
		if (!getAllValidMoves(tmp)) { //no more moves for player 1
			curPlayer = !curPlayer; 
			if (!getAllValidMoves(tmp)) { //no more moves for player 2
				//int opponent = abs(curPlayer - 1);
				int win = 0, lose = 0;
				for (int i = 0; i < width; i++) {
					for (int j = 0; j < height; j++) {
						if (reversiBoard[i][j] == 1)
							win++;
						else if (reversiBoard[i][j] == 0)
							lose++;
						else continue;
					}
				}
				if (win == lose) return 0;
				else if (win < lose) return 1;
				else return 2;
			}
		} //available move for player 1 or 2
		return -1;
	}

	//privided that position p is a valid position, we use flip to flip all the pieces available.
	void flip(position p) {
		int direction = -1;
		for (int i = -1; i < 2; i++) {
			for (int j = -1; j < 2; j++) {
				if (i == 0 && j == 0) continue;
				direction++;
				//check all directions to see if it is valid;
				//1. a piece of the opponents' beside it
				//2. on that direction, there is a piece of my own side;
				if (p.dir[direction] != 0) {
					int x = p.x, y = p.y;
					reversiBoard[x][y] = curPlayer;
					int cnt = 0;
					bool cont = true;// continue if still valid 
					while (++cnt && cont) {
						int x1 = x + cnt*i, y1 = y + cnt*j;//(x1, y1)stands for checking same-side pieces
						if (x1 < 0 || x1 > (width-1) || y1 < 0 || y1 > (height-1)) break;
						//check if this is a piece of the same side
						if (reversiBoard[x1][y1] == curPlayer) 	break;
						reversiBoard[x1][y1] = curPlayer;
					}
				}
			}
		}

	}

	double playUntilDone(board &B) {
		board tempBoard(B);
		int tempPlayer = tempBoard.curPlayer;
		vector<position> tempPos;
		int randomMove;
		//check current board state
		int curState = tempBoard.getStatus(tempPos);
		while (curState == -1) {
			int availableMoves = tempPos.size();
			randomMove = rand() % availableMoves;
			tempBoard.placeOn(tempPos.at(randomMove), tempBoard.curPlayer);
			tempBoard.curPlayer = tempPlayer;
			tempPos.clear();
			curState = tempBoard.getStatus(tempPos);
			tempPlayer = abs(1 - tempPlayer);
			tempBoard.curPlayer = tempPlayer;

			if (tempPos.size() <= 0 && curState == -1) {
				curState = 3;
			}
		}
		//cout << curState << endl;
		//tempBoard.outputBoard();
		if (curState == 1)return 1;
		return 0;

	}

	double getMonteCarloWeight(board &B) {
		double w = 0;
		for (int i = 0; i < MaxSimulationNum; i++) {
			if (playUntilDone(B) == 1)w++;
		}
		return w;
	}
	
//private:
	//store all the current valid choices
	//may not be of use in this class. perhaps it should be shifted to mcts
	//vector<position> curValid;

	//player: 1 or 0, depending on the turn of the player
	int curPlayer;
	//store the board status
	int reversiBoard[8][8];

	int createRandomBoard(int steps,int count) {
		//create <count> random board until <steps>
		vector<position> seq;
		ofstream saved_boards("random_openings22.txt", ios::app);

		if (!saved_boards) {
			cout << "Error opening output file" << endl;
			return -1;
		}

		srand(time(NULL));

		for (int i = 0; i < count; i++) {
			//clear board
			for (int row = 0; row < 8; row++) {
				for (int col = 0; col < 8; col++) {
					reversiBoard[row][col] = -1;
				}
			}
			seq.clear();
			curPlayer = 0;

			reversiBoard[3][3] = 1;
			reversiBoard[4][4] = 1;
			reversiBoard[3][4] = 0;
			reversiBoard[4][3] = 0;

			//create valid positions
			for(int j=0;j<steps;j++){
				vector<position> valid_moves;
				bool valid=true;
				if (getAllValidMoves(valid_moves) == true) {
					int move_seq = rand() % valid_moves.size();
					position move = valid_moves.at(move_seq);
					cout << "\navailable moves = " << valid_moves.size() << " chosen move = " << move_seq<<"steps no = "<<j;
					if (placeOn(move, curPlayer) == true) {
						seq.push_back(move);
						if (getAllValidMoves(valid_moves) == true) {
							move_seq = rand() % valid_moves.size();
							move = valid_moves.at(move_seq);
							cout << "\navailable moves = " << valid_moves.size() << " chosen move = " << move_seq<<"steps no = "<<j;
							if (placeOn(move, curPlayer) == true) {
								seq.push_back(move);
							}else valid = false;
						}else valid = false;
					}else valid = false;
				}else valid = false;

				if (valid == false) {
					cout << "generating board fails";
					i--;
					break;
				}
			}
			//cout << steps<<endl;
			outputBoard();

			//write Board into file
			for (int row = 0; row < 8; row++) {
				for (int col = 0; col < 8; col++) {
					if (reversiBoard[row][col] == -1)saved_boards << "*";
					else saved_boards << reversiBoard[row][col];
				}
			}

			saved_boards << endl;
		}

		saved_boards.close();
		cout << "board generation done";
	}

	int loadBoard(int count) {
		//load board number <count> from files

		ifstream board_openings("random_openings32.txt");

		if (!board_openings) {
			cout << "Error opening input file" << endl;
			return -1;
		}

		//clear current board
		for (int row = 0; row < 8; row++) {
			for (int col = 0; col < 8; col++) {
				reversiBoard[row][col] = -1;
			}
		}

		int board_no = 0;
		int count_row = 0;
		int count_col = 0;
		char move;

		for (int i = 0; !board_openings.eof(); i++) {
			board_openings.get(move);
			if (board_no == count) {
				switch(move){
					case '*':reversiBoard[count_row][count_col] = -1; break;
					case '1':reversiBoard[count_row][count_col] = 1; break;
					case '0':reversiBoard[count_row][count_col] = 0; break;
					default:reversiBoard[count_row][count_col] = -1; break;
				}
				count_col++;
				if (count_row == 8) break;
				if (count_col == 8) {
					count_row++;
					count_col = 0;
				}
			}
			if (move == '\n') board_no++;
		}

		cout << "board loading done\n";
		outputBoard();
		board_openings.close();

	}
};
