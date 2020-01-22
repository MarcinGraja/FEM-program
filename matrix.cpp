#include "matrix.h"
#include <exception>
matrix::matrix(int x, int y)
{
	data.resize(x);
	for (int i = 0; i < x; i++)
	{
		data.at(i).resize(y);
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
		throw ("invalid size for matrix addition\n");
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
			out += std::to_string(data.at(row).at(column)) + "\t";
		}
		out += '\n';
	}
	out += "\n";
	return out;
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
