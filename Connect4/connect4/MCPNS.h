#pragma once
#include "stdafx.h"
#include "Connect4.h"

class MCPNS
{
public:
	Node * best;
	MCPNS()
	{
		//solved = false;
		//r.child.clear();
		r.index = 0;
		r.pn = 0;
		r.dn = 0.0;
		//r.visit = 0;
	}
	int startSolver();
	void getPNDN(Node * p, playerInfo& activePlayer);
	void search(Node* p, playerInfo& activePlayer);
};

