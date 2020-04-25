#include "stdafx.h"
#include <stack>
#include <queue>
#include <vector>
#include <iostream>
#include <algorithm>
#include <assert.h>
#include <thread>
#include <vector>
#include <iostream>
#include <assert.h>
#include <functional>
#include <string>
#include "treeNode.h"


using namespace std;

Node::Node(bool isROOT) {
	if (isROOT) {
		ROOT = true;
		PN = 0;
		DN = 0;
		child.clear();
	}
}
Node::Node(double nPN, double nDN) {
	ROOT = false;
	PN = nPN;
	DN = nDN;
	child.clear();
}
double Node::getPN() {
	return PN;
}
double Node::getDN() {
	return DN;
}
void Node::setPN(double nPN) {
	PN = nPN;
}

void Node::setDN(double nDN) {
	DN = nDN;
}

bool Node::isRoot() {
	return ROOT;
}
void Node::addChild(Node *nChild) {
	child.push_back(*nChild);
}
vector<Node> Node::getChild() {
	return child;
}

Node &Node::getChildIndex(int i) {
	return this->child.at(i);
}

void Node::setIndex(int nIndex) {
	index = nIndex;
}
int Node::getIndex() {
	return index;
}

void Node::printChild() {
	//cout << "\nnode child";
	for (int i = 0; i < this->child.size(); i++) {
		cout << this->child.at(i).getIndex() << " ";
		if (child.at(i).child.size() > 0) {
			cout << "\t|\t";
			child.at(i).printChild();
		}
	}
	cout << endl;
}