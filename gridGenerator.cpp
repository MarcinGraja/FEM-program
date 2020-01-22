#pragma strict_gs_check(on)
#include "gridGenerator.h"
data *gridGenerator::dataCopy;
gridGenerator::gridGenerator(data *dataCopy)
{
	this->dataCopy = dataCopy;
	this->globalData.nodeCountH = dataCopy->nodeCountH;
	this->globalData.nodeCountW = dataCopy->nodeCountW;
	this->globalData.deltaH = dataCopy->H / (dataCopy->nodeCountH - 1);
	this->globalData.deltaW = dataCopy->W / (dataCopy->nodeCountW - 1);
}
void gridGenerator::generateGrid()
{
	int nodeCount = globalData.nodeCountH * globalData.nodeCountW;
	nodes.reserve(nodeCount);
	for (int i = 0; i < nodeCount; i++)
	{
		/*

		----^
		|	|
		|	|
	H	◄	y
		|	|
		----|------x----->
			|            |
			-------▼------
			       W
		*/
		double t = NULL;
		double x = i / globalData.nodeCountH * globalData.deltaW;
		double y = i % globalData.nodeCountH * globalData.deltaH;
		bool boundaryCondition =
			(i + 1) % globalData.nodeCountH == 0	//upper side 
			|| i % globalData.nodeCountH == 0	//lower side
			|| i < globalData.nodeCountH		//left side
			|| i > globalData.nodeCountH * globalData.nodeCountW - 1 - globalData.nodeCountH;	//right side
		nodes.push_back(new node(i, t, x, y, boundaryCondition));
	}
	int elementCount = (globalData.nodeCountH - 1)*(globalData.nodeCountW - 1);
	elements.reserve(elementCount);
	int offset = 0;
	for (int i = 0; i < elementCount; i++)
	{
		if ((i+1 + offset) % globalData.nodeCountH == 0 && i != 0)	offset++;
		std::vector <node*> nodes;
		/*
		k+1------k + nodeCountH + 1
		 |		 |
		 |		 |
		 |		 |
		 |		 |
		 k-------k + nodeCountH
		*/
		int k = i + offset;
		nodes.push_back(this->nodes.at(k));
		nodes.push_back(this->nodes.at(k + globalData.nodeCountH));
		nodes.push_back(this->nodes.at(k + globalData.nodeCountH + 1));
		nodes.push_back(this->nodes.at(k + 1));
		elements.push_back(element(i, nodes));
	}
}
double * gridGenerator::getPosition(int i)
{
	return new double[2]{ nodes.at(i)->x, nodes.at(i)->y };
}
bool gridGenerator::getBoundaryCondition(int i)
{
	return nodes.at(i)->boundaryCondition;
}

void gridGenerator::printNodes()
{
	for (node *n : nodes)
	{
		n->print();
	}
}

void gridGenerator::printElements()
{
	for (element e : elements)
	{
		e.print();
	}
}
