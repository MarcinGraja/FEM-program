#pragma strict_gs_check(on)
#include "element.h"
#include "gridGenerator.h"
#include <iostream>
#include <string>
#include "data.h"
#define N1 (1 - xi)*(1 - eta) * 0.25
#define N2 (1 - xi)*(1 - eta) * 0.25
#define N3 (1 - xi)*(1 - eta) * 0.25
#define N4 (1 - xi)*(1 - eta) * 0.25
data *element::globalData;
element::element(int id, std::vector<node*> nodes)
{
	this->id = id;
	this->nodes = nodes;
}
void element::print()
{
	std::string s = "id:" + std::to_string(id) + "\tnodes:";
	for (node *n : this->nodes)
	{
		s += "\t" + std::to_string(n->id);
	}
	s += '\n';
	s += "H:\n";
	s += this->getH().toString();
	std::cout << s;
}
double ** element::getJacobian(double xi, double eta)
{
	double **jacobian = new double*[2];
	jacobian[0] = new double[2];
	jacobian[1] = new double[2];
	double x[4], y[4];
	for (int i = 0; i < 4; i++)
	{
		x[i] = nodes.at(i)->x;
		y[i] = nodes.at(i)->y;
	}
	// dx/dxi
	jacobian[0][0] = 0.25 * eta * (x[0] - x[1] + x[2] - x[3]) + 0.25 * (-x[0] + x[1] + x[2] - x[3]);
	// dy/dxi
	
	jacobian[0][1] = 0.25 * eta * (y[0] - y[1] + y[2] - y[3]) + 0.25 * (-y[0] + y[1] + y[2] - y[3]);
	// dx/deta
	jacobian[1][0] = 0.25 * xi * (x[0] - x[1] + x[2] - x[3]) + 0.25 * (-x[0] - x[1] + x[2] + x[3]);
	// dy/deta
	jacobian[1][1] = 0.25 * xi * (y[0] - y[1] + y[2] - y[3]) + 0.25 * (-y[0] - y[1] + y[2] + y[3]);
	return jacobian;
}
double element::getJacobianDeterminant(double xi, double eta)
{
	double **jacobian = getJacobian(xi, eta);
	return (jacobian[0][0] * jacobian[1][1] - jacobian[0][1] * jacobian[1][0]);
}
matrix cachedInverseJacobian2;
int count = 0;
double ** element::getInverseJacobian(double xi, double eta)
{
	{
		count++;
		if (count == 17 || count == 16) {
			std::cout << "im tired\n";
		}
		double **returned = this->getJacobian(xi, eta);
		double matrixDeterminant = returned[0][0] * returned[1][1] - returned[0][1] * returned[1][0];
		double tmp = returned[0][0];
		returned[0][0] = returned[1][1] / matrixDeterminant;
		returned[0][1] /= -matrixDeterminant;
		returned[1][0] /= -matrixDeterminant;
		returned[1][1] = tmp / matrixDeterminant;
		if (cachedInverseJacobian2.getRows() == 0) 
			cachedInverseJacobian2 = matrix(returned, 2, 2);
		else if (false &&!(cachedInverseJacobian2 == matrix(returned, 2, 2)))
		{
			std::cout << cachedInverseJacobian2.toString() << matrix(returned, 2, 2).toString();
			std::cout << "jacobian changed\n";
		}
		return returned;
	}
}
double element::getShapeFunctionValue(double xi, double eta, int index)
{

	switch (index)
	{
	case 1:
		return N1;
	case 2:
		return N2;
	case 3:
		return N3;
	case 4:
		return N4;

	default:
		throw "invalid shape function index\n";
	}
}
matrix cachedInverseJacobian;
std::vector<matrix> element::getShapeFunctionDerivatives(double xi, double eta)
{
	std::vector<std::vector<double>>dNdXidEta;
	dNdXidEta.resize(4);
	for (int i = 0; i < 4; i++)
	{
		dNdXidEta.at(i).resize(2);
	}
	matrix inverseJacobian(getInverseJacobian(xi, eta), 2, 2);
	if (cachedInverseJacobian.getRows() == 0)
	{
		cachedInverseJacobian = inverseJacobian;
	}
	else if (false &&!(cachedInverseJacobian == inverseJacobian))
	{
		std::cout << cachedInverseJacobian.toString() << inverseJacobian.toString();
		std::cout << "inverse jacobian changed\n";
	}
	//dN/dXi

	dNdXidEta.at(0).at(0) = -0.25 *(1 - eta);
	dNdXidEta.at(1).at(0) = 0.25 * (1 - eta);
	dNdXidEta.at(2).at(0) = 0.25 * (1 + eta);
	dNdXidEta.at(3).at(0) = -0.25 * (1 + eta);

	//dN/dEta
	dNdXidEta.at(0).at(1) = -0.25 * (1 - xi);
	dNdXidEta.at(1).at(1) = -0.25 * (1 + xi);
	dNdXidEta.at(2).at(1) = 0.25 * (1 + xi);
	dNdXidEta.at(3).at(1) = 0.25 * (1 - xi);
	std::vector<matrix> out;
	for (int i = 0; i < 4; i++)
	{
		matrix tmp = matrix(dNdXidEta.at(i)).transpose();
		out.push_back(inverseJacobian * tmp);
	}
	
	return out;
}
std::vector <matrix> cachedShapeFunctionDerivatives[4];
matrix element::getH()
{
	H = matrix(4, 4);
	for (int k = 0; k < globalData->integrationPoints.size(); k++) {
		integrationPoint p = globalData->integrationPoints.at(k);
		std::vector <matrix> shapeFunctionDerivatives = getShapeFunctionDerivatives(p.xi, p.eta);
		//std::cout << "calculating at " << p.xi << "," << p.eta << '\n';
		if (cachedShapeFunctionDerivatives[k].size() == 0)
		{
			cachedShapeFunctionDerivatives[k] = shapeFunctionDerivatives;
		}
		else if (false)
		{
			for (int i = 0; i < 4; i++) {
				std::cout  << cachedShapeFunctionDerivatives[k].at(i).toString() << shapeFunctionDerivatives.at(i).toString() << "\n\n\n";
			}
		}
		double jacobianDeterminant = getJacobianDeterminant(p.xi, p.eta);
		double dNdX[4];
		double dNdY[4];
		for (int i = 0; i < 4; i++)
		{
			dNdX[i] = shapeFunctionDerivatives.at(i).getValue(0, 0);
			dNdY[i] = shapeFunctionDerivatives.at(i).getValue(1, 0);
		}
		
		matrix d1(dNdX, 4);
		matrix d2(dNdY, 4);
		H = H +  (d1 * d1.transpose() + d2*d2.transpose()) * jacobianDeterminant;
	}
	H = H * globalData->k;
	return H;
}
node::node(int id, double t, double x, double y, bool boundaryCondition)
{
	this->id = id;
	this->t = t;
	this->x = x;
	this->y = y;
	this->boundaryCondition = boundaryCondition;
}

void node::print()
{
	std::cout << "id: " << id << "\t t: " << t << "\t x: " << x << "\t y: " << y << "\tboundary condition: " << boundaryCondition << '\n';
}
