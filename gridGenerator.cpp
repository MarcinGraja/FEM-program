﻿#pragma strict_gs_check(on)
#include "gridGenerator.h"
#include <iostream>
data *gridGenerator::dataCopy;
gridGenerator::gridGenerator(data *dataCopy)
{
	this->dataCopy = dataCopy;
	this->globalData.nodeCountH = dataCopy->nodeCountH;
	this->globalData.nodeCountW = dataCopy->nodeCountW;
	this->globalData.deltaH = dataCopy->H / (dataCopy->nodeCountH - 1);
	this->globalData.deltaW = dataCopy->W / (dataCopy->nodeCountW - 1);
}
void gridGenerator::genGridManual()
{
	nodes.push_back(new node(0, 0, 0, 0, true));
	nodes.push_back(new node(1, 0, 0.025, 0, true));
	nodes.push_back(new node(2, 0, 0.025, 0.025, false));
	nodes.push_back(new node(3, 0, 0, 0.025, false));
	elements.push_back(element(0, nodes));
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

void gridGenerator::prepareMatrices()
{
	std::cout << "preparing matrices\n";
	double *data = new double[this->nodes.size()];
	for (int i = 0; i < this->nodes.size(); i++)
	{
		data[i] = dataCopy->initialTemp;
	}
	temperature = matrix(data, this->nodes.size());
	delete[] data;
	H = matrix(this->nodes.size(), this->nodes.size());
	C = matrix(this->nodes.size(), this->nodes.size());
	P = matrix(this->nodes.size(), 1);
	for (element e : elements)
	{
		matrix Hl = e.getH();
		matrix Pl = e.getP();
		for (int i = 0; i < 4; i++)
		{
			
			P.addValue(e.nodes.at(i)->id, 0, Pl.getValue(i, 0));
			for (int j = 0; j < 4; j++)
			{
				
				H.addValue(e.nodes.at(i)->id, e.nodes.at(j)->id, Hl.getValue(i, j));

				matrix Cl = e.getC();
				C.addValue(e.nodes.at(i)->id, e.nodes.at(j)->id, Cl.getValue(i, j));

				
			}
		}
	}
	std::cout << "C, H, P matrices initialization complete\n";
	//std::cout << "C:\n" << C.toString();
	CdT = C * (1.0 / dataCopy->timeStep);
	matrix HaddCdT = H + CdT;
	matrix PaddCdTtimesT = CdT * temperature - P;
	std::cout << "starting inversion...\n";
	finalH = (H + CdT).inverse();
	/*std::cout << "H:\n" << H.toString() << "C:\n" << C.toString() << "P:\n" << P.toString();
	std::cout << "C/dT:\n" << CdT.toString();
	std::cout << "H + C/dT: \n" << (HaddCdT).toString() << "P + C/dt * temperature\n" << PaddCdTtimesT.toString();
	std::cout << '\n';*/
	std::cout << "matrix initialization complete\n";
}

void gridGenerator::run()
{
	double currentTime = 0;
	std::cout << "time: \t" << currentTime << " min temp:\t" << temperature.minValue() << " max temp:\t" << temperature.maxValue() << '\n';
	while (currentTime < dataCopy->simulationTime)
	{
		/*matrix tmp = temperature - P;
		tmp = CdT * tmp;
		tmp = finalH * tmp;
		/**/
		temperature = finalH * (CdT * temperature - P);
		//std::cout << temperature.toString();
		currentTime += dataCopy->timeStep;
		std::cout << "time: \t" << currentTime << " min temp:\t" << temperature.minValue() << " max temp:\t" << temperature.maxValue() << '\n';
	}
}
