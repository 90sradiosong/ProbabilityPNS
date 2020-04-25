#pragma once
#include "stdafx.h"
#include "Connect4.h"
#include <vector>

struct UCTNode
{
	char index;
	double pn;
	double dn;
	int visit;
	list<UCTNode> child;
	vector<int> action;
};

extern UCTNode uctr;
extern UCTNode* uctcurNode;

class UCT
{
private:
	double uct_k;
	UCTNode* select(UCTNode *p);
	UCTNode* expand(UCTNode *p, playerInfo& activePlayer);
	//char board[BoardLength][BoardWidth];
public:
	double search(UCTNode * p, playerInfo& activePlayer);
	void finalselect(UCTNode * p);
	UCTNode * best;
	UCT() : uct_k(sqrt(2))
	{
		uctr.index = 0;
		uctr.pn = 0;
		uctr.dn = 0.0;
		uctr.visit = 0;
	}
	UCT(double uct_k)
	{
		uctr.index = 0;
		uctr.dn = 0;
		uctr.pn = 0;
		uctr.visit = 0;
		UCT::uct_k = uct_k;
	}
	int startSolver();
};
