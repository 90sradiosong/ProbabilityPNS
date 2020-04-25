#include "stdafx.h"
#include "PPNS.h"
#include "FileOperator.h"
#include <time.h>

using namespace std;

int PPNS::startSolver()
{
	if (solved)
	{
		for (list<Node>::iterator i = curNode->child.begin(); i != curNode->child.end(); i++)
		{
			if (i->index == lastMove)
			{
				curNode = &(*i);
				break;
			}
		}

		for (list<Node>::iterator i = curNode->child.begin(); i != curNode->child.end(); i++)
		{
			if (i->pn == 1.0)
			{
				curNode = &(*i);
				return i->index;
			}
		}
	}
	else
	{
		clock_t start = clock();
		int itnum = 0;
		while (1)
		{
			itnum++;
			clock_t end = clock();
			double time_elapsed = ((double)(end - start)) / CLOCKS_PER_SEC;
			if (nodeNum >= Limit || time_elapsed>=420) {
				cout << "PN = " << r.pn << "DN = " << r.dn << endl;
				std::string result;
				result.append("out of memory = ");
				result.append(std::to_string(itnum));
				result.append(" ");
				result.append(std::to_string(nodeNum));
				result.append(" ");
				result.append(std::to_string(time_elapsed));
				result.append(" ");
				result.append(std::to_string(r.pn));
				result.append(" ");
				result.append(std::to_string(r.dn));
				result.append(";\n");
				writeIntoFile("PPNSTresult3.txt", result);
				return 0;
			}
			search(&r, *playerAI);
			if (itnum % 10000 == 0) {
				cout << "iteration number : " << itnum << " PN = " << r.pn << " DN = " << r.dn << "time elapsed" << time_elapsed<< endl;
				std::string result;
				result.append(std::to_string(itnum));
				result.append(" ");
				result.append(std::to_string(nodeNum));
				result.append(" ");
				result.append(std::to_string(time_elapsed));
				result.append(" ");
				result.append(std::to_string(r.pn));
				result.append(" ");
				result.append(std::to_string(r.dn));
				result.append(";\n");
				writeIntoFile("PPNSTPNDN3.txt", result);
			}
			//if(r.pn==1.0)
			if (fabs(r.pn - 1.0) <= (epsilon * fabs(r.pn)))
			{
				solved = true;
				curNode = best;
				cout << "solved" << endl;
				cout << "Iteration number: " << itnum << endl;
				cout << "Node number: " << nodeNum << endl;
				cout << "PN = " << r.pn << "DN = " << r.dn << endl;
				std::string result;
				result.append("result = ");
				result.append(std::to_string(itnum));
				result.append(" ");
				result.append(std::to_string(nodeNum));
				result.append(" ");
				result.append(std::to_string(time_elapsed));
				result.append(" ");
				result.append(std::to_string(r.pn));
				result.append(" ");
				result.append(std::to_string(r.dn));
				result.append(";\n");
				writeIntoFile("PPNSTresult3.txt", result);
				if(best!=NULL)return best->index;
				else return 0;
			}
			//else if (r.dn==1.0)
			else if (fabs(r.dn - 1.0) <= (epsilon * fabs(r.dn)))
			{
				cout << "unsolved" << endl;
				cout << "Iteration number: " << itnum << endl;
				cout << "Node number: " << nodeNum << endl;
				cout << "PN = " << r.pn << "DN = " << r.dn << endl;
				std::string result;
				result.append("unsolved = ");
				result.append(std::to_string(itnum));
				result.append(" ");
				result.append(std::to_string(nodeNum));
				result.append(" ");
				result.append(std::to_string(time_elapsed));
				result.append(" ");
				result.append(std::to_string(r.pn));
				result.append(" ");
				result.append(std::to_string(r.dn));
				result.append(";\n");	
				writeIntoFile("PPNSTresult3.txt", result);
				return 0;
			}
		}
	}
}

