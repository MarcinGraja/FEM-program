#include "matrix.h"
#include <exception>
#include "helperFunctions.h"
#include <algorithm>
#include <iostream>
#include <Eigen/Dense>
matrix::matrix(int x, int y)
{
	data.resize(x);
	for (int i = 0; i < x; i++)
	{
		data.at(i).resize(y);
	}
	
}
matrix::matrix(double *data, int rows, int columns)
{
	this->data.resize(rows);
	for (int row = 0; row < rows; row++)
	{
		this->data[row].resize(columns);
		for (int column = 0; column < columns; column++)
		{
			this->data.at(row).at(column) = data[row*columns + column];
		}
	}
}
matrix::matrix(double *data, int size)
{
	this->data.resize(size);
	for (int i = 0; i < getRows(); i++)
	{
		this->data.at(i).push_back(data[i]);
	}
}

matrix::matrix(double ** data, int rows, int columns)
{
	this->data.resize(rows);
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < columns; j++)
		{
			this->data.at(i).push_back(data[i][j]);
		}
	}
}
matrix::matrix(std::vector<std::vector<double>> data)
{
	this->data = data;
}
matrix::matrix(std::vector<double> data)
{
	this->data.push_back(data);
}
matrix matrix::operator*(matrix const &other)
{
	if (getColumns() != other.getRows())
	{
		std::cout << "this size: " << this->getSize() << " other size: " << other.getSize();
		throw new std::invalid_argument("invalid size for matrix multiplication\n");
	}
	matrix m(getRows(), other.getColumns());
	for (int row = 0; row < getRows(); row++)
	{
		for (int column = 0; column < other.getColumns(); column++)
		{
			for (int k = 0; k < other.getRows(); k++)
			{
				double toAdd = data.at(row).at(k) * other.data.at(k).at(column);
				m.data.at(row).at(column) += toAdd;
			}
		}
	}
	return m;
}
matrix matrix::operator*(double d)
{
	matrix m(getRows(), getColumns());
	for (int i = 0; i < getRows(); i++)
	{
		for (int j = 0; j < getColumns(); j++)
		{
			m.data.at(i).at(j) = this->data.at(i).at(j) * d;
		}
	}
	return m;
}
matrix matrix::operator+(matrix const &other)
{
	if (getRows() != other.getRows()|| getColumns() != other.getColumns())
	{
		throw std::invalid_argument("invalid size for matrix addition\n");
	}
	matrix m(getRows(), getColumns());
	for (int row = 0; row < getRows(); row++)
	{
		for (int column = 0; column < getColumns(); column++)
		{
			m.data.at(row).at(column) = data.at(row).at(column) + other.data.at(row).at(column);
		}
	}
	return m;
}
matrix matrix::operator-(matrix const & other)
{
	if (getRows() != other.getRows() || getColumns() != other.getColumns())
	{
		throw std::invalid_argument("invalid size for matrix addition\n");
	}
	matrix m(getRows(), getColumns());
	for (int row = 0; row < getRows(); row++)
	{
		for (int column = 0; column < getColumns(); column++)
		{
			m.data.at(row).at(column) = data.at(row).at(column) - other.data.at(row).at(column);
		}
	}
	return m;
	return matrix();
}
matrix matrix::transpose()
{
	matrix m(getColumns(), getRows());
	for (int i = 0; i < getColumns(); i++)
	{
		for (int j = 0; j < getRows(); j++)
		{
			m.data.at(i).at(j) = data.at(j).at(i);
		}
	}
	return m;
}
void matrix::addValue(int i, int j, double value)
{
	data.at(i).at(j) += value;
}
double matrix::getValue(int row, int column)
{
	return data.at(row).at(column);
}
std::string matrix::toString()
{
	std::string out;
	for (int row = 0; row < getRows(); row++)
	{
		for (int column = 0; column < getColumns(); column++)

		{
			out += std::to_string(data.at(row).at(column)) + " ";
		}
		out += '\n';
	}
	out += "\n";
	return out;
}
double matrix::maxValue()
{
	double max = data.at(0).at(0);
	for (std::vector<double> v : data)
	{
		for (double d : v)
		{
			if (max < d) max = d;
		}
	}
	return max;
}
double matrix::minValue()
{
	double min = data.at(0).at(0);
	for (std::vector<double> v : data)
	{
		for (double d : v)
		{
			if (min > d) min = d;
		}
	}
	return min;
}
std::string matrix::getSize()
{
	return std::string(std::to_string(this->getRows()) + "x" + std::to_string(this->getColumns()));
}
std::string matrix::getSize() const
{
	return std::string(std::to_string(this->getRows()) + "x" + std::to_string(this->getColumns()));
}
matrix::matrix()
{
}
int matrix::getRows() const
{
	return data.size();
}
int matrix::getColumns() const
{
	return data.at(0).size();
}
bool matrix::operator==(const matrix &other)const
{
	return this->data == other.data;
}
matrix::~matrix()
{
}

matrix matrix::inverse()
{
	if (this->getRows() != this->getColumns())
	{
		throw new std::invalid_argument("non square matrix cannot be inverted");
	}
	Eigen::MatrixXd eMatrix(data.size(), data[0].size());
	for (int i = 0; i < data.size(); ++i)
		eMatrix.row(i) = Eigen::VectorXd::Map(&data[i][0], data[0].size());
	double *out = new double[this->getRows() * this->getColumns()];
	Eigen::Map<Eigen::MatrixXd>(&out[0], this->getRows(), this->getColumns()) = eMatrix.inverse();
	matrix mOut(out, this->getRows(), this->getColumns());
	return mOut;
}
