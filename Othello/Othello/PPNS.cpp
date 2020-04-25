#include "stdafx.h"


#include "PPNS.h"
#include <string>
#include "FileOperator.h"

void PPNS::solutionTreeShape(double *b, int *d) {
	bool has_child = true;

	Node *temp = root;
	double total_branch = 0;
	int total_depth = 0;
	int max_depth = 1500;

	if (temp == NULL|| temp->child.empty() == true)has_child = false;
	while (has_child == true && total_depth<max_depth) {
		double maxPN = temp->child.at(0).getPN();
		Node *MPN = temp;
		total_branch += temp->child.size();
		total_depth++;
		//max nodes
		for (int i = 1; i < temp->child.size(); i++) {
			if (temp->child.at(i).getPN() > maxPN) {
				maxPN = temp->child.at(i).getPN();
				MPN = &(temp->child.at(i));
			}
		}

		if (MPN->child.empty() == true) has_child = false;

		else {
			//min nodes
			temp = MPN;
			maxPN = temp->child.at(0).getPN();
			total_branch += temp->child.size();
			total_depth++;
			for (int i = 1; i < temp->child.size(); i++) {
				if (temp->child.at(i).getPN() < maxPN) {
					maxPN = temp->child.at(i).getPN();
					MPN = &(temp->child.at(i));
				}
			}
		}

		if (MPN->child.empty() == true) has_child = false;
	}

	*b = total_branch / total_depth;
	*d = total_depth;
}
int PPNS::startSolver(board &ncurrentBoard) {

	cout << "PPNS::startSolver() START [solved=" << solved << "]\n";
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
		clock_t start = clock();
		while (1)
		{
			//cout << "iteration #" << itnum << endl;
			itnum++;
			clock_t end = clock();
			double time_elapsed = ((double)(end - start)) / CLOCKS_PER_SEC;
			double PN = root->getPN();
			if (itnum % 100000 == 0) {
				std::cout << "new iteration" << itnum << " PN = ";
				std::cout << PN << "\nDN = ";
				std::cout << root->getDN() << endl << endl;

				//if (fabs(PN - 1) <= (0.00001 * fabs(PN)))cout << "PN = 1!";
			}
			if (nodeNum >= 8000000 || time_elapsed>=600) {
				std::string result;
				result.append("outofresource= ");
				result.append(std::to_string(itnum));
				result.append(" ");
				result.append(std::to_string(nodeNum)); 
				result.append(" ");
				result.append(std::to_string(time_elapsed));
				result.append(" ");
				result.append(std::to_string(root->getPN()));
				result.append(" ");
				result.append(std::to_string(root->getDN()));
				result.append(";\n");
				writeIntoFile("PPNSresult.txt", result);
				return 0;
			}
			thisPlayer = currentBoard->curPlayer;
			search(root, thisPlayer, currentPosition, *currentBoard);
			if (fabs(root->getPN() - 1) <= (epsilon * fabs(root->getPN())))
			{
				//root->printChild();
				solved = true;
				curNode = best;
				cout << "solved" << endl;
				cout << "Iteration number: " << itnum << endl;
				cout << "Node number: " << nodeNum << endl;
				cout << "PN " << root->getPN() << endl;
				cout << "DN" << root->getDN() << endl;
				std::string result;
				result.append("solved= ");
				result.append(std::to_string(itnum));
				result.append(" ");
				result.append(std::to_string(nodeNum));
				result.append(" ");
				result.append(std::to_string(time_elapsed));
				result.append(" ");
				result.append(std::to_string(root->getPN()));
				result.append(" ");
				result.append(std::to_string(root->getDN()));
				result.append(";\n");
				writeIntoFile("PPNSresult.txt", result);
				if (best == NULL)return 0;
				else return best->getIndex();
			}
			else if (fabs(root->getDN() - 1) <= (epsilon * fabs(root->getDN())))
			{
				cout << "unsolved" << endl;
				cout << "Iteration number: " << itnum << endl;
				cout << "Node number: " << nodeNum << endl;
				cout << "PN " << root->getPN()<<endl;
				cout << "DN" << root->getDN() << endl;
				std::string result;
				result.append("unsolved= ");
				result.append(std::to_string(itnum));
				result.append(" ");
				result.append(std::to_string(nodeNum));
				result.append(" ");
				result.append(std::to_string(time_elapsed));
				result.append(" ");
				result.append(std::to_string(root->getPN()));
				result.append(" ");
				result.append(std::to_string(root->getDN()));
				result.append(";\n");
				writeIntoFile("PPNSresult.txt", result);
				return 0;
			}
		}
	}
}
void PPNS::getPNDN(Node *p, int &thisPlayer, board &b) {

	int AI = 0;
	int human = 1;
	vector<position> tmp;
	//reinforce the player
	b.curPlayer = thisPlayer;
	int win = b.getStatus(tmp);
	//cout << win<<endl;
	if (win == 1) { //computer wins
		//cout << thisPlayer<<" wins!\n";
		//b.outputBoard();
		p->setPN(1.0);
		p->setDN(0.0);
		return;
	}
	if (win == 2||win==0) { //human wins or draw
		/*cout << "computer loses or draw!\n";
		b.outputBoard();*/
		//cout << thisPlayer << " wins!\n";
		//b.outputBoard();
		p->setPN(0.0);
		p->setDN(1.0);
		return;
	}


	int wnum = 0;
	wnum = currentBoard->getMonteCarloWeight(*currentBoard);
	//cout << "wnum = "<<wnum << endl;

	if (wnum == 0)
	{
		p->setPN(0.01);
		p->setDN(0.99);
		return;
	}
	else if (wnum == MaxSimulationNum)
	{
		p->setPN(0.99);
		p->setDN(0.01);
		return;
	}

	p->setPN((double)wnum / MaxSimulationNum);
	p->setDN(1.0 - p->getPN());
}
void PPNS::search(Node*p, int &thisPlayer, vector<position> &currentPositions, board &b) {

	//reinforce current Player
	currentBoard->curPlayer = thisPlayer;
	//determine opposite player
	int oppositePlayer = abs(1 - thisPlayer);

	if (p->child.size() == 0)
	{
		//cout << "LEAF NODE" << endl;
		double andp = 1.0;
		double orp = 1.0;

		// Count the number of unused spaces on the board
		vector<position> unused_spaces; //unused_spaces = only valid spaces in the board
		currentBoard->getAllValidMoves(unused_spaces);
		//std::cout << unused_spaces_count << endl;

		//cout << "branching factor : "<<unused_spaces.size() << endl;

		for (int i = 0; i < unused_spaces.size(); i++) {
			Node c = new Node(0, 0);
			int index = (unused_spaces.at(i).y * b.width) + unused_spaces.at(i).x; //index = x y encoded into linear
			c.setIndex(index);
			//temporary objects for easier undo
			board temp(*currentBoard);
			int tempPlayer = thisPlayer;
			if (currentBoard->placeOn(unused_spaces.at(i), tempPlayer) == true) {
				getPNDN(&c, oppositePlayer, *currentBoard);
				currentBoard->copy(temp);
			}
			nodeNum++;

			p->child.push_back(c);

			andp *= c.getPN();
			orp *= c.getDN();
		}

		if (thisPlayer == 0)
		{
			p->setPN(1.0 - orp);
			p->setDN(1.0 - p->getPN());
		}
		else
		{
			p->setPN(andp);
			p->setDN(1.0 - p->getPN());
		}
		return;
	}

	//cout << "NON-ROOT NODE" << endl;
	//cout << "thisPlayer is ";
	/*if (thisPlayer == P_BLACK) {
		cout << "black" << endl;
	}
	else {
		cout << "white" << endl;
	}*/

	if (thisPlayer == 0)
	{
		//cout << "entered black"<<endl;
		Node* c = NULL;
		int chosen;
		double maxpn = -MaxValue - 1;

		for (int i = 0; i < p->child.size(); i++) {
			if (p->child.at(i).getPN() == 0.0)continue;
			if (p->child.at(i).getPN() > maxpn) {
				maxpn = p->child.at(i).getPN();
				chosen = i;
			}
		}

		position pos;
		pos.x = p->child.at(chosen).getIndex() % currentBoard->width;
		pos.y = (p->child.at(chosen).getIndex() / currentBoard->height);
		board simulatedBoard(*currentBoard);
		if (currentBoard->placeOn(pos, thisPlayer) == true) {
			search(&(p->child.at(chosen)), thisPlayer, currentPositions, *currentBoard);
			currentBoard->copy(simulatedBoard);
		}
		else {
			cout << "gagal\n";
			thisPlayer = oppositePlayer;
			return;
		}

		double andp = 1.0;
		double orp = 1.0;
		maxpn = -MaxValue - 1;

		for (int i = 0; i < p->child.size(); i++) {

			andp *= p->child.at(i).getPN();
			orp *= p->child.at(i).getDN();
			if (p->child.at(i).getPN() > maxpn) {
				maxpn = p->child.at(i).getPN();
				best = &(p->child.at(i));
			}
		}

		p->setPN(1.0 - orp);
		p->setDN(1.0 - p->getPN());
		return;
	}
	else
	{
		//cout << "entered white" << endl;
		Node* c = NULL;
		double maxdn = -MaxValue - 1;
		int chosen = -1;
		for (int i = 0; i < p->child.size(); i++) {
			if (p->child.at(i).getPN() == 0.0)continue;
			if (p->child.at(i).getDN() > maxdn) {
				maxdn = p->child.at(i).getDN();
				chosen = i;
			}
		}

		position pos;
		pos.x = p->child.at(chosen).getIndex() % currentBoard->width;
		pos.y = (p->child.at(chosen).getIndex() / currentBoard->height);
		board simulatedBoard(*currentBoard);
		if (currentBoard->placeOn(pos, thisPlayer) == true) {
			search(&(p->child.at(chosen)), thisPlayer, currentPositions, *currentBoard);
			currentBoard->copy(simulatedBoard);
		}
		else {
			cout << "gagal!\n";
			thisPlayer = oppositePlayer;
			return;
		}

		double andp = 1.0;
		double orp = 1.0;

		for (int i = 0; i < p->child.size(); i++) {

			andp *= p->child.at(i).getPN();
			orp *= p->child.at(i).getDN();
		}
		p->setPN(andp);
		p->setDN(1.0 - p->getPN());

		return;
	}

	return;
}

int PPNS::getlastMove() {
	return lastMove;
}

void PPNS::drawCurrentBoard() {
	currentBoard->outputBoard();
}