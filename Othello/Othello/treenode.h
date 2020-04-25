#pragma once

#ifndef TREENODE_H
#define	TREENODE_H

#include <iostream>
#include <vector>
#include <list>
const int MAX = 10000000;
#define MaxValue 1000000000000
#define MaxSimulationNum 60
#define epsilon 0.00001

using namespace std;

class Node {
public:
	Node(bool isROOT);
	Node(double nPN, double nDN);
	double getPN();
	double getDN();
	void setPN(double nPN);
	void setDN(double nDN);
	bool isRoot();
	void addChild(Node *child);
	vector<Node> getChild();
	Node &getChildIndex(int i);
	void setIndex(int nIndex);
	int getIndex();
	void printChild();
	vector<Node> child;
private:
	int index;
	double PN;
	double DN;
	bool ROOT;
};

#endif /* TREENODE_H */

