#include "stdafx.h"

#include "PNS.h"
#include <fstream>
#include <string>
#include "FileOperator.h"

int PNS::startSolver(board &ncurrentBoard) {

	cout << "PNS::startSolver() START [solved=" << solved << "]\n";
	currentBoard = new board(ncurrentBoard);
	vector<position> currentPosition;
	currentBoard->getStatus(currentPosition);
	//currentGraph = new hexGraph(ncurrentGraph);
	int thisPlayer = currentBoard->curPlayer;

	if (solved)
	{
		cout << "solved=TRUE" << endl;
		for (vector<Node>::iterator i = curNode->getChild().begin(); i != curNode->getChild().end(); i++)
		{
			if (i->getIndex() == lastMove)
			{
				curNode = &(*i);
				break;
			}
		}

		for (vector<Node>::iterator i = curNode->getChild().begin(); i != curNode->getChild().end(); i++)
		{
			if (i->getPN() == 0)
			{
				curNode = &(*i);
				return i->getIndex();
			}
		}

	}
	else
	{
		cout << "solved=FALSE" << endl;
		int itnum = 0;
		//std::cout << itnum << " ";
		while (1)
		{
			//cout << "iteration #" << itnum << endl;
			itnum++;
			if (itnum % 100000 == 0) {
				std::cout << "new iteration" << itnum << " ";
				std::cout << root->getPN() << " ";
				std::cout << root->getDN() << endl << endl;

			}
			if (nodeNum >= 8000000) { 
				std::string result;
				result.append("outofresource= ");
				result.append(std::to_string(itnum));
				result.append(" ");
				result.append(std::to_string(nodeNum));
				result.append(" ");
				result.append(std::to_string(root->getPN()));
				result.append(" ");
				result.append(std::to_string(root->getDN()));
				result.append(";\n");
				writeIntoFile("PNSresult.txt", result);
				
				return 0; 
			}
			thisPlayer = currentBoard->curPlayer;
			search(root, thisPlayer, currentPosition,*currentBoard);
			if (root->getPN() == 0)
			{
				//root->printChild();
				solved = true;
				curNode = best;
				cout << "solved" << endl;
				cout << "Iteration number: " << itnum << endl;
				cout << "Node number: " << nodeNum << endl;

				std::string result;
				result.append("solved= ");
				result.append(std::to_string(itnum));
				result.append(" ");
				result.append(std::to_string(nodeNum));
				result.append(" ");
				result.append(std::to_string(root->getPN()));
				result.append(" ");
				result.append(std::to_string(root->getDN()));
				result.append(";\n");
				writeIntoFile("PNSresult.txt", result);

				if (best == NULL)return 0;
				else return best->getIndex();
			}
			else if (root->getDN() == 0)
			{
				cout << "unsolved" << endl;
				cout << "Iteration number: " << itnum << endl;
				cout << "Node number: " << nodeNum << endl;
				std::string result;
				result.append("unsolved= ");
				result.append(std::to_string(itnum));
				result.append(" ");
				result.append(std::to_string(nodeNum));
				result.append(" ");
				result.append(std::to_string(root->getPN()));
				result.append(" ");
				result.append(std::to_string(root->getDN()));
				result.append(";\n");
				writeIntoFile("PNSresult.txt", result);
				return 0;
			}
		}
	}
}
void PNS::getPNDN(Node *p, int &thisPlayer,board &b) {

	int AI = 0;
	int human = 1;
	vector<position> tmp;
	//reinforce the player
	b.curPlayer = thisPlayer;
	int win = b.getStatus(tmp);
	b.curPlayer = thisPlayer;
	if (win == 1) { //computer wins
			//cout << "computer wins!\n";
			//b.outputBoard();
			p->setPN(0);
			p->setDN(MaxValue);
			return;
	}
	if (win==2) { //human wins
			//cout << "computer loses!\n";
			//b.outputBoard();
			p->setPN(MaxValue);
			p->setDN(0);
			return;
	}
	if (win==0) {//draw
		p->setPN(MaxValue);
		p->setDN(0);
		return;
	}
	/*currentBoard->outputBoard();*/
	p->setPN(1);
	p->setDN(1);
}
void PNS::search(Node*p, int &thisPlayer, vector<position> &currentPositions, board &b) {

	//reinforce current Player
	currentBoard->curPlayer = thisPlayer;
	//determine opposite player
	int oppositePlayer = thisPlayer == 0 ? 1 : 0;

	if (p->child.size() == 0)
	{
		//cout << "LEAF NODE of "<<thisPlayer << endl;
		double sumpn = 0;
		double sumdn = 0;
		double minpn = MaxValue + 1;
		double mindn = MaxValue + 1;

		// Count the number of unused spaces on the board
		vector<position> unused_spaces; //unused_spaces = only valid spaces in the board
		currentBoard->getAllValidMoves(unused_spaces);
		//currentBoard->outputBoard();

		for(int i=0;i<unused_spaces.size();i++){
			Node c = new Node(0, 0);
			int index = (unused_spaces.at(i).y * b.width) + unused_spaces.at(i).x; //index = x y encoded into linear
			c.setIndex(index);
			//temporary objects for easier undo
			board temp(*currentBoard);
			int tempPlayer = thisPlayer;
			//cout << tempPlayer << "to be put on : " << unused_spaces.at(i).x << "," << unused_spaces.at(i).y << endl;
			if (currentBoard->placeOn(unused_spaces.at(i), tempPlayer)==true) {
				getPNDN(&c, oppositePlayer, *currentBoard);
				currentBoard->copy(temp);
			}
			//currentBoard->outputBoard();
			//delete &simulatedBoard;

			nodeNum++;
			p->child.push_back(c);

			if (c.getPN() < minpn)minpn = c.getPN();
			if (c.getDN() < mindn)mindn = c.getDN();

			c.getPN() + sumpn >= MaxValue ? sumpn = MaxValue : sumpn += c.getPN();
			c.getDN() + sumdn >= MaxValue ? sumdn = MaxValue : sumdn += c.getDN();
		}
		//thisPlayer shouldn't be changed. After expansion continue search with the same player.

		if (thisPlayer == 0) //current player = AI
		{
			p->setPN(minpn);
			p->setDN(sumdn);
		}
		else
		{
			p->setPN(sumpn);
			p->setDN(mindn);
		}
		return;
	}

	
	if (thisPlayer == 0)
	{
		//cout << "entered AI turn"<<endl;
		//cout<<"amount of child "<<p->child.size()<<endl;
		Node* c = NULL;
		int chosen;
		double minpn = MaxValue + 1;

		for (int i = 0; i < p->child.size(); i++) {
			if (p->child.at(i).getPN() < minpn) {
				minpn = p->child.at(i).getPN();
				chosen = i;
			}
		}

		//decode from linear index to x y
		position pos;
		pos.x = p->child.at(chosen).getIndex() % currentBoard->width;
		pos.y = (p->child.at(chosen).getIndex()/ currentBoard->height);
		board simulatedBoard(*currentBoard);
		//cout << thisPlayer << "to be placed on " << pos.x << "," << pos.y << endl;
		if (currentBoard->placeOn(pos, thisPlayer) == true) {
			//currentBoard->outputBoard();
			search(&(p->child.at(chosen)), thisPlayer, currentPositions, *currentBoard);
			currentBoard->copy(simulatedBoard);
		}
		else {
			cout << "gagal\n";
			//cout << thisPlayer << "to be placed on " << pos.x << "," << pos.y << endl;
			//currentBoard->outputBoard();
			thisPlayer = oppositePlayer;
			return;
		}

		double sumdn = 0;
		minpn = MaxValue + 1;

		for (int i = 0; i < p->child.size(); i++) {

			if (p->child.at(i).getPN() < minpn) {
				minpn = p->child.at(i).getPN();
				best = &(p->child.at(i));
			}
			sumdn += p->child.at(i).getDN();
		}

		sumdn >= MaxValue ? p->setDN(MaxValue) : p->setDN(sumdn);
		p->setPN(minpn);
		return;
	}
	else
	{
		//cout << "entered human turn" << endl;
		Node* c = NULL;
		double mindn = MaxValue + 1;
		int chosen = -1;
		for (int i = 0; i < p->child.size(); i++) {
			if (p->child.at(i).getDN() < mindn) {
				mindn = p->child.at(i).getDN();
				chosen = i;
			}
		}

		position pos;
		pos.x = p->child.at(chosen).getIndex() % currentBoard->width;
		pos.y = (p->child.at(chosen).getIndex() / currentBoard->height);
		board simulatedBoard(*currentBoard);
		//cout << thisPlayer << "to be placed on " << pos.x << "," << pos.y << endl;
		if (currentBoard->placeOn(pos, thisPlayer)==true) {
			//currentBoard->outputBoard();
			search(&(p->child.at(chosen)), thisPlayer, currentPositions, *currentBoard);
			currentBoard->copy(simulatedBoard);
		}else {
			cout << "gagal!\n";
			//cout << thisPlayer<<"to be placed on "<<pos.x<<","<<pos.y<<endl;
			//currentBoard->outputBoard();
			thisPlayer = oppositePlayer;
			return;
		}

		mindn = MaxValue + 1;
		double sumpn = 0;

		for (int i = 0; i < p->child.size(); i++) {

			if (p->child.at(i).getDN() < mindn) {
				mindn = p->child.at(i).getDN();
			}
			sumpn += p->child.at(i).getPN();
		}
		sumpn >= MaxValue ? p->setPN(MaxValue) : p->setPN(sumpn);
		p->setDN(mindn);
		return;
	}

	return;
}

int PNS::getlastMove() {
	return lastMove;
}

void PNS::drawCurrentBoard() {
	currentBoard->outputBoard();
}