void PPNS::getPNDN(Node *p, playerInfo& activePlayer)
{
	if (CheckFour(board, *playerAI) == 1)
	{
		p->pn = 1.0;
		p->dn = 0.0;
		return;
	}
	if(CheckFour(board, *playerHuman) == 1 || FullBoard(board) == BoardWidth)
	{
		p->pn = 0.0;
		p->dn = 1.0;
		return;
	};

	int wnum = 0;
	for (int i = 0; i < MaxSimulationNum; i++)
	{
		if (simulate(activePlayer)) wnum++;
	}

	if (wnum == 0)
	{
		p->pn = 0.01;
		p->dn = 0.99;
		return;
	}
	else if (wnum == MaxSimulationNum)
	{
		p->pn = 0.99;
		p->dn = 0.01;
		return;
	}

	p->pn = (double)wnum / MaxSimulationNum;
	p->dn = 1.0 - p->pn;
}

void PPNS::search(Node* p, playerInfo& activePlayer)
{
	if (p->child.size() == 0)
	{
		//if (p == &r)
		//{
		//	Node c;
		//	c.index = 3;
		//	//c.pn = -1.0;
		//	p->child.push_back(c);
		//	//c.visit = 0;
		//	setMove(c.index, activePlayer, board);
		//	getPNDN(&c, *changeSide(&activePlayer));
		//	delMove(c.index, activePlayer, board);
		//	return;
		//}
		/*int i = findThreat(activePlayer, board);
		if (i != -1)
		{
			Node c;
			c.index = i;
			setMove(c.index, activePlayer, board);
			getPNDN(&c, *changeSide(&activePlayer));
			delMove(c.index, activePlayer, board);
			p->child.push_back(c);
			return;
		}*/
		double andp = 1.0;
		double orp = 1.0;

		for (int i = 0; i < BoardWidth; i++)
		{
			if (board[0][i] != '*') continue;
			Node c;
			c.index = i;
			nodeNum++;
			setMove(c.index, activePlayer, board);
			getPNDN(&c, *changeSide(&activePlayer));
			delMove(c.index, activePlayer, board);
			p->child.push_back(c);
			andp *= c.pn;
			orp *= c.dn;
		}

		if (&activePlayer == playerAI)
		{
			p->pn = 1.0 - orp;
			p->dn = 1.0 - p->pn;
		}
		else
		{
			p->pn = andp;
			p->dn = 1.0 - p->pn;
		}
		return;
	}

	if (&activePlayer == playerAI)
	{
		Node* c = NULL;
		double maxpn = -MaxValue - 1;
		for (list<Node>::iterator i = p->child.begin(); i != p->child.end(); i++)
		{
			if (i->pn == 0.0) continue;
			if (i->pn > maxpn)
			{
				maxpn = i->pn;
				c = &(*i);
			}
		}

		setMove(c->index, activePlayer, board);
		search(c, *changeSide(&activePlayer));
		delMove(c->index, activePlayer, board);

		double andp = 1.0;
		double orp = 1.0;
		maxpn = -MaxValue - 1;

		for (list<Node>::iterator i = p->child.begin(); i != p->child.end(); i++)
		{
			andp *= i->pn;
			orp *= i->dn;
			//orp *= 1.0 - i->pn;

			if (i->pn > maxpn)
			{
				maxpn = i->pn;
				best = &(*i);
			}
		}
		p->pn = 1.0 - orp;
		p->dn = 1.0 - p->pn;
		return;
	}
	else
	{
		Node* c = NULL;
		double maxdn = -MaxValue - 1;
		for (list<Node>::iterator i = p->child.begin(); i != p->child.end(); i++)
		{
			if (i->pn == 1.0) continue;
			
			if (i->dn > maxdn)
			//if (1.0 - i->pn > maxdn)
			{
				maxdn = i->dn;
				//maxdn = 1.0 - i->pn;
				c = &(*i);
			}
		}

		setMove(c->index, activePlayer, board);
		search(c, *changeSide(&activePlayer));
		delMove(c->index, activePlayer, board);

		double andp = 1.0;
		double orp = 1.0;

		for (list<Node>::iterator i = p->child.begin(); i != p->child.end(); i++)
		{
			andp *= i->pn;
			orp *= i->dn;
			//orp *= 1.0 - i->pn;
		}
		p->pn = andp;
		p->dn = 1.0 - p->pn;
		return;
	}

	return;
}