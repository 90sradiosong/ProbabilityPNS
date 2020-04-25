#include "stdafx.h"

#include "DFPNS.h"
#include <string>
#include <algorithm>

int DFPNS::startSolver(board &ncurrentBoard) {

	cout << "DFPNS::startSolver() START [solved=" << solved << "]\n";
	currentBoard = new board(ncurrentBoard);
	vector<position> currentPosition;
	currentBoard->getStatus(currentPosition);
	//currentGraph = new hexGraph(ncurrentGraph);
	int thisPlayer = currentBoard->curPlayer;

	if (solved)
	{
		cout << "solved=TRUE" << endl;
		for (vector<DFPNode>::iterator i = curNode->child.begin(); i != curNode->child.end(); i++)
		{
			if (i->n->getIndex() == lastMove)
			{
				curNode = &(*i);
				break;
			}
		}

		for (vector<DFPNode>::iterator i = curNode->child.begin(); i != curNode->child.end(); i++)
		{
			if (i->n->getPN() == 0)
			{
				curNode = &(*i);
				return i->n->getIndex();
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
			if (itnum % 1000 == 0) {
				std::cout << "new iteration" << itnum << " ";
				std::cout << root->n->getPN() << " ";
				std::cout << root->n->getDN() << endl << endl;
				this->table.showTableEntries();

			}
			//if (itnum == 10000)return 0;
			thisPlayer = currentBoard->curPlayer;
			search(root, thisPlayer, currentPosition, *currentBoard);
			if (root->n->getPN() >=root->thpn)
			{
				//root->printChild();
				solved = true;
				curNode = best;
				cout << "solved" << endl;
				cout << "Iteration number: " << itnum << endl;
				cout << "Node number: " << nodeNum << endl;
				if (best == NULL)return 0;
				else return best->n->getIndex();
			}
			else if (root->n->getDN() >= root->thdn)
			{
				cout << "unsolved" << endl;
				cout << "Iteration number: " << itnum << endl;
				cout << "Node number: " << nodeNum << endl;
				return 0;
			}
		}
	}
}
void DFPNS::getPNDN(DFPNode *p, int &thisPlayer, board &b) {

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
		p->n->setPN(0);
		p->n->setDN(MaxValue);
		if(table.TTLookup(&b)==-1)this->table.TTStore(p, &b);
		return;
	}
	if (win == 2) { //human wins
			//cout << "computer loses!\n";
			//b.outputBoard();
		p->n->setPN(MaxValue);
		p->n->setDN(0);
		if (table.TTLookup(&b) == -1)this->table.TTStore(p, &b);
		return;
	}
	if (win == 0) {//draw
		p->n->setPN(MaxValue);
		p->n->setDN(0);
		if (table.TTLookup(&b) == -1)this->table.TTStore(p, &b);
		return;
	}
	/*currentBoard->outputBoard();*/
	p->n->setPN(1);
	p->n->setDN(1);
}
void DFPNS::search(DFPNode*p, int &thisPlayer, vector<position> &currentPositions, board &b) {
	//reinforce current Player
	currentBoard->curPlayer = thisPlayer;
	//determine opposite player
	int oppositePlayer = thisPlayer == 0 ? 1 : 0;

	TransEntry res;
	int searchRes;
	searchRes = this->table.TTLookup(&b);

	if (searchRes>-1) {
		cout << "search result = " << searchRes<<endl;
		res = table.entries.at(searchRes);
		if (res.entry->n->getPN() <= p->thpn || res.entry->n->getDN() <= p->thdn) {
			res.entry->n->setPN(p->thpn);
			res.entry->n->setDN(p->thdn);
			this->table.TTStore(p, &b);
			return;
		}
	}

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

		for (int i = 0; i < unused_spaces.size(); i++) {
			DFPNode c;
			c.n = new Node(0, 0);
			c.child.clear();
			int index = (unused_spaces.at(i).y * b.width) + unused_spaces.at(i).x; //index = x y encoded into linear
			c.n->setIndex(index);
			//temporary objects for easier undo
			board temp(*currentBoard);
			int tempPlayer = thisPlayer;
			//cout << tempPlayer << "to be put on : " << unused_spaces.at(i).x << "," << unused_spaces.at(i).y << endl;
			if (currentBoard->placeOn(unused_spaces.at(i), tempPlayer) == true) {
				getPNDN(&c, oppositePlayer, *currentBoard);

				nodeNum++;
				p->child.push_back(c);

				if (c.n->getPN() < minpn)minpn = c.n->getPN();
				if (c.n->getDN() < mindn)mindn = c.n->getDN();

				c.n->getPN() + sumpn >= MaxValue ? sumpn = MaxValue : sumpn += c.n->getPN();
				c.n->getDN() + sumdn >= MaxValue ? sumdn = MaxValue : sumdn += c.n->getDN();


				//store new nodes to transposition table
				table.TTStore(&c, currentBoard);

				currentBoard->copy(temp);
			}
			//currentBoard->outputBoard();
			//delete &simulatedBoard;


		}
		//thisPlayer shouldn't be changed. After expansion continue search with the same player.

		if (thisPlayer == 0) //current player = AI
		{
			p->n->setPN(minpn);
			p->n->setDN(sumdn);
		}
		else
		{
			p->n->setPN(sumpn);
			p->n->setDN(mindn);
		}
		return;
	}


	if (thisPlayer == 0)
	{
		cout << "entered AI turn"<<endl;
		//cout<<"amount of child "<<p->child.size()<<endl;
		DFPNode* c = NULL;
		int chosen=-1;
		int chosen2=0;
		double mindn = MaxValue + 1;
		double mindn2 = MaxValue + 1;

		//select nc with minimum delta and n2 with second minimum delta
		for (int i = 0; i < p->child.size(); i++) {
			if (p->child.at(i).n->getDN() <= mindn) {
				if (chosen != -1) {
					mindn2 = mindn;
					chosen2 = chosen;
				}
				mindn = p->child.at(i).n->getDN();
				chosen = i;
			}
			else if (p->child.at(i).n->getDN() <= mindn2) {
				mindn2 = p->child.at(i).n->getDN();
				chosen2 = i;
			}
		}

		//decode from linear index to x y
		position pos;
		pos.x = p->child.at(chosen).n->getIndex() % currentBoard->width;
		pos.y = (p->child.at(chosen).n->getIndex() / currentBoard->height);
		board simulatedBoard(*currentBoard);
		//cout << thisPlayer << "to be placed on " << pos.x << "," << pos.y << endl;
		if (currentBoard->placeOn(pos, thisPlayer) == true) {
			//currentBoard->outputBoard();
			//calculate new threshold
			double sigmapn = 0;
			for (int i = 0; i < p->child.size(); i++) {
				sigmapn = sigmapn + p->child.at(i).n->getPN();
			}
			p->child.at(chosen).thpn = p->thdn + p->child.at(chosen).thpn - (sigmapn);
			p->child.at(chosen).thdn = std::min(p->n->getPN(), (p->child.at(chosen2).n->getDN() + 1));
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
		double minpn = MaxValue + 1;

		for (int i = 0; i < p->child.size(); i++) {

			if (p->child.at(i).n->getPN() < minpn) {
				minpn = p->child.at(i).n->getPN();
				best = &(p->child.at(i));
			}
			sumdn += p->child.at(i).n->getDN();
		}

		sumdn >= MaxValue ? p->n->setDN(MaxValue) : p->n->setDN(sumdn);
		p->n->setPN(minpn);
		return;
	}
	else
	{
		cout << "entered human turn" << endl;
		DFPNode* c = NULL;
		double minpn = MaxValue + 1;
		double minpn2 = MaxValue + 1;
		int chosen=-1;
		int chosen2=0;

		for (int i = 0; i < p->child.size(); i++) {
			if (p->child.at(i).n->getPN() <= minpn) {
				if (chosen != -1) {
					minpn2 = minpn;
					chosen2 = chosen;
				}
				minpn = p->child.at(i).n->getPN();
				chosen = i;
			}
			else if(p->child.at(i).n->getPN() <= minpn2) {
				minpn2 = p->child.at(i).n->getPN();
				chosen2 = i;
			}
		}

		position pos;
		pos.x = p->child.at(chosen).n->getIndex() % currentBoard->width;
		pos.y = (p->child.at(chosen).n->getIndex() / currentBoard->height);
		board simulatedBoard(*currentBoard);
		//cout << thisPlayer << "to be placed on " << pos.x << "," << pos.y << endl;
		if (currentBoard->placeOn(pos, thisPlayer) == true) {
			//currentBoard->outputBoard();
			double sigmadn = 0;
			for (int i = 0; i < p->child.size(); i++) {
				sigmadn = sigmadn + p->child.at(i).n->getDN();
			}
			p->child.at(chosen).thdn = p->thpn + p->child.at(chosen).thdn - (sigmadn);
			p->child.at(chosen).thdn = std::min(p->n->getDN(), (p->child.at(chosen2).n->getPN() + 1));
			search(&(p->child.at(chosen)), thisPlayer, currentPositions, *currentBoard);
			currentBoard->copy(simulatedBoard);
		}
		else {
			cout << "gagal!\n";
			//cout << thisPlayer<<"to be placed on "<<pos.x<<","<<pos.y<<endl;
			//currentBoard->outputBoard();
			thisPlayer = oppositePlayer;
			return;
		}

		double mindn = MaxValue + 1;
		double sumpn = 0;

		for (int i = 0; i < p->child.size(); i++) {

			if (p->child.at(i).n->getDN() < mindn) {
				mindn = p->child.at(i).n->getDN();
			}
			sumpn += p->child.at(i).n->getPN();
		}
		sumpn >= MaxValue ? p->n->setPN(MaxValue) : p->n->setPN(sumpn);
		p->n->setDN(mindn);
		return;
	}

	return;
}

