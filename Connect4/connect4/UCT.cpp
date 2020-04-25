#include "stdafx.h"
#include "UCT.h"
#include <algorithm>
#include "FileOperator.h"
#include <time.h>

using namespace std;

UCTNode uctr;
UCTNode* uctcurNode;

int UCT::startSolver()
{
	//std::copy(&n_board[0][0], &n_board[0][0] + BoardLength * BoardWidth, &board[0][0]);
	if (solved)
	{
		for (list<UCTNode>::iterator i = uctcurNode->child.begin(); i != uctcurNode->child.end(); i++)
		{
			if (i->index == lastMove)
			{
				uctcurNode = &(*i);
				break;
			}
		}

		for (list<UCTNode>::iterator i = uctcurNode->child.begin(); i != uctcurNode->child.end(); i++)
		{
			if (i->dn == MaxValue)
			{
				uctcurNode = &(*i);
				return i->index;
			}
		}
	}
	else
	{
		int itnum = 0;
		//start timer here
		clock_t start = clock();
		while (1)
		{
			clock_t end = clock();
			double time_elapsed = ((double)(end - start)) / CLOCKS_PER_SEC;
			itnum++;
			//if (itnum % 10000 == 0)cout << itnum << endl;
			if (nodeNum >= Limit) {
				//end timer here
				std::string result;
				result.append("out of memory = ");
				result.append(std::to_string(itnum));
				result.append(" ");
				result.append(std::to_string(nodeNum));
				result.append(" ");
				result.append(std::to_string(time_elapsed));
				result.append(";");
				if(uct_k==0)writeIntoFile("MCTSresult.txt", result);
				else writeIntoFile("UCTresult.txt", result);
				return 0;
			}
			search(&uctr, *playerAI);
			if (uctr.dn == -MaxValue)
			{

				solved = true;
				finalselect(&uctr);
				uctcurNode = best;
				cout << "solved" << endl;
				cout << "Iteration number: " << itnum << endl;
				cout << "Node number: " << nodeNum << endl; std::string result;
				result.append("result = ");
				result.append(std::to_string(itnum));
				result.append(" ");
				result.append(std::to_string(nodeNum));
				result.append(" ");
				result.append(std::to_string(time_elapsed));
				result.append(";");
				if (uct_k == 0)writeIntoFile("MCTSresult.txt", result);
				else writeIntoFile("UCTresult.txt", result);
				if (best != NULL)return best->index;
				else return 0;
			}
			else if (uctr.dn == MaxValue)
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
				result.append(";");
				if (uct_k == 0)writeIntoFile("MCTSresult.txt", result);
				else writeIntoFile("UCTresult.txt", result);
				return 0;
			}
		}
	}
}

void UCT::finalselect(UCTNode* p)
{
	double best_utc_score = -std::numeric_limits<double>::max();

	UCTNode* best_node = NULL;

	// iterate all immediate children and find best UTC score

	int num_children = p->child.size();

	for (list<UCTNode>::iterator i = p->child.begin(); i != p->child.end(); i++)
	{
		
		if (i->dn == MaxValue) best_node = &(*i);
		//ucb
		UCTNode* child = &(*i);
		double uct_score = child->pn + 1.0 / sqrt((double)p->visit);

		if (uct_score > best_utc_score) {

			best_utc_score = uct_score;
			best_node = child;
		}
	}
	best = best_node;
}

UCTNode * UCT::select(UCTNode* p)
{
	double best_utc_score = -std::numeric_limits<double>::max();

	UCTNode* best_node = NULL;

	// iterate all immediate children and find best UTC score

	int num_children = p->child.size();

	for (list<UCTNode>::iterator i = p->child.begin(); i != p->child.end(); i++)
	{
		if (i->dn == -MaxValue) continue; //ignore all the proved node

		UCTNode* child = &(*i); //child

		double uct_exploitation = (double)child->pn;// / (child->visit + FLT_EPSILON); //Here, pn is regarded as the value

		double uct_exploration = sqrt(log((double)p->visit + 1) / (child->visit + FLT_EPSILON));

		double uct_score = uct_exploitation + uct_k * uct_exploration;

		if (uct_score > best_utc_score) {

			best_utc_score = uct_score;
			best_node = child;
		}

	}


	return best_node;
}

UCTNode* UCT::expand(UCTNode * p, playerInfo& activePlayer)
{
	if (p->action.size() == 0)
	{
		for (int i = 0; i < BoardWidth; i++)
		{
			if (board[0][i] != '*') continue;
			p->action.push_back(i);
		}
		std::random_shuffle(p->action.begin(), p->action.end());
	}

	UCTNode child;
	child.index = p->action[p->child.size()];
	child.dn = 0;
	child.pn = 0;
	child.visit = 0;
	nodeNum++;
	p->child.push_back(child);
	return &p->child.back();
}


double UCT::search(UCTNode * p, playerInfo& activePlayer)
{
	p->visit += 1;
	if (CheckFour(board, *playerAI) == 1 || CheckFour(board, *playerHuman) == 1 || FullBoard(board) == BoardWidth)
	{
		p->dn = MaxValue;
		return -MaxValue;
	}
	
	UCTNode* c = NULL;
	if (p->action.size() == 0 || p->child.size() < p->action.size())
	{
		c = expand(p, activePlayer);
	}
	else
	{
		c = select(p);
	}

	double reward = 0.0;
	setMove(c->index, activePlayer, board);
	if (c->visit == 0)
	{
		/*if (CheckFour(board, *playerAI) == 1 || CheckFour(board, *playerHuman) == 1 || FullBoard(board) == BoardWidth)
		{
			c->dn = MaxValue;
			reward = MaxValue;
		}
		else
		{*/
		for (int i = 0; i < MaxSimulationNum; i++)
		{
			if (simulate(*changeSide(&activePlayer)))
			{
				if (&activePlayer == playerAI)
					reward += -1;
				else
					reward += 1;
			}
			else
			{
				reward += 0;
			}
		}

		c->visit += 1;
		c->dn += -reward;
		c->pn = c->dn / c->visit / MaxSimulationNum;
		p->dn += reward;
		p->pn = p->dn / p->visit / MaxSimulationNum;
		delMove(c->index, activePlayer, board);
		return reward;
	}
	reward = -search(c, *changeSide(&activePlayer));
	delMove(c->index, activePlayer, board);
	
	if (reward == MaxValue)
	{
		p->dn = -MaxValue;
		return reward;
	}
	else if (reward == -MaxValue)
	{
		for (list<UCTNode>::iterator i = p->child.begin(); i != p->child.end(); i++)
		{
			if (i->dn != reward)
			{
				reward = 0;
				p->dn += reward;
				p->pn = p->dn / p->visit / MaxSimulationNum;
				return reward;
			}

		}
		p->dn = MaxValue;
		return reward;
	}

	p->dn += reward;
	p->pn = p->dn / p->visit / MaxSimulationNum;
	return reward;
}