#pragma once
#include "element.h"
#include <vector>
#include "data.h"
class gridGenerator
{
	std::vector <node*> nodes;
	std::vector <element> elements;
	static data *dataCopy;
	struct
	{
		int nodeCountH;
		int nodeCountW;
		double deltaH;
		double deltaW;
	}globalData;
public:
	gridGenerator(data *dataCopy);
	void genGridManual();
	void generateGrid();
	double * getPosition(int i);
	bool getBoundaryCondition(int i);
	void printNodes();
	void printElements();
	void updateGrid();
};

