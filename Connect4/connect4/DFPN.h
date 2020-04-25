#pragma once
#include "stdafx.h"
#include "Connect4.h"
#include <vector>

class DFPNS
{
public:
	Node * best;
	DFPNS()
	{
		//solved = false;
		//r.child.clear();
		r.index = 0;
		r.pn = 0;
		r.dn = 0.0;
		//r.visit = 0;
	}
	int delta2;
	int PN;
	int DN;
	int startSolver();
	void getPNDN(Node * p, playerInfo& activePlayer);
	void search(Node* p, playerInfo& activePlayer);
};

struct TransTableEntry {
	Node *n;
	int b[BoardLength][BoardWidth];
	int pn;
	int dn;
	int md;
};

class TranspositionTable {
private:
	vector<TransTableEntry> entries;
public:
	bool TTlookup(Node *n, int PN, int DN, int currentboard[BoardLength][BoardWidth]);
	bool TTStore(Node *n,int currentboard[BoardLength][BoardWidth]);
};
