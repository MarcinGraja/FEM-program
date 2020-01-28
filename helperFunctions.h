#pragma once
#include "element.h"
namespace jacobian
{
	double ** getJacobian(element * e, double xi, double eta);
	double getJacobianDeterminant(element * e, double xi, double eta);
	double ** getInverseJacobian(element * e, double xi, double eta);
}
namespace shapeFunctions
{
	std::vector<matrix> getDerivatives(element * e, double xi, double eta);
	matrix getShapeFunctionsValues(double xi, double eta);
}