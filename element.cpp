#pragma strict_gs_check(on)
#include "element.h"
#include "gridGenerator.h"
#include <iostream>
#include <string>
#include "data.h"
#include "helperFunctions.h"
#include <cmath>
data *element::globalData;
element::element(int id, std::vector<node*> nodes)
{
	this->id = id;
	this->nodes = nodes;
}
node *element::getNode(int i)
{
	return nodes.at(i);
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
	s += "Hbc:\n";
	s += this->getHbc().toString();
	s += "C:\n" + this->getC().toString();
	s += "P:\n" + this->getP().toString();
	std::cout << s;
}

matrix element::getH()
{
	H = matrix(4, 4);
	for (integrationPoint p1 : globalData->integrationPoints)
	{
		for (integrationPoint p2 : globalData->integrationPoints) {
			std::vector <matrix> shapeFunctionsDerivatives = shapeFunctions::getDerivatives(this, p1.x, p2.x);
			double jacobianDeterminant = jacobian::getJacobianDeterminant(this, p1.x, p2.x);
			double dNdX[4];
			double dNdY[4];
			for (int i = 0; i < 4; i++)
			{
				dNdX[i] = shapeFunctionsDerivatives.at(i).getValue(0, 0);
				dNdY[i] = shapeFunctionsDerivatives.at(i).getValue(1, 0);
			}

			matrix d1(dNdX, 4);
			matrix d2(dNdY, 4);
			H = H + (d1 * d1.transpose() + d2 * d2.transpose()) * jacobianDeterminant * p1.weight * p2.weight;
		}
	}
	H = H * globalData->k;
	H = H + getHbc();
	return H;
}
matrix element::getC()
{
	matrix C(4, 4);
	for (integrationPoint p1 : globalData->integrationPoints)
	{
		for (integrationPoint p2 : globalData->integrationPoints) {
			matrix N = shapeFunctions::getShapeFunctionsValues(p1.x, p2.x);
			C = C + (N * N.transpose() * globalData->c * globalData->rho * jacobian::getJacobianDeterminant(this, p1.x, p2.x) * p1.weight * p2.weight);
		}
	}
	return C;
}
matrix element::getHbc()
{
	matrix Hbc(4, 4);
	for (int i = 0; i < this->nodes.size(); i++)
	{
		bool changeXi = false;
		double changeTo;
		if (nodes.at(i)->boundaryCondition && nodes.at((i + 1) % nodes.size())->boundaryCondition)
		{
			double sideLenght = std::sqrt(std::pow(nodes.at(i)->x - nodes.at((i + 1) % nodes.size())->x, 2) + std::pow(nodes.at(i)->y - nodes.at((i + 1) % nodes.size())->y, 2));
			switch (i)
			{
			case 0:	//bottom
				changeXi = false;
				changeTo = -1;
				break;
			case 1:	//right
				changeXi = true;
				changeTo = 1;
				break;
			case 2:	//top
				changeXi = false;
				changeTo = 1;
				break;
			case 3:	//left
				changeXi = true;
				changeTo = -1;
				break;
			}
			for (integrationPoint p : globalData->integrationPoints)
			{
				
				matrix shapeFunctions;
				if (changeXi) shapeFunctions = shapeFunctions::getShapeFunctionsValues(changeTo, p.x);
				else shapeFunctions = shapeFunctions::getShapeFunctionsValues(p.x, changeTo);
				//std::cout << "N in Hbc, my dude be like 1/2 empty, ok dude?\n" << shapeFunctions.toString();
				//std::cout << "N*N^T\n" << (shapeFunctions * shapeFunctions.transpose() * globalData->alpha).toString();
				Hbc = Hbc + shapeFunctions * shapeFunctions.transpose() * globalData->alpha * (sideLenght / 2) * p.weight;
			}
		}
	}
	return Hbc;
}
matrix element::getP()
{
	matrix P(4, 1);
	for (int i = 0; i < this->nodes.size(); i++)
	{
		bool changeXi = false;
		double changeTo;
		if (nodes.at(i%nodes.size())->boundaryCondition && nodes.at((i + 1) % nodes.size())->boundaryCondition)
		{
			double dx = nodes.at(i)->x - nodes.at((i + 1) % nodes.size())->x;
			double dy = nodes.at(i)->y - nodes.at((i + 1) % nodes.size())->y;
			double sideLenght = std::sqrt(std::pow(dx, 2) + std::pow(dy, 2));
			switch (i)
			{
			case 0:	//bottom
				changeXi = false;
				changeTo = -1;
				break;
			case 1:	//right
				changeXi = true;
				changeTo = 1;
				break;
			case 2:	//top
				changeXi = false;
				changeTo = 1;
				break;
			case 3:	//left
				changeXi = true;
				changeTo = -1;
				break;
			}
			for (integrationPoint p : globalData->integrationPoints)
			{

				matrix shapeFunctions;
				if (changeXi) shapeFunctions = shapeFunctions::getShapeFunctionsValues(changeTo, p.x);
				else shapeFunctions = shapeFunctions::getShapeFunctionsValues(p.x, changeTo);
				P = P - (shapeFunctions * globalData->alpha * globalData->ambientTemperature * p.weight)*(sideLenght/2);
			}
		}
	}
	return P;
	
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