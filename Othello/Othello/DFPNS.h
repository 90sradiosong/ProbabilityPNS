#pragma once
#ifndef DFPNS_H
#define	DFPNS_H

#include <iostream>
#include <vector>
#include <list>
#include "board.h"
#include "treeNode.h"

struct DFPNode {
	Node *n;
	double thpn;
	double thdn;
	vector<DFPNode> child;
};

struct TransEntry {
	DFPNode *entry;
	board *entryBoard;
	int md;
};

class TranspositionTable {
public:
	vector<TransEntry> entries;
	TranspositionTable() {
		entries.clear();
	}
	bool CompareBoard(board &b1, board &b2);
	int TTLookup(board *boardstate);
	bool TTStore(DFPNode *newentry, board *boardstate);
	void showTableEntries();
};

class DFPNS {
public:
	DFPNode *best;
	DFPNode *root;
	DFPNode *curNode;
	DFPNS()
	{
		root = new DFPNode();
		root->n = new Node(true);
		root->n->setIndex(0);
		root->thpn=MaxValue;
		root->thdn=MaxValue;
		best = NULL;
		curNode = NULL;
		nodeNum = 0;
		iterationNum = 0;
		lastMove = -1;
		solved = false;
		table.entries.clear();
	}

	int startSolver(board &currentBoard);
	void getPNDN(DFPNode *p, int &thisPlayer, board &b);
	void search(DFPNode*p, int &thisPlayer, vector<position> &currentPositions, board &b);
	int getlastMove();
	int nodeNum;
	int iterationNum;
	int time;
	bool solved;
	int lastMove;
	board *currentBoard;
	TranspositionTable table;
	void drawCurrentBoard();
	int deltac;
	int delta2;
};

#endif /* DFPNS_H */
#pragma once
