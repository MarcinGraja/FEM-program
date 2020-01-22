#pragma once
#include <vector>
#include "matrix.h"
#include "data.h"
class node
{
	int id;
	double t;
	double x;
	double y;
	bool boundaryCondition;
	friend class gridGenerator;
	friend class element;
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
	void print();
public:
	double ** getJacobian(double xi, double eta);
	double getJacobianDeterminant(double xi, double eta);
	double ** getInverseJacobian(double xi, double eta);
	double getShapeFunctionValue(double xi, double eta, int index);
	std::vector<matrix> getShapeFunctionDerivatives(double xi, double eta);
	matrix getH();
	static void setDataCopy(data *d)
	{
		globalData = d;
	}
};


