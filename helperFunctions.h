#pragma once
#include <vector>
#include "matrix.h"
#include "element.h"
#include <exception>
namespace jacobian
{
	double ** getJacobian(element *e, double xi, double eta)
	{
		double **jacobian = new double*[2];
		jacobian[0] = new double[2];
		jacobian[1] = new double[2];
		double x[4], y[4];
		for (int i = 0; i < 4; i++)
		{
			node *tmp = e->getNode(i);
			x[i] = tmp->x;
			y[i] = tmp->y;
		}
		// dx/dxi
		jacobian[0][0] = 0.25 * eta * (x[0] - x[1] + x[2] - x[3]) + 0.25 * (-x[0] + x[1] + x[2] - x[3]);
		// dy/dxi

		jacobian[0][1] = 0.25 * eta * (y[0] - y[1] + y[2] - y[3]) + 0.25 * (-y[0] + y[1] + y[2] - y[3]);
		// dx/deta
		jacobian[1][0] = 0.25 * xi * (x[0] - x[1] + x[2] - x[3]) + 0.25 * (-x[0] - x[1] + x[2] + x[3]);
		// dy/deta
		jacobian[1][1] = 0.25 * xi * (y[0] - y[1] + y[2] - y[3]) + 0.25 * (-y[0] - y[1] + y[2] + y[3]);
		return jacobian;
	}

	double getJacobianDeterminant(element *e, double xi, double eta)
	{
		double **jacobian = getJacobian(e, xi, eta);
		return (jacobian[0][0] * jacobian[1][1] - jacobian[0][1] * jacobian[1][0]);
	}
	double ** getInverseJacobian(element *e, double xi, double eta)
	{
		{
			double **returned = getJacobian(e, xi, eta);
			double matrixDeterminant = returned[0][0] * returned[1][1] - returned[0][1] * returned[1][0];
			double tmp = returned[0][0];
			returned[0][0] = returned[1][1] / matrixDeterminant;
			returned[0][1] /= -matrixDeterminant;
			returned[1][0] /= -matrixDeterminant;
			returned[1][1] = tmp / matrixDeterminant;
			return returned;
		}
	}
}
namespace shapeFunctions
{
	std::vector<matrix> getDerivatives(element *e, double xi, double eta)
	{
		std::vector<std::vector<double>>dNdXidEta;
		dNdXidEta.resize(4);
		for (int i = 0; i < 4; i++)
		{
			dNdXidEta.at(i).resize(2);
		}
		matrix inverseJacobian(jacobian::getInverseJacobian(e, xi, eta), 2, 2);
		//dN/dXi

		dNdXidEta.at(0).at(0) = -0.25 *(1 - eta);
		dNdXidEta.at(1).at(0) = 0.25 * (1 - eta);
		dNdXidEta.at(2).at(0) = 0.25 * (1 + eta);
		dNdXidEta.at(3).at(0) = -0.25 * (1 + eta);

		//dN/dEta
		dNdXidEta.at(0).at(1) = -0.25 * (1 - xi);
		dNdXidEta.at(1).at(1) = -0.25 * (1 + xi);
		dNdXidEta.at(2).at(1) = 0.25 * (1 + xi);
		dNdXidEta.at(3).at(1) = 0.25 * (1 - xi);
		std::vector<matrix> out;
		for (int i = 0; i < 4; i++)
		{
			matrix tmp = matrix(dNdXidEta.at(i)).transpose();
			out.push_back(inverseJacobian * tmp);
		}

		return out;
	}
	matrix getShapeFunctionsValues(double xi, double eta)
	{
		double *data = new double[4];
		data[0] = (1 - xi)*(1 - eta) * 0.25; 
		data[1] = (1 + xi)*(1 - eta) * 0.25;
		data[2] = (1 + xi)*(1 + eta) * 0.25;
		data[3] = (1 - xi)*(1 + eta) * 0.25;
		return matrix(data, 4);
	}
}
