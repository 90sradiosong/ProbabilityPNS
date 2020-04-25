//Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <cmath>
#include <vector>
#include <iomanip>
#include <ctime>
#include <time.h>
#include "FileOperator.h"
#include "board.h"
#include "position.h"
#include "MonteCarlo.h"
#include "PNS.h"
#include "PPNS.h"
#include "DFPNS.h"
#include "MCPNS.h"
using namespace std;


int main()
{
	cout << "What do you want to do? [Create random board(g)/Solve with PNS(p)/Solve with PPNS(n)]  ";
	char mode;
	cin >> mode;
	if (mode == 'g') {
		board Board(0);
		Board.outputBoard();
		Board.createRandomBoard(11, 400);

		Board.loadBoard(0);
	}
	else if (mode == 'p') {
		cout << "pns mode";
		board Board(0, 8, 8);
		position p;
		vector<position> valid_moves;
		int player = 0;//AI goes first = 0
		int start;
		int count;

		cout << "board id = "; cin >> start;
		cout << "count "; cin >> count;
		PNS *pns;

		for (int i = start; i < (start+count); i++) {
			std::string resultpns;
			resultpns.append("id");
			resultpns.append(std::to_string(i));
			writeIntoFile("PNSresult.txt", resultpns);
			pns = new PNS();
			Board.loadBoard(i);
			pns->startSolver(Board);
		}
	}
	else if (mode == 'n') {
		cout << "ppns mode";
		board Board(0, 8, 8);
		position p;
		vector<position> valid_moves;
		int player = 0;//AI goes first = 0
		int start;
		int count;

		cout << "board id = "; cin >> start;
		cout << "count "; cin >> count;
		PPNS *pns;

		for (int i = start; i < (start + count); i++) {
			std::string resultpns;
			resultpns.append("id");
			resultpns.append(std::to_string(i));
			writeIntoFile("PPNSresult.txt", resultpns);
			pns = new PPNS();
			Board.loadBoard(i);
			pns->startSolver(Board);

			if (pns->solved == true) {
				double b=-1;
				int d=-1;
				pns->solutionTreeShape(&b, &d);
				cout << "\naverage branch size = " << b << "total depth = " << d << endl;
				writeIntoFile("PPNSbranchdepth.txt", resultpns);
				resultpns.clear();
				resultpns.append("(");
				resultpns.append(std::to_string(b));
				resultpns.append(",");
				resultpns.append(std::to_string(d));
				resultpns.append(")\n");
				writeIntoFile("PPNSbranchdepth.txt", resultpns);
			}
		}
	}
	else {
		cout << "emmm....sorry, what did you say again?";
	}

    return 0;
}

