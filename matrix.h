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
	double getValue(int row, int column);
	std::string toString();
	
	int getRows()const;
	int getColumns()const;
	bool operator==(const matrix &other) const;
	~matrix();
	double determinant(double ** A, int n);
};

