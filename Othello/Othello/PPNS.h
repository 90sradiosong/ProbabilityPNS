#pragma once
#ifndef PPNS_H
#define	PPNS_H

#include <iostream>
#include <vector>
#include <list>
#include "board.h"
#include "treeNode.h"

class PPNS {
public:
	Node *best;
	Node *root;
	Node *curNode;
	PPNS()
	{
		root = new Node(true);
		root->setIndex(0);
		root->setDN(0);
		root->setPN(0);
		root->child.clear();
		best = NULL;
		curNode = NULL;
		nodeNum = 0;
		iterationNum = 0;
		lastMove = -1;
		solved = false;

	}
	int startSolver(board &currentBoard);
	void getPNDN(Node *p, int &thisPlayer, board &b);
	void search(Node*p, int &thisPlayer, vector<position> &currentPositions, board &b);
	int getlastMove();
	int nodeNum;
	int iterationNum;
	int time;
	bool solved;
	int lastMove;
	board *currentBoard;
	void drawCurrentBoard();
	void solutionTreeShape(double *b, int *d);
};


#endif /* PPNS_H */
