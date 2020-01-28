#pragma once
#include <string>
#include <vector>
class matrix
{
	std::vector<std::vector<double>>data;
public:
	matrix(int x, int y);
	matrix(double * data, int size);
	matrix(double ** data, int rows, int columns);
	matrix(std::vector<double> data);
	matrix(std::vector<std::vector<double>> data);
	matrix();
	matrix operator*(matrix const &other);
	matrix operator*(double d);
	matrix operator+(matrix const &other);
	matrix operator-(matrix const &other);
	matrix transpose();
	void addValue(int i, int j, double value);
	double getValue(int row, int column);
	std::string toString();
	
	double maxValue();
	double minValue();
	std::string getSize()const;
	std::string getSize();
	int getRows()const;
	int getColumns()const;
	bool operator==(const matrix &other) const;
	~matrix();
	matrix inverse();
	matrix(double *data, int rows, int columns);
};

