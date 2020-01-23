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
//shamelessly stolen from https://www.geeksforgeeks.org/adjoint-inverse-matrix/
void getCofactor(double ** A, double** temp, int p, int q, int n)
{
	int i = 0, j = 0;

	// Looping for each element of the matrix 
	for (int row = 0; row < n; row++)
	{
		for (int col = 0; col < n; col++)
		{
			//  Copying into temporary matrix only those element 
			//  which are not in given row and column 
			if (row != p && col != q)
			{
				temp[i][j++] = A[row][col];

				// Row is filled, so increase row index and 
				// reset col index 
				if (j == n - 1)
				{
					j = 0;
					i++;
				}
			}
		}
	}
}
/* Recursive function for finding determinant of matrix.
   n is current dimension of A[][]. */
double  matrix::determinant(double **A, int n)
{
	if (this->getColumns() != this->getRows())
	{
		throw std::invalid_argument("matrix must be a square matrix to calculate determinant\n");
	}
	double D = 0; // Initialize result 

	//  Base case : if matrix contains single element 
	if (n == 1)
		return A[0][0];

	double **temp = new double*[this->getRows()];
	for (int i = 0; i < this->getRows(); i++)
	{
		temp[i] = new double[this->getColumns()];
	}
	int sign = 1;  // To store sign multiplier 

	 // Iterate for each element of first row 
	for (int f = 0; f < n; f++)
	{
		// Getting Cofactor of A[0][f] 
		getCofactor(A, temp, 0, f, n);
		D += sign * A[0][f] * determinant(temp, n - 1);

		// terms are to be added with alternate sign 
		sign = -sign;
	}

	return D;
}