#include <iostream>
#include "data.h"
#include "gridGenerator.h"

int main()
{
	data *d = new data();
	d->initializeData();
	element::setDataCopy(d);
	gridGenerator grid(d);
	grid.generateGrid();
	std::cout.precision(4);
	std::cout << std::fixed;
	grid.printNodes();
	grid.printElements();
	std::cin.ignore();
}