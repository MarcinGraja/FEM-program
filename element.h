#pragma once
#include <vector>
#include "matrix.h"
#include "data.h"
struct node
{
	int id;
	double x;
	double y;
	double t;
	bool boundaryCondition;
	node(int id, double t, double x, double y, bool boundaryCondition);
	void print();
	
};
class element
{
	static data *globalData;
	int id;
	std::vector <node*> nodes;
	matrix H = matrix(4,4) ;
	matrix Hbc = matrix(4,4);
	matrix P = matrix(4,1);
	friend class gridGenerator;
	element(int id, std::vector<node*> nodes);
	matrix getHbc();
	void print();
public:
	node *getNode(int i);
	matrix getH();
	matrix getC();

	matrix getP();
	static void setDataCopy(data *d)
	{
		globalData = d;
	}
};