int DFPNS::getlastMove() {
	return lastMove;
}

void DFPNS::drawCurrentBoard() {
	currentBoard->outputBoard();
}

bool TranspositionTable::CompareBoard(board &b1, board &b2) {
	for (int i = 0; i < b1.width; i++) {
		for (int j = 0; j < b1.height; j++) {
			if (b1.reversiBoard[i][j] != b2.reversiBoard[i][j])return false;
		}
	}
	return true;
}
int TranspositionTable::TTLookup(board *boardstate) {
	if (this->entries.size() > 0) {
		//cout << "search in table with size "<<entries.size()<<endl;
		for (int i = 0; i < this->entries.size(); i++) {
			if (CompareBoard(*boardstate, *(entries.at(i).entryBoard)) == true) {
				return i;
			}
		}
	}
	return -1;
}
bool TranspositionTable::TTStore(DFPNode *newentry, board *boardstate) {
	TransEntry temp;
	int res;

	res = TTLookup(boardstate);
	if (res != -1) { //override value
		//cout << "override table\n";
		entries.at(res).entry = newentry;
		return true;
	}
	else if (res == -1) { //new value, push back
		//cout << "storing new entry\n";
		temp.entry = newentry;
		temp.entryBoard = new board(*boardstate);
		this->entries.push_back(temp);
		//cout << "table size = " << entries.size()<<endl;
		return true;
	}

}

void TranspositionTable ::showTableEntries() {
	cout << "\n\ntransposition table : ";
	for (int i = 0; i < this->entries.size(); i++) {
		cout << "PN = "<<entries.at(i).entry->n->getPN();
		cout << "\tDN = " << entries.at(i).entry->n->getDN();
		cout << "\tthPN = " << entries.at(i).entry->thpn;
		cout << "\tthDN = " << entries.at(i).entry->thdn;
			cout << endl;
	}
	cout << "--------------------------------\n";
}