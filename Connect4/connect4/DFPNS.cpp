#include "stdafx.h"
#include "DFPN.h"
#include "FileOperator.h"
#include <time.h>

using namespace std;

int DFPNS::startSolver()
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
			if (i->pn == 0)
			{
				curNode = &(*i);
				return i->index;
			}
		}
	}
	else
	{
		int itnum = 0;
		clock_t start = clock();
		while (1)
		{
			itnum++;
			//if (itnum % 10000 == 0)cout << itnum << endl;
			clock_t end = clock();
			double time_elapsed = ((double)(end - start)) / CLOCKS_PER_SEC;
			if (nodeNum >= Limit || time_elapsed >= 420) {
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
				writeIntoFile("DFPNSTresult.txt", result);
				return 0;
			}
			if (itnum % 10000 == 0) {
				cout << "iteration number : " << itnum << " PN = " << r.pn << " DN = " << r.dn << "time elapsed" << time_elapsed << endl;
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
				writeIntoFile("DFPNSTPNDN.txt", result);
			}
			search(&r, *playerAI);
			if (r.pn == 0)
			{
				solved = true;
				curNode = best;
				cout << "solved" << endl;
				cout << "Iteration number: " << itnum << endl;
				cout << "Node number: " << nodeNum << endl;
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
				writeIntoFile("DFPNSTresult.txt", result);
				if (best != NULL)return best->index;
				else return 0;
				return best->index;
			}
			else if (r.dn == 0)
			{
				cout << "unsolved" << endl;
				cout << "Iteration number: " << itnum << endl;
				cout << "Node number: " << nodeNum << endl;
				std::string result;
				result.append("unsolved = ");
				result.append(std::to_string(itnum));
				result.append(" ");
				result.append(std::to_string(nodeNum));
				result.append(" ");
				result.append(std::to_string(time_elapsed));
				result.append(";\n");
				writeIntoFile("DFPNSTresult.txt", result);
				return 0;
			}
		}
	}
}

void DFPNS::getPNDN(Node *p, playerInfo& activePlayer)
{
	if (CheckFour(board, *playerAI) == 1)
	{
		p->pn = 0;
		p->dn = MaxValue;
		return;
	}
	if (CheckFour(board, *playerHuman) == 1 || FullBoard(board) == BoardWidth)
	{
		p->pn = MaxValue;
		p->dn = 0;
		return;
	};

	p->pn = 1;
	p->dn = 1;
}

void DFPNS::search(Node* p, playerInfo& activePlayer)
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
		double sumpn = 0;
		double sumdn = 0;
		double minpn = MaxValue + 1;
		double mindn = MaxValue + 1;

		for (int i = 0; i < BoardWidth; i++)
		{
			if (board[0][i] != '*') continue;
			Node c;
			c.index = i;
			//c.visit = 0;
			setMove(c.index, activePlayer, board);
			getPNDN(&c, *changeSide(&activePlayer));
			delMove(c.index, activePlayer, board);
			nodeNum++;
			p->child.push_back(c);

			if (c.pn < minpn) minpn = c.pn;
			if (c.dn < mindn) mindn = c.dn;
			c.pn + sumpn >= MaxValue ? sumpn = MaxValue : sumpn += c.pn;
			c.dn + sumdn >= MaxValue ? sumdn = MaxValue : sumdn += c.dn;
		}

		if (&activePlayer == playerAI)
		{
			p->pn = minpn;
			p->dn = sumdn;
		}
		else
		{
			p->pn = sumpn;
			p->dn = mindn;
		}
		return;
	}

	if (&activePlayer == playerAI)
	{
		Node* c = NULL;
		double minpn = MaxValue + 1;
		for (list<Node>::iterator i = p->child.begin(); i != p->child.end(); i++)
		{
			if (i->pn < minpn)
			{
				minpn = i->pn;
				c = &(*i);
			}
		}

		setMove(c->index, activePlayer, board);
		search(c, *changeSide(&activePlayer));
		delMove(c->index, activePlayer, board);

		double sumdn = 0;
		minpn = MaxValue + 1;

		for (list<Node>::iterator i = p->child.begin(); i != p->child.end(); i++)
		{
			if (i->pn < minpn)
			{
				minpn = i->pn;
				best = &(*i);
			}
			sumdn += i->dn;
		}
		sumdn >= MaxValue ? p->dn = MaxValue : p->dn = sumdn;
		p->pn = minpn;
		return;
	}
	else
	{
		Node* c = NULL;
		double mindn = MaxValue + 1;
		for (list<Node>::iterator i = p->child.begin(); i != p->child.end(); i++)
		{
			if (i->dn < mindn)
			{
				mindn = i->dn;
				c = &(*i);
			}
		}

		setMove(c->index, activePlayer, board);
		search(c, *changeSide(&activePlayer));
		delMove(c->index, activePlayer, board);

		mindn = MaxValue + 1;
		double sumpn = 0;

		for (list<Node>::iterator i = p->child.begin(); i != p->child.end(); i++)
		{
			if (i->dn < mindn)
			{
				mindn = i->dn;
			}
			sumpn += i->pn;
		}
		sumpn >= MaxValue ? p->pn = MaxValue : p->pn = sumpn;
		p->dn = mindn;
		return;
	}

	return;
}